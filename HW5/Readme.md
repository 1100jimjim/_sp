## power.c

執行power.c
```
gcc power.c
./a.out
```

```c
#include <stdio.h>

int power(int base, int exp)
{
    int result = 1;
    for (int i = 0; i < exp; i++) {
        result *= base;
    }
    return result;
}

int main()
{
    printf("%d\n", power(2, 3));
    return 0;
}
```

產生組合語言.s
```
gcc -S power.c -o power.s
```

產生目的檔.o
```
gcc -c power.c -o power.o
```

反組譯目的檔
```
objdump -d power.o
```

```sh
power.o:     file format elf64-x86-64


Disassembly of section .text:

0000000000000000 <power>:
   0:   f3 0f 1e fa             endbr64
   4:   55                      push   %rbp
   5:   48 89 e5                mov    %rsp,%rbp
   8:   89 7d ec                mov    %edi,-0x14(%rbp)
   b:   89 75 e8                mov    %esi,-0x18(%rbp)
   e:   c7 45 f8 01 00 00 00    movl   $0x1,-0x8(%rbp)
  15:   c7 45 fc 00 00 00 00    movl   $0x0,-0x4(%rbp)
  1c:   eb 0e                   jmp    2c <power+0x2c>
  1e:   8b 45 f8                mov    -0x8(%rbp),%eax
  21:   0f af 45 ec             imul   -0x14(%rbp),%eax
  25:   89 45 f8                mov    %eax,-0x8(%rbp)
  28:   83 45 fc 01             addl   $0x1,-0x4(%rbp)
  2c:   8b 45 fc                mov    -0x4(%rbp),%eax
  2f:   3b 45 e8                cmp    -0x18(%rbp),%eax
  32:   7c ea                   jl     1e <power+0x1e>
  34:   8b 45 f8                mov    -0x8(%rbp),%eax
  37:   5d                      pop    %rbp
  38:   c3                      ret

0000000000000039 <main>:
  39:   f3 0f 1e fa             endbr64
  3d:   55                      push   %rbp
  3e:   48 89 e5                mov    %rsp,%rbp
  41:   be 03 00 00 00          mov    $0x3,%esi
  46:   bf 02 00 00 00          mov    $0x2,%edi
  4b:   e8 00 00 00 00          call   50 <main+0x17>
  50:   89 c6                   mov    %eax,%esi
  52:   48 8d 05 00 00 00 00    lea    0x0(%rip),%rax        # 59 <main+0x20>
  59:   48 89 c7                mov    %rax,%rdi
  5c:   b8 00 00 00 00          mov    $0x0,%eax
  61:   e8 00 00 00 00          call   66 <main+0x2d>
  66:   b8 00 00 00 00          mov    $0x0,%eax
  6b:   5d                      pop    %rbp
  6c:   c3                      ret
```

印出目的檔表頭
```
 objdump -h power.o
```

```sh
power.o:     file format elf64-x86-64

Sections:
Idx Name          Size      VMA               LMA               File off  Algn
  0 .text         0000006d  0000000000000000  0000000000000000  00000040  2**0
                  CONTENTS, ALLOC, LOAD, RELOC, READONLY, CODE
  1 .data         00000000  0000000000000000  0000000000000000  000000ad  2**0
                  CONTENTS, ALLOC, LOAD, DATA
  2 .bss          00000000  0000000000000000  0000000000000000  000000ad  2**0
                  ALLOC
  3 .rodata       00000004  0000000000000000  0000000000000000  000000ad  2**0
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
  4 .comment      0000002c  0000000000000000  0000000000000000  000000b1  2**0
                  CONTENTS, READONLY
  5 .note.GNU-stack 00000000  0000000000000000  0000000000000000  000000dd  2**0
                  CONTENTS, READONLY
  6 .note.gnu.property 00000020  0000000000000000  0000000000000000  000000e0  2**3
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
  7 .eh_frame     00000058  0000000000000000  0000000000000000  00000100  2**3
                  CONTENTS, ALLOC, LOAD, RELOC, READONLY, DATA
```

