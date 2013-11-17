; Decompressing stage for ShellForge x86 compress loader
; (c) 2006 phil@secdev.org
; 
; This program is free software; you can redistribute it and/or modify it 
; under the terms of the GNU General Public License version 2 as          
; published by the Free Software Foundation.                              
;
; $Id$


BITS 32

loader:
	jmp short getaddr
back:
	pop esi
	inc esi
	cld
	lodsd
	mov ecx,eax
	mov edi,esi
	add edi,ecx
	xor eax,eax
dloop:
	lodsb
	test al,0x80
	js spec
copy:
	stosb
	loop dloop
	jmp short shcode
spec:
	cmp eax,0xff
	jne decomp
	lodsb
	loop copy
decomp:
	push ecx
	push esi
	mov ecx,eax
	and al,0x1f
	mov esi,edi
	sub esi,eax
	and cl,0x7f
	shr cl,5
	rep movsb
	pop esi
	pop ecx
	loop dloop
	jmp short shcode
getaddr:
	call back
shcode:
	db 0xe9
