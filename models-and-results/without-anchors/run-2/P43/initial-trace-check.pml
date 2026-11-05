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
inline deliver() { if :: pending && s_enabled && !used -> used=true; pending=false; s_bad = (((s_b != 0) && (!((s_a == 1) || ((s_a == 2) && (s_c == 1))))) -> 1 : s_bad);
s_b = (((s_b == 0) && (s_a == 2)) -> 2 : (((s_b == 0) && (s_a == 0)) -> 1 : s_b));
s_c = (((s_b == 0) && (s_a == 2)) -> 1 : s_c);
s_a = (((s_b == 0) && (s_a == 0)) -> 1 : s_a);
s_enabled = s_enabled; :: else -> skip; fi; }

int target; init { atomic { target=-1; do
:: target <= 4 -> pending=false; used=false;
s_a=2;
s_b=2;
s_c=1;
s_d=0;
s_k=0;
s_l=0;
s_r=0;
s_enabled=1;
s_bad=0;
printf("TRACE %d %d %d %d %d %d %d %d %d %d %d\n",target,0,s_a,s_b,s_c,s_d,s_k,s_l,s_r,s_enabled,s_bad);
if :: target==0 -> pending=true; :: else -> skip; fi; deliver();
s_k=s_enabled;
s_enabled=0;
printf("TRACE %d %d %d %d %d %d %d %d %d %d %d\n",target,1,s_a,s_b,s_c,s_d,s_k,s_l,s_r,s_enabled,s_bad);
if :: target==1 -> pending=true; :: else -> skip; fi; deliver();
s_a=(((s_a != 0) && (((s_a == 1) && (s_b == s_a)) || ((s_a == 2) && (s_c == s_a)))) -> 0 : s_a);
s_b=(((s_a == 2) && (s_a != s_c) && (s_c == 1)) -> s_c : s_b);
s_c=(((s_a == 1) && (s_a != s_b) && (s_b == 2)) -> s_b : s_c);
s_d=((s_a == 1) -> s_b : ((s_a == 2) -> s_c : 0));
printf("TRACE %d %d %d %d %d %d %d %d %d %d %d\n",target,2,s_a,s_b,s_c,s_d,s_k,s_l,s_r,s_enabled,s_bad);
if :: target==2 -> pending=true; :: else -> skip; fi; deliver();
s_enabled=s_k;
printf("TRACE %d %d %d %d %d %d %d %d %d %d %d\n",target,3,s_a,s_b,s_c,s_d,s_k,s_l,s_r,s_enabled,s_bad);
if :: target==3 -> pending=true; :: else -> skip; fi; deliver();
s_b=((s_d == 1) -> 0 : s_b);
s_c=((s_d == 2) -> 0 : s_c);
printf("TRACE %d %d %d %d %d %d %d %d %d %d %d\n",target,4,s_a,s_b,s_c,s_d,s_k,s_l,s_r,s_enabled,s_bad);
if :: target==4 -> pending=true; :: else -> skip; fi; deliver();
printf("TRACE %d %d %d %d %d %d %d %d %d %d %d\n",target,5,s_a,s_b,s_c,s_d,s_k,s_l,s_r,s_enabled,s_bad);
target++; :: else -> break; od; } }
