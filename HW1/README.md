# Compiler

## 語法

```
PROG = STMTS
BLOCK = { STMTS }
STMTS = STMT*
STMT = WHILE | BLOCK | ASSIGN
WHILE = while (E) STMT
ASSIGN = id '=' E;
E = F (op E)*
F = (E) | Number | Id
```

## 執行結果

```
========== lex ==============
token=while
token=(
token=i
token=<
token=10
token=)
token=i
token==
token=i
token=+
token=1
token=;
========== dump ==============
0:while
1:(
2:i
3:<
4:10
5:)
6:i
7:=
8:i
9:+
10:1
11:;
============ parse =============
(L0)
t0 = i
t1 = 10
t2 = t0 < t1
if not T2 goto L1
t3 = i
t4 = 1
t5 = t3 + t4
i = t5
goto L0
(L1)

```
