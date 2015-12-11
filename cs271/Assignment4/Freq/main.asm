TITLE Assignment4-Freq					(main.asm)

; Author: Cody Malick
; Course/Project ID: CS 271, Assignment 4-D
; Date Created: 8/7/2014
; Date Last Modified: 8/7/2014
; Description: Frequency Table

INCLUDE Irvine32.inc

.data

;Text to be displayed throughout the program
prog_title BYTE "Assignment4-D-Freq",0dh,0ah,0 
my_name BYTE "Cody Malick",0dh,0ah,0
term_mess BYTE "**END OF PROGRAM**",0dh,0ah,0

;Integer Variables

;arrays
target BYTE "helloworld.thisisatestofascii",0
freq_table DWORD 256 DUP(0)

.code

check_array PROC
	;iterators
	mov edi, 0
	mov esi, TYPE freq_table

	;length of array to loop through
	mov ecx, LENGTHOF target

ch_array:
	movzx eax, [target+edi]

	;case_0, handles null terminator
	cmp eax, 0
	je case_0
	add [freq_table+eax*4], 1

case_0:
	;increment edi
	add edi, TYPE target
	loop ch_array
	ret
check_array ENDP

print_let PROC
	;iterators
	mov edi, 0
	mov esi, TYPE freq_table
	mov ecx, LENGTHOF freq_table

prnt_arry0:
	mov eax, [freq_table+edi]
	cmp eax, 0
	je zero_case0
	mov eax, LENGTHOF freq_table
	sub eax, ecx
	call WriteChar
zero_case0:
	add edi, esi
	loop prnt_arry0
	call Crlf
	ret
print_let ENDP


print_freq PROC
	mov edi, 0
	mov esi, TYPE freq_table
	mov ecx, LENGTHOF freq_table
prnt_arry:
	mov eax, 0
	mov eax, [freq_table+edi]
	;case 0, don't print
	cmp eax, 0
	je zero_case
	call WriteDec

zero_case:
	add edi, esi
	loop prnt_arry
	ret
print_freq ENDP

main PROC
	call Clrscr
	;This would only have a use, it seems, when there is something coming before this program is executed.

	;INTRODUCTION

	;Output name and program title
	mov edx,OFFSET my_name
	call WriteString

	mov	 edx,OFFSET prog_title
	call WriteString
	call Crlf
	mov edx, 0

;part 1

	;mov al, 'd'
	;call WriteChar
	;movzx eax, al
	;call WriteDec

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

exit
main ENDP

END main