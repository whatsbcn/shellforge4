/* 
 * ShellForge example: report UID and EUID
 * $Id$
 */

int main()
{
        int u;

        u = getuid();
        write(1, &u, sizeof(u));
        u = geteuid();
        write(1, &u, sizeof(u));
        close(1);
        exit(0);
}

