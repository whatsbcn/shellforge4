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
	cld
	lodsd
	mov ecx,eax
	lea edi,[esi+ecx]
dloop:
	xor eax,eax
	lodsb
	btr eax,7
	jc replic
	sub ecx,eax
	xchg eax,ecx
	rep movsb
	xchg eax,ecx
reloop:	
	loop dloop
	jmp esi
replic:
	mov ah,al
	lodsb
	push ecx
	push esi
	mov ecx,eax
	shr ecx,10
	and ah, 0x3
	mov esi,edi
	sub esi,eax
	rep movsb
	pop esi
	pop ecx
	loop reloop
getaddr:
	call back
