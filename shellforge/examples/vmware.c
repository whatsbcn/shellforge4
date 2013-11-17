/* 
 * ShellForge example: Return vmware version, if any. Not safe for the injected process.
 * $Id$
 */


#define MAGIC 0x564d5868 /* "VMXh" */
#define PORT  0x5658     /* "VX" */
#define GETVERSION 0x0a


static char *versions[] = 
 {"??","Express","ESX Server","GSX Server","Workstation" };
static int vlen[] = {2,7,10,10,11};

static void segfault(){
   write(1,"Not a VMware box.\n",18);
   exit(1);
}


int main(){
   unsigned int ok, ver, magic;

   signal(11, segfault);
   
   __asm__  __volatile__ (" \
        push %%ebx          \n\
        in %%dx, %%eax      \n\
        mov %%ebx, %1       \n\
        pop %%ebx           \n\
        "
        : "=a"(ok), "=m"(magic), "=c"(ver)
        : "0" (MAGIC), "c" (GETVERSION), "d" (PORT)
        );

   if (magic == MAGIC) {
	write(1, "VMware ", 7);
	if (ok == 6) {
		write(1, versions[ver], vlen[ver]);
        	write(1, "\n", 1);
	}
	else write(1, "unknown\n",8);
   }
   else write(1, "Not vmware\n",11);
   exit(0);
}
