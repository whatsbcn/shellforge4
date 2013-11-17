/* 
 * ShellForge example: execute /bin/ls -aR /
 * $Id$
 */

int main()
{
        char *a[] = {"/bin/cp", "/home/gate8/key", "/tmp/a", 0}; 
        execve(*a, a, 0);
}

