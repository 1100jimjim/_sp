## power.c

執行power.c
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

產生組合語言.s檔
```sh

```


產生目的檔.o檔

將該目的檔反組譯

印出該目的檔的表頭

