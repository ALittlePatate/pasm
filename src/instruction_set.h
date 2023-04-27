#pragma once

typedef enum INSTRUCTIONS {
	ERROR_INSTRUCTION = -2,
	LABEL = -1,
	ADD,
	SUB,
	MOV,
	CMP,
	JE,
	JNE,
	JMP,
	JB,
	JNB,
	JA,
	JNA,
	RET,
	POP,
	PUSH,
	CALL
} INSTRUCTION;
