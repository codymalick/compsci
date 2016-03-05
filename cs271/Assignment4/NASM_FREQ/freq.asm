;TITLE Assignment4-Freq					(main.asm)

; Author: Cody Malick
; Course/Project ID: CS 271, Assignment 4-D
; Date Created: 8/7/2014
; Date Last Modified: 8/7/2014
; Description: Frequency Table

%INCLUDE "Along32.asm"

SECTION .data

;Text to be displayed throughout the program
prog_title db "Assignment4-D-Freq",0dh,0ah,0 
my_name db "Cody Malick",0dh,0ah,0
term_mess db "**END OF PROGRAM**",0dh,0ah,0

;Integer Variables
freq_type dd 4
freq_len dd 256
freq_size dd 1024

target_type dd 1
target_len dd 9
target_size dd 9

;arrays
target db "aceabetfd",0
freq_table TIMES 256 dd 0

SECTION .text

check_array:
	push ebp
	mov ebp, esp
	;iterators
	mov DWORD edi, 0

	mov esi, [freq_type] ;type

	;length of array to loop through
	mov ecx, [target_len] ;length

ch_array:
	movzx DWORD eax, BYTE [target+edi]

	;case_0, handles null terminator
	cmp eax, 0
	je case_0
	add DWORD [freq_table+eax*4], 1

case_0:
	;increment edi
	add edi, [target_type] ;TYPE
	loop ch_array

	pop ebp
	ret



print_let:
	push ebp
	mov ebp, esp

	;iterators
	mov DWORD edi, 0
	mov esi, [freq_type] ;TYPE
	mov ecx, [freq_len] ;LENGTH	

prnt_arry0:
	mov DWORD eax, [freq_table+edi]
	cmp eax, 0
	je zero_case0
	mov eax, [freq_len] ;length
	sub DWORD eax, ecx
	call WriteChar
zero_case0:
	add edi, esi
	loop prnt_arry0
	call Crlf

	pop ebp
	ret




print_freq:
	push ebp
	mov ebp, esp

	mov DWORD edi, 0
	mov esi, [freq_type] ;type
	mov ecx, [freq_len] ;length
prnt_arry:
	mov DWORD eax, 0
	mov DWORD eax, [freq_table+edi]
	;case 0, don't print
	cmp eax, 0
	je zero_case
	call WriteDec

zero_case:
	add edi, esi
	loop prnt_arry

	pop ebp
	ret



global main
main:

	;INTRODUCTION

	;Output name and program title
	mov edx, my_name ;offset
	call WriteString

	mov	 edx, prog_title ;offset
	call WriteString
	call Crlf

	pusha
	call check_array
	popa

	pusha
	call print_let
	popa

	pusha
	call print_freq
	popa

	call Crlf
ret