;; pointers usage example

main:
mov eax, 5
mov a1, &eax	; move the address of eax in a1
mov a1, *a1		; dereference a1 in a1
end