#include "general.h"
#include <stdio.h>

typedef long long stack_elem_t;
#include "include_stack.h"

int main() {
    unsigned long long last_err = 0;

    log_init(NULL, &last_err);
    stack_t stk = {};
    STACK_INIT(&stk, 0, &last_err);

    stack_push(&stk, 14, &last_err);
    DUMP(&stk)

    printf("Hello, World");
}