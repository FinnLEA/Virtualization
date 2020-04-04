	.file	"main.c"
	.intel_syntax noprefix
	.def	__main;	.scl	2;	.type	32;	.endef
	.section .rdata,"dr"
.LC0:
	.ascii "%x\12\0"
.LC1:
	.ascii "pause\0"
	.text
	.globl	main
	.def	main;	.scl	2;	.type	32;	.endef
	.seh_proc	main
main:
	push	rbp
	.seh_pushreg	rbp
	mov	rbp, rsp
	.seh_setframe	rbp, 0
	sub	rsp, 48
	.seh_stackalloc	48
	.seh_endprologue
	call	__main
	mov	DWORD PTR -4[rbp], 10
/APP
 # 8 "main.c" 1
	.intel_syntax noprefix
	db 0xa, 0xff
 # 0 "" 2
/NO_APP
	mov	eax, DWORD PTR -4[rbp]
	mov	edx, eax
	lea	rcx, .LC0[rip]
	call	printf
	lea	rcx, .LC1[rip]
	call	system
	mov	eax, 0
	add	rsp, 48
	pop	rbp
	ret
	.seh_endproc
	.ident	"GCC: (x86_64-posix-sjlj, built by strawberryperl.com project) 4.9.2"
	.def	printf;	.scl	2;	.type	32;	.endef
	.def	system;	.scl	2;	.type	32;	.endef
