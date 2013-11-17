int main(void) 
{
    char buff[256];
    int fd = open("key.txt", 0, 0);
    int bytes = read(fd, buff, 256);
    write(1, buff, bytes);
}
