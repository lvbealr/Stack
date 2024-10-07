#include <cstdio>

void printBin(int x) {
    if (x > 1) {
        printBin(x / 2);
    }

    printf("%d", x % 2);
}

int main() {
    int x = 34;
    printBin(x);
}