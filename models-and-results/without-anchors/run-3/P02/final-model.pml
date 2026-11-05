int s_a = 1;
int s_b = 1;
int s_c = 0;
int s_d = 0;
int s_k = 0;
int s_l = 0;
int s_r = 0;
int s_enabled = 1;
int s_bad = 0;
bool pending=false; bool used=false;
inline deliver() { if :: pending && s_enabled && !used -> used=true; pending=false; s_a = 2;
s_b = 2; :: else -> skip; fi; }
inline request() { if :: !pending && !used -> if :: pending=true :: skip fi :: else -> skip fi; deliver(); }
init { atomic { assert(!s_bad);
request();
s_k = s_enabled;
s_enabled = (0 -> 0 : s_enabled);
assert(!s_bad);
request();
s_c = s_a;
assert(!s_bad);
request();
s_d = s_b;
assert(!s_bad);
request();
s_enabled = s_k;
assert(!s_bad);
request();
s_bad = (s_c != s_d);
assert(!s_bad);
request(); assert(!s_bad); } }
