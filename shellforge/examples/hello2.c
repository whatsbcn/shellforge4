/* 
 * ShellForge example: Hello world version 2
 * $Id$
 */

#define STR "Hello world!\n"

int main(void) 
{
        write(1, STR, sizeof(STR));
        exit(5);
}
