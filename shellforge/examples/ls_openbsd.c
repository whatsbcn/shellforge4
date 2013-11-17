/* 
 * ShellForge example: list a directory - OpenBSD style
 * $Id$
 */

#define DIRECTORY "/"

int main(void)
{
	int fd;
	struct bsd_dirent *d;
	int l,ret;
	long base=0;
	int bb;
	char buf[8192];

	write(1,"go[",3);

	fd = open(DIRECTORY, O_RDONLY, 0);
	if (fd < 0) 
		write(1,"ERROR open",10);
        else {
                ret=getdirentries(fd, buf, sizeof(buf), &base);
                if (ret < 0) write(1,"ERROR getdirentries",19); 
		else { 
  	                for (bb=0, d=(struct bsd_dirent *)buf;
                             bb < ret && d->d_reclen ;
                             bb+=d->d_reclen, d=(struct bsd_dirent *)(buf+bb)) {
                                 write(1, d->d_name, d->d_namlen);
                                 write(1, "\r\n", 2);
                        }
		}
        }
	write(1,"]og",4);

	exit(0);
}
