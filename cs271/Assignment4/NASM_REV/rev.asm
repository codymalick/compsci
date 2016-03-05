; Author: Cody Malick
; Course/Project ID: CS 271, Assignment 4-B
; Date Created: 8/6/2014
; Date Last Modified: 8/6/2014
; Description: fibonnaci sequence

%include "Along32.asm"

SECTION .data

;Text to be displayed throughout the program
prog_title db 'Assignment4-B - Reverse Array',0
my_name db 'Cody Malick',0
term_mess db '**END OF PROGRAM**',0dh,0ah,0


;integer array
in1 dd 4,5,6,7,8,9,10,11,12,13

in1_size dd 40
in1_length dd 10
in1_type dd 4

SECTION .text
	global main
main:
	;INTRODUCTION

	;Output name and program title
	mov edx, my_name
	call WriteString
	call Crlf
	mov	 edx, prog_title
	call WriteString
	call Crlf

	;print the array using dumpmem, optional per the assignment
	mov esi, in1 ;offset
	mov ecx, DWORD [in1_length] ;LENGTHOF
	mov ebx, DWORD [in1_type] ;TYPE
	call DumpMem ;capitalization, see .inc

	;reverse the array
	mov edi, [in1_size] ;size
	add edi, in1 ;offset
	;null terminator
	sub edi, [in1_type] ;type
	mov esi, in1 ;offset
reverse:
	;mov end values
	mov eax, DWORD [edi]
	mov ebx, DWORD [esi]

	;switch end values
	mov DWORD [edi], ebx
	mov DWORD [esi], eax

	sub edi, [in1_type] ;TYPE
	add esi, [in1_type] ;type

	cmp esi, edi
	jle reverse

	;print the array using dumpmem
	mov esi, in1 ;offset
	mov ecx, DWORD [in1_length] ;LENGTHOF
	mov ebx, DWORD [in1_type] ;TYPE
	call DumpMem

ret
