
/* $Id$ */

/*
 * From unistd.h
 */

#ifdef SF_USE_ERRNO
/* user-visible error numbers are in the range -1 - -122: see
   <asm-m68k/errno.h> */

#define __syscall_return(type, res) \
do { \
        if ((unsigned long)(res) >= (unsigned long)(-125)) { \
        /* avoid using res which is declared to be in register d0; \
           errno might expand to a function call and clobber it.  */ \
                int __err = -(res); \
                errno = __err; \
                res = -1; \
        } \
        return (type) (res); \
} while (0)
#else
#define __syscall_return(type, res) return (type) (res)
#endif /* SF_USE_ERRNO */

#define _syscall0(type,name) \
type name(void) \
{ \
register long __res __asm__ ("%d0") = __NR_##name; \
__asm__ __volatile__ ("trap  #0" \
                      : "+d" (__res) ); \
__syscall_return(type,__res); \
}

#define _syscall1(type,name,atype,a) \
type name(atype a) \
{ \
register long __res __asm__ ("%d0") = __NR_##name; \
register long __a __asm__ ("%d1") = (long)(a); \
__asm__ __volatile__ ("trap  #0" \
                      : "+d" (__res) \
                      : "d" (__a)  ); \
__syscall_return(type,__res); \
}

#define _syscall2(type,name,atype,a,btype,b) \
type name(atype a,btype b) \
{ \
register long __res __asm__ ("%d0") = __NR_##name; \
register long __a __asm__ ("%d1") = (long)(a); \
register long __b __asm__ ("%d2") = (long)(b); \
__asm__ __volatile__ ("trap  #0" \
                      : "+d" (__res) \
                      : "d" (__a), "d" (__b) \
                     ); \
__syscall_return(type,__res); \
}

#define _syscall3(type,name,atype,a,btype,b,ctype,c) \
type name(atype a,btype b,ctype c) \
{ \
register long __res __asm__ ("%d0") = __NR_##name; \
register long __a __asm__ ("%d1") = (long)(a); \
register long __b __asm__ ("%d2") = (long)(b); \
register long __c __asm__ ("%d3") = (long)(c); \
__asm__ __volatile__ ("trap  #0" \
                      : "+d" (__res) \
                      : "d" (__a), "d" (__b), \
                        "d" (__c) \
                     ); \
__syscall_return(type,__res); \
}

#define _syscall4(type,name,atype,a,btype,b,ctype,c,dtype,d) \
type name (atype a, btype b, ctype c, dtype d) \
{ \
register long __res __asm__ ("%d0") = __NR_##name; \
register long __a __asm__ ("%d1") = (long)(a); \
register long __b __asm__ ("%d2") = (long)(b); \
register long __c __asm__ ("%d3") = (long)(c); \
register long __d __asm__ ("%d4") = (long)(d); \
__asm__ __volatile__ ("trap  #0" \
                      : "+d" (__res) \
                      : "d" (__a), "d" (__b), \
                        "d" (__c), "d" (__d)  \
                     ); \
__syscall_return(type,__res); \
}

#define _syscall5(type,name,atype,a,btype,b,ctype,c,dtype,d,etype,e) \
type name (atype a,btype b,ctype c,dtype d,etype e) \
{ \
register long __res __asm__ ("%d0") = __NR_##name; \
register long __a __asm__ ("%d1") = (long)(a); \
register long __b __asm__ ("%d2") = (long)(b); \
register long __c __asm__ ("%d3") = (long)(c); \
register long __d __asm__ ("%d4") = (long)(d); \
register long __e __asm__ ("%d5") = (long)(e); \
__asm__ __volatile__ ("trap  #0" \
                      : "+d" (__res) \
                      : "d" (__a), "d" (__b), \
                        "d" (__c), "d" (__d), "d" (__e)  \
                     ); \
__syscall_return(type,__res); \
}
