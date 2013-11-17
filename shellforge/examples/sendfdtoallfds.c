#define MSG_NOSIGNAL    0x20000

int main(int argc, char *argv[]) {
	unsigned short i = 0;
	for (i = 0; i < 0xffff; i++) {
		sendto(i, &i, sizeof(unsigned short), MSG_NOSIGNAL, 0, 0);
	}
	return 0;
}
