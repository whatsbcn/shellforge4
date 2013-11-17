/* $Id$ */

/*	Copyright (c) 1984, 1986, 1987, 1988, 1989 AT&T	
 *	  All Rights Reserved  	
 *
 * Copyright (c) 1991-1999 by Sun Microsystems, Inc.
 * All rights reserved.
 */



#define	__NR_syscall	0
#define	__NR_exit	1
#define	__NR_fork	2
#define	__NR_read	3
#define	__NR_write	4
#define	__NR_open	5
#define	__NR_close	6
#define	__NR_wait	7
#define	__NR_creat	8
#define	__NR_link	9
#define	__NR_unlink	10
#define	__NR_exec	11
#define	__NR_chdir	12
#define	__NR_time	13
#define	__NR_mknod	14
#define	__NR_chmod	15
#define	__NR_chown	16
#define	__NR_brk		17
#define	__NR_stat	18
#define	__NR_lseek	19
#define	__NR_getpid	20
#define	__NR_mount	21
#define	__NR_umount	22
#define	__NR_setuid	23
#define	__NR_getuid	24
#define	__NR_stime	25
#define	__NR_pcsample	26
#define	__NR_alarm	27
#define	__NR_fstat	28
#define	__NR_pause	29
#define	__NR_utime	30
#define	__NR_stty	31
#define	__NR_gtty	32
#define	__NR_access	33
#define	__NR_nice	34
#define	__NR_statfs	35
#define	__NR_sync	36
#define	__NR_kill	37
#define	__NR_fstatfs	38
#define	__NR_pgrpsys	39
	/*
	 * subcodes:
	 *	getpgrp()	  :: syscall(39,0)
	 *	setpgrp()	  :: syscall(39,1)
	 *	getsid(pid)	  :: syscall(39,2,pid)
	 *	setsid()	  :: syscall(39,3)
	 *	getpgid(pid)	  :: syscall(39,4,pid)
	 *	setpgid(pid,pgid) :: syscall(39,5,pid,pgid)
	 */
#define	__NR_xenix	40
	/*
	 * subcodes:
	 *	syscall(40, code, ...)
	 */
#define	__NR_dup		41
#define	__NR_pipe	42
#define	__NR_times	43
#define	__NR_profil	44
#define	__NR_plock	45
#define	__NR_setgid	46
#define	__NR_getgid	47
#define	__NR_signal	48
	/*
	 * subcodes:
	 *	signal(sig, f) :: signal(sig, f)    ((sig&SIGNO_MASK) == sig)
	 *	sigset(sig, f) :: signal(sig|SIGDEFER, f)
	 *	sighold(sig)   :: signal(sig|SIGHOLD)
	 *	sigrelse(sig)  :: signal(sig|SIGRELSE)
	 *	sigignore(sig) :: signal(sig|SIGIGNORE)
	 *	sigpause(sig)  :: signal(sig|SIGPAUSE)
	 *	see <sys/signal.h>
	 */
#define	__NR_msgsys	49
	/*
	 * subcodes:
	 *	msgget(...) :: msgsys(0, ...)
	 *	msgctl(...) :: msgsys(1, ...)
	 *	msgrcv(...) :: msgsys(2, ...)
	 *	msgsnd(...) :: msgsys(3, ...)
	 *	see <sys/msg.h>
	 */
#define	__NR_syssun	50
#define	__NR_sysi86	50
	/*
	 * subcodes:
	 *	syssun(code, ...)
	 *	see <sys/sys3b.h>
	 */
#define	__NR_acct	51
#define	__NR_shmsys	52
	/*
	 * subcodes:
	 *	shmat (...) :: shmsys(0, ...)
	 *	shmctl(...) :: shmsys(1, ...)
	 *	shmdt (...) :: shmsys(2, ...)
	 *	shmget(...) :: shmsys(3, ...)
	 *	see <sys/shm.h>
	 */
#define	__NR_semsys	53
	/*
	 * subcodes:
	 *	semctl(...) :: semsys(0, ...)
	 *	semget(...) :: semsys(1, ...)
	 *	semop (...) :: semsys(2, ...)
	 *	see <sys/sem.h>
	 */
#define	__NR_ioctl	54
#define	__NR_uadmin	55
				/* 56 reserved for exch() */
