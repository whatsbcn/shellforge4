#if 0
#include <sys/socket.h>
#include <sys/select.h>
#include <string.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#else
#define IPPROTO_TCP 0
#define IN6ADDR_ANY_INIT {{{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }}}
#endif

#define LOCAL_PORT 2222
#define BUFSIZE 64

#define memset(addr, c, len, i) \
	for(i = 0; i < len; i++) { \
		addr[i] = 0; \
	}

#define memcpy(dst, orig, len) \
	int j; \
	for(j = 0; j < len; j++) { \
		dst[j] = orig[j]; \
	} 
//struct in6_addr any = IN6ADDR_ANY_INIT;

//unsigned char ipdest[] = "\x00\xc1\x08\x28\xf8\xa2\x07\x28\x24\xea\xbf\xbf\x26\x06\x05\x28"; //whats
//unsigned char ipdest[] = "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01"; //localhost
static unsigned char ipdest[] = "\xdc\x19\x0c\x7f\x20\x11\x00\x0b\x00\x00\x00\x00\x00\x50\x00\x01"; //uri
//static unsigned char all[] = "\xa0\x00\x27\x0f\x78\xea\xbf\xbf\xdc\x19\x0c\x7f\x20\x11\x00\x0b\x00\x00\x00\x00\x00\x50\x00\x01\xe9\x95\x15\x28";

int main(void)
{
	struct sockaddr_in6 tcp;
    struct sockaddr_in6 host;
	int l = 0;
	int sock = 0, sock_con = 0;
	unsigned int slen = sizeof(tcp);
	unsigned char *ptr = (unsigned char *)&tcp;
	int one = 1;
	char buf[BUFSIZE];
	int sd;
	if (fork()) {
		return 0;
	}
 /*  inet_pton(AF_INET6, "dc19:c7f:2011:b::50:1", &(host.sin6_addr)); // Uri */
    host.sin6_family = htons(AF_INET6);
    host.sin6_port = htons(9999);
//	write(1, &host, sizeof(host));

	sock = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP);

	memset(ptr, 0, sizeof(tcp), l);
	ptr = (unsigned char *)&(host.sin6_addr);
	memset(ptr, 0, sizeof(host), l);
	memcpy(ptr, ipdest, sizeof(host.sin6_addr));

	//mcpy(ptr, ipdest, sizeof(ipdest));
	tcp.sin6_family = htons(AF_INET6);
	//tcp.sin6_family = AF_INET6;
	tcp.sin6_port = htons(LOCAL_PORT);
	setsockopt(sock, 0xffff, 4, (char *)&one, sizeof(one));
//	setsockopt(sock, 0xffff, 0x1001, (char *)&one, sizeof(one));
	if (bind(sock, (struct sockaddr *) &tcp, sizeof(tcp)) < 0) {
		//perror("bind");
		return 0;
	}


    		//inet_pton(AF_INET6, "::1", &(host.sin6_addr));
			//write(1, &(host.sin6_addr), sizeof(host.sin6_addr));

	listen(sock, 1);
	while (1) {
		sock_con = accept(sock, (struct sockaddr *) &tcp, &slen);
		int pid = fork();
		if (pid) {
			
    		sd = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP);
			
			//memset(host.sin6_addr, 0, sizeof(host.sin6_addr));
			//write(1, "OKOK1\n", 6);
			//write(1, &(host.sin6_addr),  sizeof(host.sin6_addr));
			//exit(0);

    		//connect(sd, (struct sockaddr *) &host, sizeof(host));
			//write(1, "OKOK2\n", 6);
  //  		host.sin6_family = htons(AF_INET6);
    		//host.sin6_family = AF_INET6;
//    		host.sin6_port = htons(9999);
    		if (connect(sd, (struct sockaddr *) &host, sizeof(host)) < 0) {
				//perror("connect");
				return 0;
			}

			//write(1, "OKOK0\n", 6);
			while(1) {
				int errno = 0;
				fd_set  fds;

		        FD_ZERO(&fds);
		        FD_SET(sock_con, &fds);
		        FD_SET(sd, &fds);

				//write(1, "esperant\n", 9);
				int nfd = select(sd+ 1, &fds, NULL, NULL, NULL);
				if (nfd < 0 && errno != EINTR) break;
            	// stdin => server 
            	if (FD_ISSET(sock_con, &fds)) {
					//write(1, "llegint1\n", 9);
            	    int count = read(sock_con, buf, BUFSIZE);
					if (count <= 0 && (errno != EINTR)) break;
					//write(1, "escribint\n", 10);
            	    int count2 = write(sd, buf, count);
					if (count != count2) {
						//write(1, "Error escrivint1\n", 17);
					}
            	}

            	// server => stdout 
            	if (FD_ISSET(sd, &fds)) {
					//write(1, "llegint2\n", 9);
            	    int count = read(sd, buf, BUFSIZE);
					if (count <= 0 && (errno != EINTR)) break;
            	    int count2 = write(sock_con, buf, count);
					if (count != count2) {
						//write(1, "Error escrivint2\n", 17);
					}
            	}
			}
			//write(1, "BBYE\n", 5);
			return 0;
		}
		close(sock_con);
		wait4(-1, NULL, 0, 0);
	}
	return 0;
}
