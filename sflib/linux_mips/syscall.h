
/* $Id$ */

/*
 * Modified from include/asm/unistd.h 
 * Copyright (C) 1995, 96, 97, 98, 99, 2000 by Ralf Baechle
 */


#define _sfreturnsyscall(type,res,ret)    return (type) (res); 

/*
int errno;
#define _sfreturnsyscall    if (__a3 == 0) return (type) __v0; \
		            errno = __v0; return -1; 
*/
				


#define _sfsyscall0(type,name) \
type name(void) \
{ \
	register unsigned long __v0 asm("$2") = __NR_##name; \
	register unsigned long __a3 asm("$7"); \
	\
	__asm__ volatile ( \
	".set\tnoreorder\n\t" \
	"li\t$2, %2\t\t\t# " #name "\n\t" \
	"syscall\n\t" \
	".set\treorder" \
	: "=&r" (__v0), "=r" (__a3) \
	: "i" (__NR_##name) \
	: "$8", "$9", "$10", "$11", "$12", "$13", "$14", "$15", "$24"); \
	\
	_sfreturnsyscall(type, __v0, __a3) \
}

/*
 * DANGER: This macro isn't usable for the pipe(2) call
 * which has a unusual return convention.
 */
#define _sfsyscall1(type,name,atype,a) \
type name(atype a) \
{ \
	register unsigned long __v0 asm("$2") = __NR_##name; \
	register unsigned long __a0 asm("$4") = (unsigned long) a; \
	register unsigned long __a3 asm("$7"); \
	\
	__asm__ volatile ( \
	".set\tnoreorder\n\t" \
	"li\t$2, %3\t\t\t# " #name "\n\t" \
	"syscall\n\t" \
	".set\treorder" \
	: "=&r" (__v0), "=r" (__a3) \
	: "r" (__a0), "i" (__NR_##name) \
	: "$8", "$9", "$10", "$11", "$12", "$13", "$14", "$15", "$24"); \
	\
	_sfreturnsyscall(type, __v0, __a3) \
}

#define _sfsyscall2(type,name,atype,a,btype,b) \
type name(atype a, btype b) \
{ \
	register unsigned long __v0 asm("$2") = __NR_##name; \
	register unsigned long __a0 asm("$4") = (unsigned long) a; \
	register unsigned long __a1 asm("$5") = (unsigned long) b; \
	register unsigned long __a3 asm("$7"); \
	\
	__asm__ volatile ( \
	".set\tnoreorder\n\t" \
	"li\t$2, %4\t\t\t# " #name "\n\t" \
	"syscall\n\t" \
	".set\treorder" \
	: "=&r" (__v0), "=r" (__a3) \
	: "r" (__a0), "r" (__a1), "i" (__NR_##name) \
	: "$8", "$9", "$10", "$11", "$12", "$13", "$14", "$15", "$24"); \
	\
	_sfreturnsyscall(type, __v0, __a3) \
}

#define _sfsyscall3(type,name,atype,a,btype,b,ctype,c) \
type name(atype a, btype b, ctype c) \
{ \
	register unsigned long __v0 asm("$2") = __NR_##name; \
	register unsigned long __a0 asm("$4") = (unsigned long) a; \
	register unsigned long __a1 asm("$5") = (unsigned long) b; \
	register unsigned long __a2 asm("$6") = (unsigned long) c; \
	register unsigned long __a3 asm("$7"); \
	\
	__asm__ volatile ( \
	".set\tnoreorder\n\t" \
	"li\t$2, %5\t\t\t# " #name "\n\t" \
	"syscall\n\t" \
	".set\treorder" \
	: "=&r" (__v0), "=r" (__a3) \
	: "r" (__a0), "r" (__a1), "r" (__a2), "i" (__NR_##name) \
	: "$8", "$9", "$10", "$11", "$12", "$13", "$14", "$15", "$24"); \
	\
	_sfreturnsyscall(type, __v0, __a3) \
}

#define _sfsyscall4(type,name,atype,a,btype,b,ctype,c,dtype,d) \
type name(atype a, btype b, ctype c, dtype d) \
{ \
	register unsigned long __v0 asm("$2") = __NR_##name; \
	register unsigned long __a0 asm("$4") = (unsigned long) a; \
	register unsigned long __a1 asm("$5") = (unsigned long) b; \
	register unsigned long __a2 asm("$6") = (unsigned long) c; \
	register unsigned long __a3 asm("$7") = (unsigned long) d; \
	\
	__asm__ volatile ( \
	".set\tnoreorder\n\t" \
	"li\t$2, %5\t\t\t# " #name "\n\t" \
	"syscall\n\t" \
	".set\treorder" \
	: "=&r" (__v0), "+r" (__a3) \
	: "r" (__a0), "r" (__a1), "r" (__a2), "i" (__NR_##name) \
	: "$8", "$9", "$10", "$11", "$12", "$13", "$14", "$15", "$24"); \
	\
	_sfreturnsyscall(type, __v0, __a3) \
}

/*
 * Using those means your brain needs more than an oil change ;-)
 */

#define _sfsyscall5(type,name,atype,a,btype,b,ctype,c,dtype,d,etype,e) \
type name(atype a, btype b, ctype c, dtype d, etype e) \
{ \
	register unsigned long __v0 asm("$2") = __NR_##name; \
	register unsigned long __a0 asm("$4") = (unsigned long) a; \
	register unsigned long __a1 asm("$5") = (unsigned long) b; \
	register unsigned long __a2 asm("$6") = (unsigned long) c; \
	register unsigned long __a3 asm("$7") = (unsigned long) d; \
	\
	__asm__ volatile ( \
	".set\tnoreorder\n\t" \
	"lw\t$2, %6\n\t" \
	"subu\t$29, 32\n\t" \
	"sw\t$2, 16($29)\n\t" \
	"li\t$2, %5\t\t\t# " #name "\n\t" \
	"syscall\n\t" \
	"addiu\t$29, 32\n\t" \
	".set\treorder" \
	: "=&r" (__v0), "+r" (__a3) \
	: "r" (__a0), "r" (__a1), "r" (__a2), "i" (__NR_##name), \
	  "m" ((unsigned long)e) \
	: "$8", "$9", "$10", "$11", "$12", "$13", "$14", "$15", "$24"); \
	\
	_sfreturnsyscall(type, __v0, __a3) \
}

#define _sfsyscall6(type,name,atype,a,btype,b,ctype,c,dtype,d,etype,e,ftype,f) \
type name(atype a, btype b, ctype c, dtype d, etype e, ftype f) \
{ \
	register unsigned long __v0 asm("$2") = __NR_##name; \
	register unsigned long __a0 asm("$4") = (unsigned long) a; \
	register unsigned long __a1 asm("$5") = (unsigned long) b; \
	register unsigned long __a2 asm("$6") = (unsigned long) c; \
	register unsigned long __a3 asm("$7") = (unsigned long) d; \
	\
	__asm__ volatile ( \
	".set\tnoreorder\n\t" \
	"lw\t$2, %6\n\t" \
	"lw\t$8, %7\n\t" \
	"subu\t$29, 32\n\t" \
	"sw\t$2, 16($29)\n\t" \
	"sw\t$8, 20($29)\n\t" \
	"li\t$2, %5\t\t\t# " #name "\n\t" \
	"syscall\n\t" \
	"addiu\t$29, 32\n\t" \
	".set\treorder" \
	: "=&r" (__v0), "+r" (__a3) \
	: "r" (__a0), "r" (__a1), "r" (__a2), "i" (__NR_##name), \
	  "m" ((unsigned long)e), "m" ((unsigned long)f) \
	: "$8", "$9", "$10", "$11", "$12", "$13", "$14", "$15", "$24"); \
	\
	_sfreturnsyscall(type, __v0, __a3) \
}

#define _sfsyscall7(type,name,atype,a,btype,b,ctype,c,dtype,d,etype,e,ftype,f,gtype,g) \
type name(atype a, btype b, ctype c, dtype d, etype e, ftype f, gtype g) \
{ \
	register unsigned long __v0 asm("$2") = __NR_##name; \
	register unsigned long __a0 asm("$4") = (unsigned long) a; \
	register unsigned long __a1 asm("$5") = (unsigned long) b; \
	register unsigned long __a2 asm("$6") = (unsigned long) c; \
	register unsigned long __a3 asm("$7") = (unsigned long) d; \
	\
	__asm__ volatile ( \
	".set\tnoreorder\n\t" \
	"lw\t$2, %6\n\t" \
	"lw\t$8, %7\n\t" \
	"lw\t$9, %8\n\t" \
	"subu\t$29, 32\n\t" \
	"sw\t$2, 16($29)\n\t" \
	"sw\t$8, 20($29)\n\t" \
	"sw\t$9, 24($29)\n\t" \
	"li\t$2, %5\t\t\t# " #name "\n\t" \
	"syscall\n\t" \
	"addiu\t$29, 32\n\t" \
	".set\treorder" \
	: "=&r" (__v0), "+r" (__a3) \
	: "r" (__a0), "r" (__a1), "r" (__a2), "i" (__NR_##name), \
	  "m" ((unsigned long)e), "m" ((unsigned long)f), \
	  "m" ((unsigned long)g), \
	: "$8", "$9", "$10", "$11", "$12", "$13", "$14", "$15", "$24"); \
	\
	_sfreturnsyscall(type, __v0, __a3) \
}

