;this program sums the rows of an array
;this program is a failed experiment.
extern printf

SECTION .data

fmt: db "%d",0

row_size: dw 12
row_index: dw 0

table dd 100, 200, 300, 1, 2, 3, 10, 20, 30

SECTION .text


calc_row_sum:

	pop eax ;row index
	pop ebx	;row size
	pop ecx ;table offset

	push dword [ecx]
	push fmt
	call printf
	add esp, 8

	push eax
	ret

	global main
main:

	push dword table
	push dword [row_size]
	push dword [row_index]
 
	;call calc_row_sum
	pop eax	
ret
