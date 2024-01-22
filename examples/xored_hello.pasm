; Demonstration of printing a xored string, the same base can be used for decrypting shellcode,
; then, an api can be created that will execute the shellcode ig

; decryption key is at the end of the string because 42 ^ 42 = 0 and we need this null byte
; a decent way to not do this is to write only the size of the string if we know it beforehand
set hello 66, 79, 70, 70, 69, 6, 10, 93, 69, 88, 70, 78, 10, 11, 42

main:
mov a1, hello

loop:
mov a2, *a1
xor a2, 42		; decryption key
mov a2, eax
push a2
push 1
call put

cmp a2, 0
jne 1
end

add a1, 8
jmp loop