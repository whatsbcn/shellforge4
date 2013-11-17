/*
 * Copyright (c) 2000-2003 Apple Computer, Inc. All rights reserved.
 */


#define	__NR_syscall	0
#define	__NR_exit	1
#define	__NR_fork	2
#define	__NR_read	3
#define	__NR_write	4
#define	__NR_open	5
#define	__NR_close	6
#define	__NR_wait4	7
				/* 8 is old creat */
#define	__NR_link	9
#define	__NR_unlink	10
				/* 11 is obsolete execv */
#define	__NR_chdir	12
#define	__NR_fchdir	13
#define	__NR_mknod	14
#define	__NR_chmod	15
#define	__NR_chown	16
				/* 17 is obsolete sbreak */
#if COMPAT_GETFSSTAT
				/* 18 is old getfsstat */
#else
#define	__NR_getfsstat	18
#endif
				/* 19 is old lseek */
#define	__NR_getpid	20
				/* 21 is obsolete mount */
				/* 22 is obsolete umount */
#define	__NR_setuid	23
#define	__NR_getuid	24
#define	__NR_geteuid	25
#define	__NR_ptrace	26
#define	__NR_recvmsg	27
#define	__NR_sendmsg	28
#define	__NR_recvfrom	29
#define	__NR_accept	30
#define	__NR_getpeername	31
#define	__NR_getsockname	32
#define	__NR_access	33
#define	__NR_chflags	34
#define	__NR_fchflags	35
#define	__NR_sync	36
#define	__NR_kill	37
				/* 38 is old stat */
#define	__NR_getppid	39
				/* 40 is old lstat */
#define	__NR_dup	41
#define	__NR_pipe	42
#define	__NR_getegid	43
#define	__NR_profil	44
#define	__NR_ktrace	45
#define	__NR_sigaction	46
#define	__NR_getgid	47
#define	__NR_sigprocmask	48
#define	__NR_getlogin	49
#define	__NR_setlogin	50
#define	__NR_acct	51
#define	__NR_sigpending	52
#define	__NR_sigaltstack	53
#define	__NR_ioctl	54
#define	__NR_reboot	55
#define	__NR_revoke	56
#define	__NR_symlink	57
#define	__NR_readlink	58
#define	__NR_execve	59
#define	__NR_umask	60
#define	__NR_chroot	61
				/* 62 is old fstat */
				/* 63 is unused */
				/* 64 is old getpagesize */
#define	__NR_msync	65
#define	__NR_vfork	66
				/* 67 is obsolete vread */
				/* 68 is obsolete vwrite */
#define	__NR_sbrk	69
#define	__NR_sstk	70
				/* 71 is old mmap */
				/* 72 is obsolete vadvise */
#define	__NR_munmap	73
#define	__NR_mprotect	74
#define	__NR_madvise	75
				/* 76 is obsolete vhangup */
				/* 77 is obsolete vlimit */
#define	__NR_mincore	78
#define	__NR_getgroups	79
#define	__NR_setgroups	80
#define	__NR_getpgrp	81
#define	__NR_setpgid	82
#define	__NR_setitimer	83
				/* 84 is old wait */
#define	__NR_swapon	85
#define	__NR_getitimer	86
				/* 87 is old gethostname */
				/* 88 is old sethostname */
#define __NR_getdtablesize 89
#define	__NR_dup2	90
#define	__NR_fcntl	92
#define	__NR_select	93
				/* 94 is obsolete setdopt */
#define	__NR_fsync	95
#define	__NR_setpriority	96
#define	__NR_socket	97
#define	__NR_connect	98
				/* 99 is old accept */
#define	__NR_getpriority	100
				/* 101 is old send */
				/* 102 is old recv */
#define	__NR_sigreturn	103

#define	__NR_bind	104
#define	__NR_setsockopt	105
#define	__NR_listen	106
				/* 107 is obsolete vtimes */
				/* 108 is old sigvec */
				/* 109 is old sigblock */
				/* 110 is old sigsetmask */
#define	__NR_sigsuspend	111
				/* 112 is old sigstack */
				/* 113 is old recvmsg */
				/* 114 is old sendmsg */
				/* 115 is obsolete vtrace */
#define	__NR_gettimeofday	116
#define	__NR_getrusage	117
#define	__NR_getsockopt	118
				/* 119 is obsolete resuba */
#define	__NR_readv	120
#define	__NR_writev	121
#define	__NR_settimeofday	122
#define	__NR_fchown	123
#define	__NR_fchmod	124
				/* 125 is old recvfrom */
				/* 126 is old setreuid */
				/* 127 is old setregid */
#define	__NR_rename	128
				/* 129 is old truncate */
				/* 130 is old ftruncate */
#define	__NR_flock	131
#define	__NR_mkfifo	132
#define	__NR_sendto	133
#define	__NR_shutdown	134
#define	__NR_socketpair	135
#define	__NR_mkdir	136
#define	__NR_rmdir	137
#define	__NR_utimes	138
#define	__NR_futimes	139
#define	__NR_adjtime	140
				/* 141 is old getpeername */
				/* 142 is old gethostid */
				/* 143 is old sethostid */
				/* 144 is old getrlimit */
				/* 145 is old setrlimit */
				/* 146 is old killpg */
#define	__NR_setsid	147
				/* 148 is obsolete setquota */
				/* 149 is obsolete quota */
				/* 150 is old getsockname */
