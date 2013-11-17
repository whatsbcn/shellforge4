
/* $Id$ */

/*
 * From linux/include/asm-arm/unistd.h
 * Copyright (C) 2001-2002 Russell King
 */    


#define __sys2(x) #x
#define __sys1(x) __sys2(x)

#ifndef __sfsyscall
#if defined(__thumb__)
#define __sfsyscall(name)			        \
	"push	{r7}\n\t"				\
	"mov	r7, #" __sys1(__NR_##name) "\n\t"	\
	"swi	0\n\t"					\
	"pop	{r7}"
#else
#define __sfsyscall(name) "swi\t" __sys1(__NR_##name) "\n\t"
#endif
#endif

#define __sfsyscall_return(type, res)					\
do {									\
	return (type) (res);						\
} while (0)

#define _sfsyscall0(type,name)						\
type name(void) {							\
  register long __res __asm__("r0");					\
  __asm__ __volatile__ (						\
  __sfsyscall(name)							\
	:"=r" (__res)							\
	:								\
	: "lr");							\
  __sfsyscall_return(type,__res); 					\
}

#define _sfsyscall1(type,name,type1,arg1) 				\
type name(type1 arg1) { 						\
  register long __r0 __asm__("r0") = (long)arg1;			\
  register long __res __asm__("r0");					\
  __asm__ __volatile__ (						\
  __sfsyscall(name)							\
	: "=r" (__res)							\
	: "r" (__r0)							\
	: "lr");							\
  __sfsyscall_return(type,__res); 					\
}

#define _sfsyscall2(type,name,type1,arg1,type2,arg2)			\
type name(type1 arg1,type2 arg2) {					\
  register long __r0 __asm__("r0") = (long)arg1;			\
  register long __r1 __asm__("r1") = (long)arg2;			\
  register long __res __asm__("r0");					\
  __asm__ __volatile__ (						\
  __sfsyscall(name)							\
	: "=r" (__res)							\
	: "r" (__r0),"r" (__r1) 					\
	: "lr");							\
  __sfsyscall_return(type,__res); 					\
}


#define _sfsyscall3(type,name,type1,arg1,type2,arg2,type3,arg3)		\
type name(type1 arg1,type2 arg2,type3 arg3) {				\
  register long __r0 __asm__("r0") = (long)arg1;			\
  register long __r1 __asm__("r1") = (long)arg2;			\
  register long __r2 __asm__("r2") = (long)arg3;			\
  register long __res __asm__("r0");					\
  __asm__ __volatile__ (						\
  __sfsyscall(name)							\
	: "=r" (__res)							\
	: "r" (__r0),"r" (__r1),"r" (__r2)				\
	: "lr");							\
  __sfsyscall_return(type,__res); 					\
}


#define _sfsyscall4(type,name,type1,arg1,type2,arg2,type3,arg3,type4,arg4)\
type name(type1 arg1, type2 arg2, type3 arg3, type4 arg4) {		\
  register long __r0 __asm__("r0") = (long)arg1;			\
  register long __r1 __asm__("r1") = (long)arg2;			\
  register long __r2 __asm__("r2") = (long)arg3;			\
  register long __r3 __asm__("r3") = (long)arg4;			\
  register long __res __asm__("r0");					\
  __asm__ __volatile__ (						\
  __sfsyscall(name)							\
	: "=r" (__res)							\
	: "r" (__r0),"r" (__r1),"r" (__r2),"r" (__r3)			\
	: "lr");							\
  __sfsyscall_return(type,__res); 					\
}
  

#define _sfsyscall5(type,name,type1,arg1,type2,arg2,type3,arg3,type4,arg4,type5,arg5)	\
type name(type1 arg1, type2 arg2, type3 arg3, type4 arg4, type5 arg5) { \
  register long __r0 __asm__("r0") = (long)arg1;			\
  register long __r1 __asm__("r1") = (long)arg2;			\
  register long __r2 __asm__("r2") = (long)arg3;			\
  register long __r3 __asm__("r3") = (long)arg4;			\
  register long __r4 __asm__("r4") = (long)arg5;			\
  register long __res __asm__("r0");					\
  __asm__ __volatile__ (						\
  __sfsyscall(name)							\
	: "=r" (__res)							\
	: "r" (__r0),"r" (__r1),"r" (__r2),"r" (__r3),"r" (__r4)	\
	: "lr");							\
  __sfsyscall_return(type,__res); 					\
}

#define _sfsyscall6(type,name,type1,arg1,type2,arg2,type3,arg3,type4,arg4,type5,arg5,type6,arg6)	\
type name(type1 arg1, type2 arg2, type3 arg3, type4 arg4, type5 arg5, type6 arg6) {	\
  register long __r0 __asm__("r0") = (long)arg1;			\
  register long __r1 __asm__("r1") = (long)arg2;			\
  register long __r2 __asm__("r2") = (long)arg3;			\
  register long __r3 __asm__("r3") = (long)arg4;			\
  register long __r4 __asm__("r4") = (long)arg5;			\
  register long __r5 __asm__("r5") = (long)arg6;			\
  register long __res __asm__("r0");					\
  __asm__ __volatile__ (						\
  __sfsyscall(name)							\
	: "=r" (__res)							\
	: "r" (__r0),"r" (__r1),"r" (__r2),"r" (__r3),"r" (__r4),"r" (__r5)		\
	: "lr");							\
  __sfsyscall_return(type,__res); 					\
}

