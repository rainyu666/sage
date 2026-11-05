int s_a = 0;
int s_b = 0;
int s_c = 0;
int s_d = 0;
int s_k = 0;
int s_l = 0;
int s_r = 0;
int s_enabled = 1;
int s_bad = 0;
bool pending=false; bool used=false;
inline deliver() { if :: pending && s_enabled && !used -> used=true; pending=false; s_c = (s_c + 1); :: else -> skip; fi; }
inline request() { if :: !pending && !used -> if :: pending=true :: skip fi :: else -> skip fi; deliver(); }
init { atomic { assert(!s_bad);
request();
s_a = (s_a + 1);
s_enabled = 0;
s_k = 1;
s_k = 0;
assert(!s_bad);
request();
s_enabled = 0;
s_b = 1;
s_l = 0;
s_b = 0;
s_enabled = 0;
s_a = (s_a - 1);
s_enabled = 1;
s_r = (-19);
assert(!s_bad);
request();
s_bad = ((s_a != 0) || (s_b != 0) || (s_enabled != 1));
assert(!s_bad);
request(); assert(!s_bad); } }
