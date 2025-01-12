#ifndef __SETJMP_H
#define __SETJMP_H
#ifndef __TYPES_H
#include <types.h>
#endif

// RISC-V 32

typedef uint32_t jmp_buf[19];
extern int setjmp(jmp_buf __env);
__attribute__((__noreturn__)) void longjmp (jmp_buf __env, int __val);

extern void longjmp(jmp_buf __env, int __rv);

#endif
