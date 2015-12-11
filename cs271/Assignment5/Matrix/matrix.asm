;TITLE Assignment5-A						(main.asm)

; Author: Cody Malick
; Course/Project ID: CS 271, Assignment 5 - A
; Date Created: 8/10/2014
; Date Last Modified: 8/10/2014
; Description: 4x4 letter matrix

;%include "Along32.asm"

;used this webpage to figure out prinf: 
;http://www.csee.umbc.edu/portal/help/nasm/sample.shtml
;section: Prinf1.asm basic calling prinf
extern printf
extern rand
extern srand
extern time

SECTION .data

prnt_form: db "%c", 0 ;10, 0 = \n, 0
prnt_num: db "%d", 10, 0
prnt_break: db "%c", 10, 0

rand_form: db "time(NULL)",0



;letter lists
test_char db 'b', 0
con_list db "BCDFGHJKLMNPQRSTVWXYZ",0
vow_list db "AEIOU",0


con_size dd 21 ;RandomRange uses 0 -> (n-1)
vow_size dd 5 ;RandomRange

space db " ",0
break_count dd 0
rnum dd 0

SECTION .text

mat_gen:
	push ebp
	mov ebp, esp
	mov ecx, DWORD 4
num_mat:
	push ecx
	mov ecx, DWORD 0
	mov ecx, DWORD 4

lett_pick:
	mov eax, DWORD 2
	;call RandomRange

	push ecx
	call rand
	mov ebx, DWORD 2
	xor edx, edx
	div DWORD ebx

	mov eax, edx

	cmp eax, 0
	je print_const

print_vowel:
	pusha
	call print_vow
	popa

	jmp print_space
print_const:
	pusha
	call print_con
	popa
print_space:

	;push ecx
	
	;add a space
	movzx DWORD eax, BYTE [space]
	push eax
	push prnt_form
	call printf
	pop eax
	pop eax 
	
	pop ecx
	loop lett_pick

	;break line
	push ecx

	mov eax, DWORD 0
	push eax
	push prnt_break
	call printf
	pop eax
	pop eax

	pop ecx

	pop ecx
	loop num_mat

	pop ebp
	ret

print_con:
	push ebp
	mov ebp, esp

	;clear registers
	mov eax, DWORD 0
	mov ebx, DWORD 0
	mov edx, DWORD 0

	;mov eax, [con_size]

	call rand
	mov ebx, [con_size]
	xor edx, edx
	div DWORD ebx

	mov edi, edx
	movzx DWORD eax, BYTE [con_list+edi]
	
	push eax
	push prnt_form
	call printf

	pop eax
	pop eax

	pop ebp

	;call WriteChar
	ret

print_vow:
	push ebp
	mov ebp, esp
	;clear registers
	mov eax, DWORD 0
	mov ebx, DWORD 0
	mov edx, DWORD 0

	call rand
	mov ebx, [vow_size]
	xor edx, edx
	div DWORD ebx

	mov edi, edx
	movzx DWORD eax, BYTE [vow_list+edi]
	
	push eax
	push prnt_form
	call printf

	pop eax
	pop eax

	pop ebp
	ret

;---------------------

	global main
main:
	;call Randomize

	mov eax, DWORD 0
	push eax
	;print out the return value
	call time
	pop ebx
	push eax
	call srand
	pop ebx


	mov ecx, DWORD 5

gen:

	pusha
	call mat_gen
	popa

	;break line after matrix is printed
	push ecx
	mov eax, DWORD 0
	push eax
	push prnt_break
	call printf
	pop eax
	pop eax


	pop ecx

	loop gen
ret
