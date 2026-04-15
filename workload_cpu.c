#include <stdio.h>

int main() {
    volatile long i = 0;
    while (1) {
        i++;
    }
    return 0;
}