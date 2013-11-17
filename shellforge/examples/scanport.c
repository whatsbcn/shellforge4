/* 
 * ShellForge example: scan localhost's TCP ports
 * $Id$
 */

#define FIRST 1
#define LAST 5001

int main(void) {
        struct sockaddr_in sa;
        int s,l,i;
        char buf[1024];


        sa.sin_family = PF_INET;
        sa.sin_addr.s_addr = IP(127,0,0,1);

        i=FIRST-1;
//        write(1,"go[\n",4);
reopen:
        if ((s = socket(PF_INET, SOCK_STREAM, 0)) < 0) write(1,"erreur\n",7);
        while(++i<LAST) {
                sa.sin_port = htons(i);
                if (connect(s, (struct sockaddr *)&sa, sizeof(struct sockaddr)) 
< 0) {

                        /* nothing */
                } else {
                    write(1, &i, sizeof(i));
                    close(s);
                    goto reopen;
                }
        }
//        write(1,"]og\n",5);
        close(1);
        exit(0);

}

