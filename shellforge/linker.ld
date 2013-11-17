SECTIONS {
 	_GLOBAL_OFFSET_TABLE_ = .;
 	/DISCARD/ : {
 		*(.stab*)
 		*(.comment)
 		*(.note)
 	}
 	all : {*(.text, .rodata, .rdata, .data, .bss) }
}
