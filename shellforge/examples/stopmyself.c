/* 
 * ShellForge example: return code 42
 * $Id$
 */

#define SIGSTOP 19

int main(void)
{
      kill(getpid(), SIGSTOP);
}
