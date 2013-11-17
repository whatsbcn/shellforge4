/* $Id$ */

/* From unistd.h */


#ifdef SF_USE_ERRNO

/* user-visible error numbers are in the range -1 - -122: see <asm-s390/errno.h> */

#define __syscall_return(type, res)                          \
do {                                                         \
        if ((unsigned long)(res) >= (unsigned long)(-125)) { \
                errno = -(res);                              \
                res = -1;                                    \
        }                                                    \
        return (type) (res);                                 \
} while (0)

#else 

#define _svc_clobber "2", "cc", "memory"

#define _syscall0(type,name)                                 \
type name(void) {                                            \
        long __res;                                          \
        __asm__ __volatile__ (                               \
                "    svc %b1\n"                              \
                "    lr  %0,2"                               \
                : "=d" (__res)                               \
                : "i" (__NR_##name)                          \
                : _svc_clobber );                            \
        __syscall_return(type,__res);                        \
}

#define _syscall1(type,name,type1,arg1)                      \
type name(type1 arg1) {                                      \
        register type1 __arg1 asm("2") = arg1;               \
        long __res;                                          \
        __asm__ __volatile__ (                               \
                "    svc %b1\n"                              \
                "    lr  %0,2"                               \
                : "=d" (__res)                               \
                : "i" (__NR_##name),                         \
                  "d" (__arg1)                               \
                : _svc_clobber );                            \
        __syscall_return(type,__res);                        \
}

#define _syscall2(type,name,type1,arg1,type2,arg2)           \
type name(type1 arg1, type2 arg2) {                          \
        register type1 __arg1 asm("2") = arg1;               \
        register type2 __arg2 asm("3") = arg2;               \
        long __res;                                          \
        __asm__ __volatile__ (                               \
                "    svc %b1\n"                              \
                "    lr  %0,2"                               \
                : "=d" (__res)                               \
                : "i" (__NR_##name),                         \
                  "d" (__arg1),                              \
                  "d" (__arg2)                               \
                : _svc_clobber );                            \
        __syscall_return(type,__res);                        \
}

#define _syscall3(type,name,type1,arg1,type2,arg2,type3,arg3)\
type name(type1 arg1, type2 arg2, type3 arg3) {              \
        register type1 __arg1 asm("2") = arg1;               \
        register type2 __arg2 asm("3") = arg2;               \
        register type3 __arg3 asm("4") = arg3;               \
        long __res;                                          \
        __asm__ __volatile__ (                               \
                "    svc %b1\n"                              \
                "    lr  %0,2"                               \
                : "=d" (__res)                               \
                : "i" (__NR_##name),                         \
                  "d" (__arg1),                              \
                  "d" (__arg2),                              \
                  "d" (__arg3)                               \
                : _svc_clobber );                            \
        __syscall_return(type,__res);                        \
}

#define _syscall4(type,name,type1,arg1,type2,arg2,type3,arg3,\
                  type4,name4)                               \
type name(type1 arg1, type2 arg2, type3 arg3, type4 arg4) {  \
        register type1 __arg1 asm("2") = arg1;               \
        register type2 __arg2 asm("3") = arg2;               \
        register type3 __arg3 asm("4") = arg3;               \
        register type4 __arg4 asm("5") = arg4;               \
        long __res;                                          \
        __asm__ __volatile__ (                               \
                "    svc %b1\n"                              \
                "    lr  %0,2"                               \
                : "=d" (__res)                               \
                : "i" (__NR_##name),                         \
                  "d" (__arg1),                              \
                  "d" (__arg2),                              \
                  "d" (__arg3),                              \
                  "d" (__arg4)                               \
                : _svc_clobber );                            \
        __syscall_return(type,__res);                        \
}

#define _syscall5(type,name,type1,arg1,type2,arg2,type3,arg3,\
                  type4,name4,type5,name5)                   \
type name(type1 arg1, type2 arg2, type3 arg3, type4 arg4,    \
          type5 arg5) {                                      \
        register type1 __arg1 asm("2") = arg1;               \
        register type2 __arg2 asm("3") = arg2;               \
        register type3 __arg3 asm("4") = arg3;               \
        register type4 __arg4 asm("5") = arg4;               \
        register type5 __arg5 asm("6") = arg5;               \
        long __res;                                          \
        __asm__ __volatile__ (                               \
                "    svc %b1\n"                              \
                "    lr  %0,2"                               \
                : "=d" (__res)                               \
                : "i" (__NR_##name),                         \
                  "d" (__arg1),                              \
                  "d" (__arg2),                              \
                  "d" (__arg3),                              \
                  "d" (__arg4),                              \
                  "d" (__arg5)                               \
                : _svc_clobber );                            \
        __syscall_return(type,__res);                        \
}

#ifdef __KERNEL_SYSCALLS__

/*
 * we need this inline - forking from kernel space will result
 * in NO COPY ON WRITE (!!!), until an execve is executed. This
 * is no problem, but for the stack. This is handled by not letting
 * main() use the stack at all after fork(). Thus, no function
 * calls - which means inline code for fork too, as otherwise we
 * would use the stack upon exit from 'fork()'.
 *
 * Actually only pause and fork are needed inline, so that there
 * won't be any messing with the stack from main(), but we define
 * some others too.
 */
#define __NR__exit __NR_exit
static inline _syscall0(int,idle)
static inline _syscall0(int,pause)
static inline _syscall0(int,sync)
static inline _syscall0(pid_t,setsid)
static inline _syscall3(int,write,int,fd,const char *,buf,off_t,count)
static inline _syscall3(int,read,int,fd,char *,buf,off_t,count)
static inline _syscall3(off_t,lseek,int,fd,off_t,offset,int,count)
static inline _syscall1(int,dup,int,fd)
static inline _syscall3(int,execve,const char *,file,char **,argv,char **,envp)
static inline _syscall3(int,open,const char *,file,int,flag,int,mode)
static inline _syscall1(int,close,int,fd)
static inline _syscall1(int,_exit,int,exitcode)
static inline _syscall1(int,delete_module,const char *,name)
static inline _syscall2(long,stat,char *,filename,struct stat *,statbuf)

static inline pid_t waitpid(int pid, int * wait_stat, int flags)
{
        return sys_wait4(pid, wait_stat, flags, NULL);
}

static inline pid_t wait(int * wait_stat)
{
        return waitpid(-1,wait_stat,0);
}

#endif

#endif /* _ASM_S390_UNISTD_H_ */