#define	__NR_getpgid	151
#define __NR_setprivexec 152
#define	__NR_pread	153
#define	__NR_pwrite	154
#define	__NR_nfssvc	155
				/* 156 is old getdirentries */
#define	__NR_statfs	157
#define	__NR_fstatfs	158
#define __NR_unmount	159
				/* 160 is obsolete async_daemon */
#define	__NR_getfh	161
				/* 162 is old getdomainname */
				/* 163 is old setdomainname */
				/* 164 is obsolete pcfs_mount */
#define __NR_quotactl	165
				/* 166 is obsolete exportfs	*/
#define __NR_mount	167
				/* 168 is obsolete ustat */
				/* 169 is unused */
#define __NR_table	170
				/* 171 is old wait_3 */
				/* 172 is obsolete rpause */
				/* 173 is unused */
				/* 174 is obsolete getdents */
#define __NR_gc_control	175
#define __NR_add_profil	176
				/* 177 is unused */
				/* 178 is unused */
				/* 179 is unused */
#define __NR_kdebug_trace 180       
#define	__NR_setgid	181
#define	__NR_setegid	182
#define	__NR_seteuid	183

#define	__NR_sigreturn_ppc	184
				/* 185 is unused */
				/* 186 is unused */
				/* 187 is unused */
#define	__NR_stat	188
#define	__NR_fstat	189
#define	__NR_lstat	190
#define	__NR_pathconf	191
#define	__NR_fpathconf	192
#if COMPAT_GETFSSTAT
#define	__NR_getfsstat	193
#endif
#define	__NR_getrlimit	194
#define	__NR_setrlimit	195
#define __NR_getdirentries	196
#define	__NR_mmap	197
#define	__NR___syscall	198
#define	__NR_lseek	199
#define	__NR_truncate	200
#define	__NR_ftruncate	201
#define	__NR___sysctl	202
#define __NR_mlock	203
#define __NR_munlock 	204
#define	__NR_undelete	205
#define	__NR_ATsocket	206
#define	__NR_ATgetmsg	207
#define	__NR_ATputmsg	208
#define	__NR_ATPsndreq	209
#define	__NR_ATPsndrsp	210
#define	__NR_ATPgetreq	211
#define	__NR_ATPgetrsp	212
				/* 213 is reserved for AppleTalk */
#define __NR_kqueue_from_portset_np 214
#define __NR_kqueue_portset_np	215
#define __NR_mkcomplex	216 
#define __NR_statv	217		
#define __NR_lstatv	218 			
#define __NR_fstatv	219 			
#define __NR_getattrlist	220 		
#define __NR_setattrlist	221		
#define __NR_getdirentriesattr	222 	
#define __NR_exchangedata	223 				
#define __NR_checkuseraccess	224 
#define __NR_searchfs	 225

				/* 226 - 230 are reserved for HFS expansion */
				/* 231 - 241 are reserved  */
#define	__NR_fsctl	242
				/* 243 - 246 are reserved  */
#define	__NR_nfsclnt	247	/* from freebsd, for lockd */
#define	__NR_fhopen	248	/* from freebsd, for lockd */
				/* 249 is reserved  */
#define __NR_minherit	 250
#define	__NR_semsys	251
#define	__NR_msgsys	252
#define	__NR_shmsys	253
#define	__NR_semctl	254
#define	__NR_semget	255
#define	__NR_semop	256
#define	__NR_semconfig	257
#define	__NR_msgctl	258
#define	__NR_msgget	259
#define	__NR_msgsnd	260
#define	__NR_msgrcv	261
#define	__NR_shmat	262
#define	__NR_shmctl	263
#define	__NR_shmdt	264
#define	__NR_shmget	265
#define	__NR_shm_open	266
#define	__NR_shm_unlink	267
#define	__NR_sem_open	268
#define	__NR_sem_close	269
#define	__NR_sem_unlink	270
#define	__NR_sem_wait	271
#define	__NR_sem_trywait	272
#define	__NR_sem_post	273
#define	__NR_sem_getvalue 274
#define	__NR_sem_init	275
#define	__NR_sem_destroy	276
				/* 277 - 295 are reserved  */
#define __NR_load_shared_file 296
#define __NR_reset_shared_file 297
#define __NR_new_system_shared_regions 298
				/* 299 - 309 are reserved  */
#define	__NR_getsid	310
				/* 311 - 312 are reserved */
#define	__NR_aio_fsync	313
#define	__NR_aio_return	314
#define	__NR_aio_suspend	315
#define	__NR_aio_cancel	316
#define	__NR_aio_error	317
#define	__NR_aio_read	318
#define	__NR_aio_write	319
#define	__NR_lio_listio	320
				/* 321 - 323 are reserved */
#define __NR_mlockall	 324
#define __NR_munlockall	 325
				/* 326 is reserved */
#define __NR_issetugid    327
#define __NR___pthread_kill    328
#define __NR_pthread_sigmask    329
#define __NR_sigwait    330

#define __NR_audit		350	/* submit user space audit records */
#define __NR_auditon		351	/* audit subsystem control */
#define __NR_auditsvc		352	/* audit file control */
#define __NR_getauid		353
#define __NR_setauid		354
#define __NR_getaudit		355
#define __NR_setaudit		356
#define __NR_getaudit_addr	357
#define __NR_setaudit_addr	358
#define __NR_auditctl		359	/* audit control */

#define __NR_kqueue    362
#define __NR_kevent    363