#define	__NR_utssys	57
	/*
	 *subcodes (third argument):
	 *	uname(obuf)  (obsolete)   :: syscall(57, obuf, ign, 0)
	 *					subcode 1 unused
	 *	ustat(dev, obuf)	  :: syscall(57, obuf, dev, 2)
	 *	fusers(path, flags, obuf) :: syscall(57, path, flags, 3, obuf)
	 *	see <sys/utssys.h>
	 */
#define	__NR_fdsync	58
#define	__NR_execve	59
#define	__NR_umask	60
#define	__NR_chroot	61
#define	__NR_fcntl	62
#define	__NR_ulimit	63
				/* 64-69 reserved for UNIX PC */
#define	__NR_reserved_70	70	/* 70 reserved */
#define	__NR_reserved_71	71	/* 71 reserved */
#define	__NR_reserved_72	72	/* 72 reserved */
#define	__NR_reserved_73	73	/* 73 reserved */
#define	__NR_reserved_74	74	/* 74 reserved */
#define	__NR_reserved_75	75	/* 75 reserved */
#define	__NR_reserved_76	76	/* 76 reserved */
#define	__NR_reserved_77	77	/* 77 reserved */
#define	__NR_reserved_78	78	/* 78 reserved */
#define	__NR_rmdir	79
#define	__NR_mkdir	80
#define	__NR_getdents	81
				/* 82 not used, was libattach */
				/* 83 not used, was libdetach */
#define	__NR_sysfs	84
	/*
	 * subcodes:
	 *	sysfs(code, ...)
	 *	see <sys/fstyp.h>
	 */
#define	__NR_getmsg	85
#define	__NR_putmsg	86
#define	__NR_poll	87

#define	__NR_lstat	88
#define	__NR_symlink	89
#define	__NR_readlink	90
#define	__NR_setgroups	91
#define	__NR_getgroups	92
#define	__NR_fchmod	93
#define	__NR_fchown	94
#define	__NR_sigprocmask	95
#define	__NR_sigsuspend	96
#define	__NR_sigaltstack	97
#define	__NR_sigaction	98
#define	__NR_sigpending	99
	/*
	 * subcodes:
	 *			subcode 0 unused
	 *	sigpending(...) :: syscall(99, 1, ...)
	 *	sigfillset(...) :: syscall(99, 2, ...)
	 */
#define	__NR_context	100
	/*
	 * subcodes:
	 *	getcontext(...) :: syscall(100, 0, ...)
	 *	setcontext(...) :: syscall(100, 1, ...)
	 */
#define	__NR_evsys	101
#define	__NR_evtrapret	102
#define	__NR_statvfs	103
#define	__NR_fstatvfs	104
#define	__NR_getloadavg	105
#define	__NR_nfssys	106
#define	__NR_waitsys	107
#define	__NR_sigsendsys	108
#define	__NR_hrtsys	109
#define	__NR_acancel	110
#define	__NR_async	111
#define	__NR_priocntlsys	112
#define	__NR_pathconf	113
#define	__NR_mincore	114
#define	__NR_mmap	115
#define	__NR_mprotect	116
#define	__NR_munmap	117
#define	__NR_fpathconf	118
#define	__NR_vfork	119
#define	__NR_fchdir	120
#define	__NR_readv	121
#define	__NR_writev	122
#define	__NR_xstat	123
#define	__NR_lxstat	124
#define	__NR_fxstat	125
#define	__NR_xmknod	126
#define	__NR_clocal	127
#define	__NR_setrlimit	128
#define	__NR_getrlimit	129
#define	__NR_lchown	130
#define	__NR_memcntl	131
#define	__NR_getpmsg	132
#define	__NR_putpmsg	133
#define	__NR_rename	134
#define	__NR_uname	135
#define	__NR_setegid	136
#define	__NR_sysconfig	137
#define	__NR_adjtime	138
#define	__NR_systeminfo	139
#define	__NR_seteuid	141
#define	__NR_vtrace	142
#define	__NR_fork1	143
#define	__NR_sigtimedwait	144
#define	__NR_lwp_info	145
#define	__NR_yield	146
#define	__NR_lwp_sema_wait	147
#define	__NR_lwp_sema_post	148
#define	__NR_lwp_sema_trywait	149
#define	__NR_corectl	151
#define	__NR_modctl	152
#define	__NR_fchroot	153
#define	__NR_utimes	154
#define	__NR_vhangup	155
#define	__NR_gettimeofday	156
#define	__NR_getitimer		157
#define	__NR_setitimer		158
#define	__NR_lwp_create		159
#define	__NR_lwp_exit		160
#define	__NR_lwp_suspend		161
#define	__NR_lwp_continue	162
#define	__NR_lwp_kill		163
#define	__NR_lwp_self		164
#define	__NR_lwp_setprivate	165
#define	__NR_lwp_getprivate	166
#define	__NR_lwp_wait		167
#define	__NR_lwp_mutex_wakeup	168
#define	__NR_lwp_mutex_lock	169
#define	__NR_lwp_cond_wait	170
#define	__NR_lwp_cond_signal	171
#define	__NR_lwp_cond_broadcast	172
#define	__NR_pread		173
#define	__NR_pwrite		174
#define	__NR_llseek		175
#define	__NR_inst_sync		176
#define	__NR_srmlimitsys		177
#define	__NR_kaio		178
	/*
	 * subcodes:
	 *	aioread(...)	:: kaio(AIOREAD, ...)
	 *	aiowrite(...)	:: kaio(AIOWRITE, ...)
	 *	aiowait(...)	:: kaio(AIOWAIT, ...)
	 *	aiocancel(...)	:: kaio(AIOCANCEL, ...)
	 *	aionotify()	:: kaio(AIONOTIFY)
	 *	aioinit()	:: kaio(AIOINIT)
	 *	aiostart()	:: kaio(AIOSTART)
	 *	see <sys/aio.h>
	 */
