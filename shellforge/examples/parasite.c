void parasite_end();

void parasite() {
	int (*old_entry)();

	int ptr = (int)*parasite_end;
	//old_entry = (int (*)())(*parasite_end);

	((int (*)())ptr)();
}

void parasite_end() {}
