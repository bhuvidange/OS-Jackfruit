#include <stdio.h>
#include <unistd.h>

int main() {
    while (1) {
        printf("I/O workload\n");
        fflush(stdout);
        usleep(10000);
    }
    return 0;
}