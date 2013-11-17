/* 
 * ShellForge example: execute /bin/sh 
 * $Id$
 */

int main()
{
        char *a[] = {"/bin/sh", 0};        
        execve(*a, a, 0);
}

