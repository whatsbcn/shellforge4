#if 0
#include <sys/socket.h>
#include <string.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#endif
int main(void)
{
	struct sockaddr_in6 tcp;
	int sock = 0, slen;
	//unsigned char *ptr = (unsigned char *)&tcp;
	
	//sock = socket(AF_INET6, SOCK_STREAM, 0);
	//memset(ptr, 0, sizeof(tcp));
	//tcp.sin6_family = htons(AF_INET6);
	//tcp.sin6_port = htons(9999);
	//bind(sock, (struct sockaddr *) &tcp, sizeof(tcp));
	listen(sock, 1);
	sock = accept(sock, (struct sockaddr *) &tcp, &slen);
	char c;
	while (read(sock, &c, 1)){

	}
}
