;TITLE Assignment5-B						(main.asm)

; Author: Cody Malick
; Course/Project ID: CS 271, Assignment 5 - B
; Date Created: 8/11/2014
; Date Last Modified: 8/11/2014
; Description: Sum of rows in a 2d array



extern printf
;extern rand
;extern srand
;extern ctime

SECTION .data

x dq 22.0
y dq 20.0

;fmt: db "%s",0
;test: db "test",10,0
bigx: db "X is greater than Y",10,0
bigy: db "Y is greater than X",10,0

SECTION .text


	global main
main:
	finit
	;fild for integer
	fld QWORD [x]
	fld QWORD [y]

	;make sure ax is empty
	mov eax, DWORD 0
	fcom
	fnstsw ax
	sahf
	ja case_y	

	push DWORD bigx	
	call printf
	pop eax

	jmp end_lab
case_y:
	mov eax, DWORD 0
	push DWORD bigy
	call printf
	pop eax

end_lab:
	
ret
