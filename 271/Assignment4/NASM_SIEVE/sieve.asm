;TITLE Assignment4-Sieve						(main.asm)

; Author: Cody Malick
; Course/Project ID: CS 271, Assignment 4-C
; Date Created: 8/6/2014
; Date Last Modified: 8/7/2014
; Description: Sieve

%INCLUDE "Along32.asm"

SECTION .data

;Text to be displayed throughout the program
prog_title db "Assignment4-C-Sieve",0dh,0ah,0 
my_name db "Cody Malick",0dh,0ah,0
term_mess db "**END OF PROGRAM**",0dh,0ah,0

;Integer Variables
one dd 1d
ival dd 0d
val1 dd 0d
val2 dd 0d

num_type dd 4
num_len dd 100000
num_size dd 400000

;Constants
;LIST_SIZE EQU 650000

;Number Arrays
;num_list DWORD LIST_SIZE DUP (?)
num_list TIMES 100000 dd 0

SECTION .text

fill_array:
	push ebp
	mov ebp, esp

	;edi is starting location
	mov edi, [num_type] ;type
	;ebx is size of each item
	mov ebx, [num_type] ;type

	mov DWORD eax, 1
	mov DWORD ecx, [num_len] ;length

l1:
	mov [num_list+edi], eax
	add eax, 1
	add edi, ebx
	loop l1

	pop ebp
	ret

;--------------------------------


find_primes:
	push ebp
	mov ebp, esp

	mov DWORD edi, 0
	mov eax, [num_list+edi]

check_num:
	;move again for loop
	mov DWORD eax, [num_list+edi]
	cmp DWORD eax, [one]
	jg case_above

case_1:
	add edi, 4
	jmp check_num

case_above:
	mov DWORD eax, [num_list+edi]
	call WriteDec
	call Crlf
	;mov values for it_prime, we don't want these back
	mov DWORD [val1], edi
	pusha
	call it_prime
	popa

	mov DWORD eax, [num_list+edi]
	mov DWORD ebx, [num_list+edi]
	mul ebx
	add DWORD edi, 4
	;call WriteDec
	cmp eax, [num_len]
	jl check_num
print_value:

	mov DWORD eax, [num_list+edi]

	cmp eax, [one]
	je skip_1
	call WriteDec
	call Crlf
skip_1:
	add DWORD edi, 4

	cmp edi, [num_size]
	jl print_value

	pop ebp
	ret


;------------------------------------------

it_prime:
	push ebp
	mov ebp, esp

	;initial value (2, 3, 5, etc)
	mov DWORD edi, [val1]

	mov ebx, edi

	;eax is location doubled, 2 = 16, 3 = 24
	mov DWORD eax, 2
	mul edi
	;mov doubled size into edi
	mov edi, eax
	mov eax, ebx
	;ebx contains 1 for setting
	mov DWORD ebx, 1
l2:
	;mov 1 into value
	mov DWORD [num_list+edi], DWORD ebx
	add edi, eax
	cmp DWORD edi, [num_size]
	jg end_prime
	jmp l2
end_prime:

	pop ebp
	ret


global main
main:
	;INTRODUCTION

	;Output name and program title
	mov edx, my_name ;OFFSET
	call WriteString

	mov	 edx, prog_title ;OFFSET
	call WriteString
	call Crlf

;part 1
;set the values of the array
p1:
	pusha 
	call fill_array
	popa
	
	pusha
	call find_primes
	popa

	call Crlf

ret