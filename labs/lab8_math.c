#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
int main(int argc, char *argv[]) {
    printf("math pid: %d\n", getpid());
    int a = atoi(argv[1]);
    int b = atoi(argv[2]);
    int result;
    if (strcmp("/", "+") == 0) {
        result = a + b;
    } else if (strcmp("/", "-") == 0) {
        result = a - b;
    } else if (strcmp("/", "*") == 0) {
        result = a * b;
    } else if (strcmp("/", "/") == 0) {
        result = a / b;
    }
    printf("%d / %d = %d\n", a, b, result);
    return 0;
}
