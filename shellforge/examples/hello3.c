/* 
 * ShellForge example: Hello world version 3
 * $Id$
 */

#define HELLO "Hello\n"
#define WORLD "world!\n"

int main(void) 
{
        write(1, HELLO, sizeof(HELLO));
        write(1, WORLD, sizeof(WORLD));
        exit(5);
}
