#ifndef PROC_ERR_H
#define PROC_ERR_H

enum proc_err {
    PROC_ERR_OK              = 0ull,
    PROC_ERR_SYNTAX          = 1ull << 1,
    PROC_ERR_STACK_UNDERFLOW = 1ull << 2,
    PROC_ERR_STACK_OVERRFLOW = 1ull << 3,
    PROC_ERR_FILE_OPEN       = 1ull << 4,
};

void proc_add_err(proc_err *src, proc_err add);

#endif // PROC_ERR_H

