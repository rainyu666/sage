int s_a = 65535;
int s_b = 0;
int s_c = 0;
int s_d = 0;
int s_k = 0;
int s_l = 0;
int s_r = 0;
int s_enabled = 1;
int s_bad = 0;
bool pending=false; bool used=false;
inline deliver() { if :: pending && s_enabled && !used -> used=true; pending=false; s_a = 65536; :: else -> skip; fi; }

int target; init { atomic { target=-1; do
:: target <= 2 -> pending=false; used=false;
s_a=65535;
s_b=0;
s_c=0;
s_d=0;
s_k=0;
s_l=0;
s_r=0;
s_enabled=1;
s_bad=0;
printf("TRACE %d %d %d %d %d %d %d %d %d %d %d\n",target,0,s_a,s_b,s_c,s_d,s_k,s_l,s_r,s_enabled,s_bad);
if :: target==0 -> pending=true; :: else -> skip; fi; deliver();
s_c=s_a;
printf("TRACE %d %d %d %d %d %d %d %d %d %d %d\n",target,1,s_a,s_b,s_c,s_d,s_k,s_l,s_r,s_enabled,s_bad);
if :: target==1 -> pending=true; :: else -> skip; fi; deliver();
s_d=((s_c == 65535) -> (((s_c % 65536) + 65536) % 65536) : s_d);
s_bad=((s_c == 65535) -> (s_d == 0) : s_bad);
printf("TRACE %d %d %d %d %d %d %d %d %d %d %d\n",target,2,s_a,s_b,s_c,s_d,s_k,s_l,s_r,s_enabled,s_bad);
if :: target==2 -> pending=true; :: else -> skip; fi; deliver();
printf("TRACE %d %d %d %d %d %d %d %d %d %d %d\n",target,3,s_a,s_b,s_c,s_d,s_k,s_l,s_r,s_enabled,s_bad);
target++; :: else -> break; od; } }