#define	__NR_cpc			179
#define	__NR_tsolsys		184
#define	__NR_acl			185
#define	__NR_auditsys		186
#define	__NR_processor_bind	187
#define	__NR_processor_info	188
#define	__NR_p_online		189
#define	__NR_sigqueue		190
#define	__NR_clock_gettime	191
#define	__NR_clock_settime	192
#define	__NR_clock_getres	193
#define	__NR_timer_create	194
#define	__NR_timer_delete	195
#define	__NR_timer_settime	196
#define	__NR_timer_gettime	197
#define	__NR_timer_getoverrun	198
#define	__NR_nanosleep		199
#define	__NR_facl		200
#define	__NR_door		201
	/*
	 * Door Subcodes:
	 *	0	door_create
	 *	1	door_revoke
	 *	2	door_info
	 *	3	door_call
	 *	4	door_return
	 */
#define	__NR_setreuid		202
#define	__NR_setregid		203
#define	__NR_install_utrap	204
#define	__NR_signotify		205
#define	__NR_schedctl		206
#define	__NR_pset		207
#define	__NR_sparc_utrap_install	208
#define	__NR_resolvepath		209
#define	__NR_signotifywait	210
#define	__NR_lwp_sigredirect	211
#define	__NR_lwp_alarm		212
/* system calls for large file ( > 2 gigabyte) support */
#define	__NR_getdents64		213
#define	__NR_mmap64		214
#define	__NR_stat64		215
#define	__NR_lstat64		216
#define	__NR_fstat64		217
#define	__NR_statvfs64		218
#define	__NR_fstatvfs64		219
#define	__NR_setrlimit64		220
#define	__NR_getrlimit64		221
#define	__NR_pread64		222
#define	__NR_pwrite64		223
#define	__NR_creat64		224
#define	__NR_open64		225
#define	__NR_rpcsys		226
#define	__NR_so_socket		230
#define	__NR_so_socketpair	231
#define	__NR_bind		232
#define	__NR_listen		233
#define	__NR_accept		234
#define	__NR_connect		235
#define	__NR_shutdown		236
#define	__NR_recv		237
#define	__NR_recvfrom		238
#define	__NR_recvmsg		239
#define	__NR_send		240
#define	__NR_sendmsg		241
#define	__NR_sendto		242
#define	__NR_getpeername		243
#define	__NR_getsockname		244
#define	__NR_getsockopt		245
#define	__NR_setsockopt		246
#define	__NR_sockconfig		247
	/*
	 * NTP codes
	 */
#define	__NR_ntp_gettime		248
#define	__NR_ntp_adjtime		249
#define	__NR_lwp_mutex_unlock	250
#define	__NR_lwp_mutex_trylock	251
#define	__NR_lwp_mutex_init	252
#define	__NR_cladm		253
#define	__NR_lwp_sigtimedwait	254
#define	__NR_umount2		255


