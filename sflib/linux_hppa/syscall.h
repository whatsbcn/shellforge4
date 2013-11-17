
/* $Id$ */

#define HPUX_GATEWAY_ADDR       0xC0000004
#define LINUX_GATEWAY_ADDR      0x100

#define SYS_ify(syscall_name)   __NR_##syscall_name

/* The system call number MUST ALWAYS be loaded in the delay slot of
   the ble instruction, or restarting system calls WILL NOT WORK.  See
   arch/parisc/kernel/signal.c - dhd, 2000-07-26 */
#define K_INLINE_SYSCALL(name, nr, args...)       ({              \
        unsigned long __sys_res;                                \
        {                                                       \
                register unsigned long __res asm("r28");        \
                K_LOAD_ARGS_##nr(args)                            \
                asm volatile(                                   \
                        "ble  0x100(%%sr2, %%r0)\n\t"           \
                        " ldi %1, %%r20"                        \
                        : "=r" (__res)                          \
                        : "i" (SYS_ify(name)) K_ASM_ARGS_##nr   \
                          );                                    \
                __sys_res = __res;                              \
        }                                                       \
        __sys_res;                                              \
})

#define K_LOAD_ARGS_0()
#define K_LOAD_ARGS_1(r26)                                        \
        register unsigned long __r26 __asm__("r26") = (unsigned long)r26;       \
        K_LOAD_ARGS_0()
#define K_LOAD_ARGS_2(r26,r25)                                    \
        register unsigned long __r25 __asm__("r25") = (unsigned long)r25;       \
        K_LOAD_ARGS_1(r26)
#define K_LOAD_ARGS_3(r26,r25,r24)                                \
        register unsigned long __r24 __asm__("r24") = (unsigned long)r24;       \
        K_LOAD_ARGS_2(r26,r25)
#define K_LOAD_ARGS_4(r26,r25,r24,r23)                            \
        register unsigned long __r23 __asm__("r23") = (unsigned long)r23;       \
        K_LOAD_ARGS_3(r26,r25,r24)
#define K_LOAD_ARGS_5(r26,r25,r24,r23,r22)                        \
        register unsigned long __r22 __asm__("r22") = (unsigned long)r22;       \
        K_LOAD_ARGS_4(r26,r25,r24,r23)
#define K_LOAD_ARGS_6(r26,r25,r24,r23,r22,r21)                    \
        register unsigned long __r21 __asm__("r21") = (unsigned long)r21;       \
        K_LOAD_ARGS_5(r26,r25,r24,r23,r22)

#define K_ASM_ARGS_0
#define K_ASM_ARGS_1 , "r" (__r26)
#define K_ASM_ARGS_2 , "r" (__r26), "r" (__r25)
#define K_ASM_ARGS_3 , "r" (__r26), "r" (__r25), "r" (__r24)
#define K_ASM_ARGS_4 , "r" (__r26), "r" (__r25), "r" (__r24), "r" (__r23)
#define K_ASM_ARGS_5 , "r" (__r26), "r" (__r25), "r" (__r24), "r" (__r23), "r" (__r22)
#define K_ASM_ARGS_6 , "r" (__r26), "r" (__r25), "r" (__r24), "r" (__r23), "r" (__r22), "r" (__r21)

#define _sfsyscall0(type,name)                                                  \
type name(void)                                                               \
{                                                                             \
    return (type)K_INLINE_SYSCALL(name, 0);   \
}

#define _sfsyscall1(type,name,type1,arg1)                                       \
type name(type1 arg1)                                                         \
{                                                                             \
    return (type)K_INLINE_SYSCALL(name, 1, arg1);     \
}

#define _sfsyscall2(type,name,type1,arg1,type2,arg2)                            \
type name(type1 arg1, type2 arg2)                                             \
{                                                                             \
    return (type)K_INLINE_SYSCALL(name, 2, arg1, arg2);       \
}

#define _sfsyscall3(type,name,type1,arg1,type2,arg2,type3,arg3)                 \
type name(type1 arg1, type2 arg2, type3 arg3)                                 \
{                                                                             \
    return (type)K_INLINE_SYSCALL(name, 3, arg1, arg2, arg3); \
}

#define _sfsyscall4(type,name,type1,arg1,type2,arg2,type3,arg3,type4,arg4)      \
type name(type1 arg1, type2 arg2, type3 arg3, type4 arg4)                     \
{                                                                             \
    return (type)K_INLINE_SYSCALL(name, 4, arg1, arg2, arg3, arg4);   \
}

/* select takes 5 arguments */
#define _sfsyscall5(type,name,type1,arg1,type2,arg2,type3,arg3,type4,arg4,type5,arg5) \
type name(type1 arg1, type2 arg2, type3 arg3, type4 arg4, type5 arg5)   \
{                                                                       \
    return (type)K_INLINE_SYSCALL(name, 5, arg1, arg2, arg3, arg4, arg5);     \
}


/* mmap & mmap2 take 6 arguments */

#define _sfsyscall6(type,name,type1,arg1,type2,arg2,type3,arg3,type4,arg4,type5,arg5,type6,arg6) \
type name(type1 arg1, type2 arg2, type3 arg3, type4 arg4, type5 arg5, type6 arg6) \
{                                                                       \
    return (type)K_INLINE_SYSCALL(name, 6, arg1, arg2, arg3, arg4, arg5, arg6);       \
}




