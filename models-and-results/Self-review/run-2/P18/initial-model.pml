int s_a = 255;
int s_b = 0;
int s_c = 0;
int s_d = 0;
int s_k = 0;
int s_l = 0;
int s_r = 0;
int s_enabled = 1;
int s_bad = 0;
bool pending=false; bool used=false;
inline deliver() { if :: pending && s_enabled && !used -> used=true; pending=false; s_a = 0;
s_b = 1; :: else -> skip; fi; }
inline request() { if :: !pending && !used -> if :: pending=true :: skip fi :: else -> skip fi; deliver(); }
init { atomic { assert(!s_bad);
request();
s_c = s_a;
assert(!s_bad);
request();
s_d = s_b;
assert(!s_bad);
request();
s_r = (s_c + (256 * s_d));
s_bad = (s_bad || ((s_r != 255) && (s_r != 256)));
assert(!s_bad);
request(); assert(!s_bad); } }
