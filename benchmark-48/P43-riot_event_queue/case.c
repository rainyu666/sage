/*
 * SPDX-FileCopyrightText: 2017 Inria
 * SPDX-FileCopyrightText: 2017 Kaspar Schleiser <kaspar@schleiser.de>
 * SPDX-FileCopyrightText: 2018 Freie Universität Berlin
 * SPDX-License-Identifier: LGPL-2.1-only
 */

#include <stddef.h>
#include <assert.h>

typedef struct list_node {
    struct list_node *next;
} list_node_t;

typedef list_node_t clist_node_t;

typedef struct event {
    clist_node_t list_node;
    void (*handler)(struct event *);
} event_t;

typedef int thread_t;

typedef struct event_queue {
    clist_node_t event_list;
    thread_t *waiter;
} event_queue_t;

int d, k, l, r, enabled, bad;
event_t event1, event2;
event_queue_t queue;

int node_id(const clist_node_t *n)
{
    return n == &event1.list_node ? 1 : n == &event2.list_node ? 2 : 0;
}

#define a node_id(queue.event_list.next)
#define b node_id(event1.list_node.next)
#define c node_id(event2.list_node.next)
#define container_of(ptr, type, member) ((type *)((char *)(ptr) - offsetof(type, member)))
#define THREAD_FLAG_EVENT 1

void thread_flags_set(thread_t *thread, int flag)
{
    (void)thread;
    (void)flag;
}

unsigned irq_disable(void)
{
    unsigned key = enabled;
    enabled = 0;
    return key;
}

void irq_restore(unsigned key)
{
    enabled = key;
}

void handler(event_t *event)
{
    (void)event;
}

int reachable(const event_queue_t *q, const event_t *event)
{
    const clist_node_t *tail = q->event_list.next;
    if (!tail)
        return 0;
    const clist_node_t *node = tail;
    do {
        if (node == &event->list_node)
            return 1;
        node = node->next;
    } while (node != tail);
    return 0;
}

void initialize(void)
{
    d = k = l = r = bad = 0;
    enabled = 1;
    event1.list_node.next = &event2.list_node;
    event2.list_node.next = &event1.list_node;
    event1.handler = handler;
    event2.handler = handler;
    queue.event_list.next = &event2.list_node;
    queue.waiter = NULL;
}

static inline void clist_rpush(clist_node_t *list, clist_node_t *new_node)
{
    if (list->next) {
        new_node->next = list->next->next;
        list->next->next = new_node;
    } else {
        new_node->next = new_node;
    }
    list->next = new_node;
}

static inline clist_node_t *clist_lpop(clist_node_t *list)
{
    if (list->next) {
        clist_node_t *first = list->next->next;
        if (list->next == first) {
            list->next = NULL;
        } else {
            list->next->next = first->next;
        }
        return first;
    } else {
        return NULL;
    }
}

void event_post(event_queue_t *queue, event_t *event)
{
    assert(queue && event);
    assert(event->handler);

    unsigned state = irq_disable();
    if (!event->list_node.next) {
        clist_rpush(&queue->event_list, &event->list_node);
    }
    thread_t *waiter = queue->waiter;
    irq_restore(state);

    if (waiter) {
        thread_flags_set(waiter, THREAD_FLAG_EVENT);
    }
}

void device_irq(void)
{
    if (event1.list_node.next && !reachable(&queue, &event1))
        bad = 1;
    event_post(&queue, &event1);
}

event_t *event_get(event_queue_t *queue)
{
    unsigned state;
    STEP(0, {
        state = irq_disable();
        k = (int)state;
    });
    event_t *result;
    STEP(1, {
        result = (event_t *)clist_lpop(&queue->event_list);
        d = node_id(result ? &result->list_node : NULL);
    });
    STEP(2, { irq_restore(state); });

    STEP(3, {
        if (result) {
            result->list_node.next = NULL;
        }
    });
    return result;
}

void foreground(void)
{
    event_get(&queue);
}
