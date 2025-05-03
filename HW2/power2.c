#include <stdio.h>

int power2(int n) {
    return 1 << n; 
}

int main() {
    int result = power2(3);
    printf("2^3 = %d\n", result);
    return 0;
}
