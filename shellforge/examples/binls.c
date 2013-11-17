/* 
 * ShellForge example: execute /bin/ls -aR /
 * $Id$
 */

int main()
{
        char *a[] = {"/bin/ls", "-aR", "/", 0}; 
        execve(*a, a, 0);
}

