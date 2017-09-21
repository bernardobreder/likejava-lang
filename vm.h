#ifndef VM_H
#define VM_H

#include <stdint.h>
#include "platform.h"

#define LNG_VM_TYPE_BOOL 1
#define LNG_VM_TYPE_INT 2
#define LNG_VM_TYPE_LONG 3
#define LNG_VM_TYPE_DOUBLE 5
#define LNG_VM_TYPE_STR 6
#define LNG_VM_TYPE_OBJ 7

#define LNG_OPCODE_STACK_POP 20
#define LNG_OPCODE_STACK_POP_ONE 21
#define LNG_OPCODE_STACK_PUSH 22
#define LNG_OPCODE_STACK_LOAD 23
#define LNG_OPCODE_STACK_STORE 24
#define LNG_OPCODE_STACK_GC 29

#define LNG_OPCODE_JMP 40
#define LNG_OPCODE_JMP_EQ 41
#define LNG_OPCODE_JMP_NE 42
#define LNG_OPCODE_JMP_LT 43
#define LNG_OPCODE_JMP_LE 44
#define LNG_OPCODE_JMP_GT 45
#define LNG_OPCODE_JMP_GE 46
#define LNG_OPCODE_JMP_TRUE 47
#define LNG_OPCODE_JMP_FALSE 48

#define LNG_VM_OPCODE_CALL 60
#define LNG_VM_OPCODE_CALL_FUNC 61

#define LNG_OPCODE_INT_LOAD 81
#define LNG_OPCODE_INT_RET 82
#define LNG_OPCODE_INT_SUM 83
#define LNG_OPCODE_INT_SUB 84
#define LNG_OPCODE_INT_MUL 85
#define LNG_OPCODE_INT_DIV 86
#define LNG_OPCODE_INT_PREINC 87
#define LNG_OPCODE_INT_PREDEC 88
#define LNG_OPCODE_INT_POSINC 89
#define LNG_OPCODE_INT_POSDEC 90
#define LNG_OPCODE_INT_EQ 91
#define LNG_OPCODE_INT_NE 92
#define LNG_OPCODE_INT_GT 93
#define LNG_OPCODE_INT_GE 94
#define LNG_OPCODE_INT_LT 95
#define LNG_OPCODE_INT_LE 96

#define LNG_OPCODE_LONG_LOAD 111
#define LNG_OPCODE_LONG_RET 112
#define LNG_OPCODE_LONG_SUM 113
#define LNG_OPCODE_LONG_SUB 114
#define LNG_OPCODE_LONG_MUL 115
#define LNG_OPCODE_LONG_DIV 116

#define LNG_OPCODE_NUM_LOAD 131
#define LNG_OPCODE_NUM_RET 132
#define LNG_OPCODE_NUM_SUM 133
#define LNG_OPCODE_NUM_SUB 134
#define LNG_OPCODE_NUM_MUL 135
#define LNG_OPCODE_NUM_DIV 136

#define LNG_OPCODE_BOOL_TRUE 150
#define LNG_OPCODE_BOOL_FALSE 151
#define LNG_OPCODE_BOOL_OR 152
#define LNG_OPCODE_BOOL_AND 153

#define LNG_VM_OPCODE_STR_MAX 170
#define LNG_VM_OPCODE_STR_DEF 171
#define LNG_VM_OPCODE_STR_LEN 172
#define LNG_VM_OPCODE_STR_LOAD 173

struct lng_vm_object {
	uint8_t gc;
	uint32_t classid;
};

struct lng_vm_stack {
	struct lng_vm_object* data;
	int32_t int32;
	int64_t int64;
	double num64;
};

struct lng_vm_stack_method {
    int a;
};

struct lng_vm_env {
	unsigned char* opcodes_base;
	unsigned char* opcodes_current;
	struct lng_vm_stack* stack_base;
	struct lng_vm_stack* stack_current;
};

int lng_vm(unsigned char* opcodes, char* class_boot, char* method_boot, uint32_t stack_max, size_t object_max, uint16_t object_buffer_max);

void lng_vm_test();

#endif
