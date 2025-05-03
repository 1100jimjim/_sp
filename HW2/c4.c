// asmPower2.c - 以純機器碼硬塞實作 int power2(int n)，計算 2^n
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <unistd.h>
#include <fcntl.h>
#define int long long // 64-bit

int *e;
int debug = 0;

enum { LLA ,IMM ,JMP ,JSR ,BZ  ,BNZ ,ENT ,ADJ ,LEV ,LI  ,LC  ,SI  ,SC  ,PSH ,
       OR  ,XOR ,AND ,EQ  ,NE  ,LT  ,GT  ,LE  ,GE  ,SHL ,SHR ,ADD ,SUB ,MUL ,DIV ,MOD ,
       OPEN,READ,CLOS,PRTF,MALC,FREE,MSET,MCMP,EXIT };

int run(int *pc, int *bp, int *sp) {
  int a, cycle = 0, i, *t;
  while (1) {
    i = *pc++; ++cycle;
    if (debug) {
      printf("%d> %.4s", cycle,
        &"LLA ,IMM ,JMP ,JSR ,BZ  ,BNZ ,ENT ,ADJ ,LEV ,LI  ,LC  ,SI  ,SC  ,PSH ,"
         "OR  ,XOR ,AND ,EQ  ,NE  ,LT  ,GT  ,LE  ,GE  ,SHL ,SHR ,ADD ,SUB ,MUL ,DIV ,MOD ,"
         "OPEN,READ,CLOS,PRTF,MALC,FREE,MSET,MCMP,EXIT,"[i * 5]);
      if (i <= ADJ) printf(" %d\n", *pc); else printf("\n");
    }
    if      (i == LLA) a = (int)(bp + *pc++);
    else if (i == IMM) a = *pc++;
    else if (i == JMP) pc = (int *)*pc;
    else if (i == JSR) { *--sp = (int)(pc + 1); pc = (int *)*pc; }
    else if (i == BZ)  pc = a ? pc + 1 : (int *)*pc;
    else if (i == BNZ) pc = a ? (int *)*pc : pc + 1;
    else if (i == ENT) { *--sp = (int)bp; bp = sp; sp = sp - *pc++; }
    else if (i == ADJ) sp = sp + *pc++;
    else if (i == LEV) { sp = bp; bp = (int *)*sp++; pc = (int *)*sp++; }
    else if (i == LI)  a = *(int *)a;
    else if (i == LC)  a = *(char *)a;
    else if (i == SI)  *(int *)*sp++ = a;
    else if (i == SC)  a = *(char *)*sp++ = a;
    else if (i == PSH) *--sp = a;

    else if (i == OR)  a = *sp++ |  a;
    else if (i == XOR) a = *sp++ ^  a;
    else if (i == AND) a = *sp++ &  a;
    else if (i == EQ)  a = *sp++ == a;
    else if (i == NE)  a = *sp++ != a;
    else if (i == LT)  a = *sp++ <  a;
    else if (i == GT)  a = *sp++ >  a;
    else if (i == LE)  a = *sp++ <= a;
    else if (i == GE)  a = *sp++ >= a;
    else if (i == SHL) a = *sp++ << a;
    else if (i == SHR) a = *sp++ >> a;
    else if (i == ADD) a = *sp++ +  a;
    else if (i == SUB) a = *sp++ -  a;
    else if (i == MUL) a = *sp++ *  a;
    else if (i == DIV) a = *sp++ /  a;
    else if (i == MOD) a = *sp++ %  a;

    else if (i == OPEN) a = open((char *)sp[1], *sp);
    else if (i == READ) a = read(sp[2], (char *)sp[1], *sp);
    else if (i == CLOS) a = close(*sp);
    else if (i == PRTF) { t = sp + pc[1]; a = printf((char *)t[-1], t[-2]); }
    else if (i == MALC) a = (int)malloc(*sp);
    else if (i == FREE) free((void *)*sp);
    else if (i == MSET) a = (int)memset((char *)sp[2], sp[1], *sp);
    else if (i == MCMP) a = memcmp((char *)sp[2], (char *)sp[1], *sp);
    else if (i == EXIT) { printf("exit(%d) cycle = %d\n", *sp, cycle); return *sp; }
    else { printf("unknown instruction = %d! cycle = %d\n", i, cycle); return -1; }
  }
}

int main() {
  int *pc, *bp, *sp, poolsz;
  int *power2, *loop, *exit_loop;

  poolsz = 256 * 1024;
  if (!(e = malloc(poolsz))) { printf("malloc error\n"); return -1; }
  if (!(sp = malloc(poolsz))) { printf("stack malloc error\n"); return -1; }

  memset(e, 0, poolsz);

  // 實作 power2 函數: int power2(int n) { int r=1; while(n>0){r*=2;n--;} return r; }
  power2 = e;
  *e++ = ENT; *e++ = 1;                  // 建立一個區域變數 r
  *e++ = IMM; *e++ = 1;
  *e++ = LLA; *e++ = 0;
  *e++ = SI;

  loop = e;                              // while (n > 0)
  *e++ = LLA; *e++ = 1;
  *e++ = LI;
  *e++ = IMM; *e++ = 0;
  *e++ = LE;
  *e++ = BZ; exit_loop = e; *e++ = 0;

  *e++ = LLA; *e++ = 0;                  // r = r * 2;
  *e++ = LI;
  *e++ = IMM; *e++ = 2;
  *e++ = MUL;
  *e++ = LLA; *e++ = 0;
  *e++ = SI;

  *e++ = LLA; *e++ = 1;                  // n = n - 1;
  *e++ = LI;
  *e++ = IMM; *e++ = 1;
  *e++ = SUB;
  *e++ = LLA; *e++ = 1;
  *e++ = SI;

  *e++ = JMP; *e++ = (int)loop;

  *exit_loop = (int)e;

  *e++ = LLA; *e++ = 0;                  // return r;
  *e++ = LI;
  *e++ = LEV;

  // main: printf("2^3 = %d\n", power2(3));
  pc = e;
  *e++ = ENT; *e++ = 0;
  *e++ = IMM; *e++ = (int)"2^3 = %d\n";
  *e++ = PSH;
  *e++ = IMM; *e++ = 3;
  *e++ = PSH;
  *e++ = JSR;  *e++ = (int)power2;
  *e++ = ADJ;  *e++ = 1;
  *e++ = PSH;
  *e++ = PRTF;
  *e++ = ADJ;  *e++ = 2;
  *e++ = LEV;

  bp = sp = (int *)((int)sp + poolsz);
  *--sp = EXIT;
  *--sp = PSH; int *t = sp;
  *--sp = (int)t;

  return run(pc, bp, sp);
}
