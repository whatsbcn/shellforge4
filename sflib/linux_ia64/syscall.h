
/* $Id$ */

#ifdef SF_USE_ERRNO

static int errno=1234;

#define __sfsyscall_return(type, res, retval) \
do { \
        if ((unsigned long)(res) == -1 \
                errno = -(retval); \
                retval = -1; \
        } \
        return (type) (retval); \
} while (0)

#else /* SF_USE_ERRNO */

#define __sfsyscall_return(type, res, retval) \
do { \
	return (type) (retval); \
} while (0)

#endif /* SF_USE_ERRNO */



#define __BREAK_SYSCALL 0x100000

#define ASM_ARGS_0
#define ASM_ARGS_1      ASM_ARGS_0, "r" (_out0)
#define ASM_ARGS_2      ASM_ARGS_1, "r" (_out1)
#define ASM_ARGS_3      ASM_ARGS_2, "r" (_out2)
#define ASM_ARGS_4      ASM_ARGS_3, "r" (_out3)
#define ASM_ARGS_5      ASM_ARGS_4, "r" (_out4)

#define ASM_CLOBBERS_0  ASM_CLOBBERS_1, "out0"
#define ASM_CLOBBERS_1  ASM_CLOBBERS_2, "out1"
#define ASM_CLOBBERS_2  ASM_CLOBBERS_3, "out2"
#define ASM_CLOBBERS_3  ASM_CLOBBERS_4, "out3"
#define ASM_CLOBBERS_4  ASM_CLOBBERS_5, "out4"
#define ASM_CLOBBERS_5  , "out5", "out6", "out7"                        \

#define LOAD_ARGS_0()   do { } while (0)
#define LOAD_ARGS_1(out0)                               \
  register long _out0 asm ("out0") = (long) (out0);     \
  LOAD_ARGS_0 ()
#define LOAD_ARGS_2(out0, out1)                         \
  register long _out1 asm ("out1") = (long) (out1);     \
  LOAD_ARGS_1 (out0)
#define LOAD_ARGS_3(out0, out1, out2)                   \
  register long _out2 asm ("out2") = (long) (out2);     \
  LOAD_ARGS_2 (out0, out1)
#define LOAD_ARGS_4(out0, out1, out2, out3)             \
  register long _out3 asm ("out3") = (long) (out3);     \
  LOAD_ARGS_3 (out0, out1, out2)
#define LOAD_ARGS_5(out0, out1, out2, out3, out4)       \
  register long _out4 asm ("out4") = (long) (out4);     \
  LOAD_ARGS_4 (out0, out1, out2, out3)


#define INLINE_SYSCALL(type, name, nr, args...)                 \
    register long _r8 asm ("r8");                               \
    register long _r10 asm ("r10");                             \
    register long _r15 asm ("r15") = __NR_##name;               \
    LOAD_ARGS_##nr (args);                                      \
    __asm __volatile ("break %3;\n\t"                          \
                      : "=r" (_r8), "=r" (_r10), "=r" (_r15)    \
                      : "i" (__BREAK_SYSCALL), "2" (_r15)       \
                        ASM_ARGS_##nr                           \
                      : "memory" ASM_CLOBBERS_##nr);            \
   __sfsyscall_return(type, _r10, _r8);
   



#define _sfsyscall0(type, name)  \
type name(void)                   \
{ INLINE_SYSCALL(type, name, 0)  }

#define _sfsyscall1(type, name, t1, a1)  \
type name(t1 a1)                          \
{ INLINE_SYSCALL(type, name, 1, (long)a1)  }

#define _sfsyscall2(type, name, t1, a1, t2, a2)  \
type name(t1 a1, t2 a2)                           \
{ INLINE_SYSCALL(type, name, 2, (long)a1, (long)a2)  }

#define _sfsyscall3(type, name, t1, a1, t2, a2, t3, a3)  \
type name(t1 a1, t2 a2, t3 a3)                            \
{ INLINE_SYSCALL(type, name, 3, (long)a1, (long)a2, (long)a3)  }

#define _sfsyscall4(type, name, t1, a1, t2, a2, t3, a3, t4, a4)  \
type name(t1 a1, t2 a2, t3 a3, t4 a4)                             \
{ INLINE_SYSCALL(type, name, 4, (long)a1, (long)a2, (long)a3, (long)a4)  }

#define _sfsyscall5(type, name, t1, a1, t2, a2, t3, a3, t4, a4, t5, a5)  \
type name(t1 a1, t2 a2, t3 a3, t4 a4, t5 a5)                              \
{ INLINE_SYSCALL(type, name, 5, (long)a1, (long)a2, (long)a3, (long)a4, (long)a5)  }

#define _sfsyscall6(type, name, t1, a1, t2, a2, t3, a3, t4, a4, t5, a5, t6, a6) void  ___nothing(void); 


