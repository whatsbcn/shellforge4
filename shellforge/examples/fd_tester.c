int main() {
	int i;
	for (i=1; i<256; i++) {
		int j;
		for (j=1; j<i; j++) {
			write(i, "A\n", 2);
		}
	}
	return 0;
}
