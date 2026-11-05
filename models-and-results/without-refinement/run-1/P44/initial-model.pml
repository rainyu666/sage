int s_a = 2;
int s_b = 2;
int s_c = 1;
int s_d = 0;
int s_k = 0;
int s_l = 0;
int s_r = 0;
int s_enabled = 1;
int s_bad = 0;
bool pending=false; bool used=false;
inline deliver() { if :: pending && s_enabled && !used -> used=true; pending=false; s_bad = (((s_b != 0) && (!((s_a != 0) && ((s_a == 1) || ((s_a == 2) && (s_c == 1)))))) -> 1 : s_bad);
s_a = ((s_b != 0) -> 0 : ((s_a == 0) -> 1 : 1));
s_b = ((s_b != 0) -> s_b : ((s_a == 0) -> s_c : ((s_a == 1) -> s_b : s_c)));
s_c = ((s_b != 0) -> s_c : ((s_a == 0) -> s_c : ((s_a == 1) -> s_b : s_c)));
s_enabled = s_enabled; :: else -> skip; fi; }
inline request() { if :: !pending && !used -> if :: pending=true :: skip fi :: else -> skip fi; deliver(); }
init { atomic { assert(!s_bad);
request();
s_k = s_enabled;
s_enabled = 0;
assert(!s_bad);
request();
s_a = (((s_a == 0) || ((s_a == 1) && (s_b == 1)) || ((s_a == 2) && (s_c == 2))) -> 0 : s_a);
s_c = (((s_a == 0) || ((s_a == 1) && (s_b == 1)) || ((s_a == 2) && (s_c == 2))) -> s_c : ((s_a == 1) -> s_b : ((s_a == 2) -> s_b : s_c)));
s_b = s_b;
s_d = ((s_a == 0) -> 0 : ((s_a == 1) -> s_b : ((s_a == 2) -> s_c : 0)));
assert(!s_bad);
request();
s_b = ((s_d == 1) -> 0 : s_b);
s_c = ((s_d == 2) -> 0 : s_c);
assert(!s_bad);
request();
s_enabled = s_k;
assert(!s_bad);
request(); assert(!s_bad); } }
