TITLE assignment1				(main.asm)

INCLUDE Irvine32.inc

.data
val1 DWORD 20d
val2 DWORD 10d

.code

main PROC
	;we’re going to move the value to the register first
	mov eax, val1
	mov ebx, val2 
	;multiply
	mul ebx
	;subtract
	sub eax, ebx
	;the answer we get is equal to (20*10)-10

exit
main ENDP
END main
