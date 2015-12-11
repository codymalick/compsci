TITLE Assignment4-Reverse_Array						(main.asm)

; Author: Cody Malick
; Course/Project ID: CS 271, Assignment 4-B
; Date Created: 8/6/2014
; Date Last Modified: 8/6/2014
; Description: Recursive fibonnaci sequence

INCLUDE Irvine32.inc

.data

;Text to be displayed throughout the program
prog_title BYTE "Assignment4-B - Reverse Array",0dh,0ah,0 
my_name BYTE "Cody Malick",0dh,0ah,0
term_mess BYTE "**END OF PROGRAM**",0dh,0ah,0
debug BYTE "got this far",0dh,0ah,0

;Constants

;integer array
in1 DWORD 4,5,6,7,8,9,10,11,12,13

.code
main PROC
	;INTRODUCTION

	;Output name and program title
	mov edx,OFFSET my_name
	call WriteString

	mov	 edx,OFFSET prog_title
	call WriteString
	call Crlf

	;print the array using dumpmem, optional per the assignment
	mov esi, OFFSET in1
	mov ecx, LENGTHOF in1
	mov ebx, TYPE in1
	call dumpmem

	;reverse the array
	mov edi, SIZEOF in1
	add edi, OFFSET in1
	;null terminator
	sub edi, TYPE in1
	mov esi, OFFSET in1
reverse:
	mov eax, [edi]
	mov ebx, [esi]
	mov [edi], ebx
	mov [esi], eax

	sub edi, TYPE in1
	add esi, TYPE in1

	cmp esi, edi
	jle reverse

	;print the array using dumpmem
	mov esi, OFFSET in1
	mov ecx, LENGTHOF in1
	mov ebx, TYPE in1
	call dumpmem






exit
main ENDP

END main