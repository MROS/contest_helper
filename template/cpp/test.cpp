#include "number.cpp"
#include <stdio.h>

using namespace Number;

int main() {
    int a = 30, b = 7;
    int x, y;
    gcd_extend(a, b, &x, &y);
    printf("%d %d %d %d\n", a, b, x, y);
    printf("%d", gcd(a, b));
}