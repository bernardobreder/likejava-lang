#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include "platform.h"
#include "vm.h"
#include "treemap.h"
#include "io.h"

int lng_vm(unsigned char* opcodes, char* class_boot, char* method_boot, uint32_t stack_max, size_t object_max, uint16_t object_buffer_max) {
	if (*opcodes++ != 0xBE) {
		return 0;
	}
	uint32_t n, class_count;
	lng_io_uint32_read_inline(opcodes, class_count);
	uint16_t class_fields[class_count];
	uint8_t* class_fields_types[class_count];
	for (n = 0; n < class_count; n++) {
		uint32_t pkg_size;
		uint16_t field_count, method_count;
		lng_io_uint32_read_inline(opcodes, pkg_size);
		opcodes += pkg_size;
		lng_io_uint16_read_inline(opcodes, field_count);
		class_fields[n] = field_count;
		lng_io_uint16_read_inline(opcodes, method_count);
	}
	uint32_t object_buffer_index[object_buffer_max];
	struct lng_vm_object* object_buffer[class_count][object_buffer_max];
	for (n = 0; n < class_count; n++) {
		class_fields[n] = 0;
		object_buffer_index[n] = 0;
	}
	unsigned char* opcodes_base = opcodes;
	int32_t int32value;
	uint32_t uint32value;
	int64_t int64value;
	//	uint64_t uint64value;
	double num64value;
	while (*opcodes != 0xFF) {
		switch (*opcodes++) {
		case LNG_OPCODE_INT_PREINC:
		case LNG_OPCODE_INT_PREDEC:
		case LNG_OPCODE_INT_POSINC:
		case LNG_OPCODE_INT_POSDEC:
			int32value = lng_io_uint32_see_inline(opcodes);
			*((uint32_t*) opcodes) = -int32value;
			opcodes += 4;
			break;
		case LNG_OPCODE_INT_LOAD:
			uint32value = lng_io_int32_see_inline(opcodes);
			*((uint32_t*) opcodes) = uint32value;
			opcodes += 4;
			break;
		case LNG_OPCODE_INT_SUM:
		case LNG_OPCODE_INT_SUB:
		case LNG_OPCODE_INT_MUL:
		case LNG_OPCODE_INT_DIV:
		case LNG_OPCODE_INT_EQ:
		case LNG_OPCODE_INT_NE:
		case LNG_OPCODE_INT_GT:
		case LNG_OPCODE_INT_GE:
		case LNG_OPCODE_INT_LT:
		case LNG_OPCODE_INT_LE:
			break;
		case LNG_OPCODE_INT_RET:
			opcodes += 4;
			break;
		case LNG_OPCODE_LONG_LOAD:
			*((uint64_t*) opcodes) = lng_io_uint64_see_inline(opcodes);
			opcodes += 8;
			break;
		case LNG_OPCODE_LONG_SUM:
			break;
		case LNG_OPCODE_LONG_SUB:
			break;
		case LNG_OPCODE_LONG_MUL:
			break;
		case LNG_OPCODE_LONG_DIV:
			break;
		case LNG_OPCODE_LONG_RET:
			opcodes += 8;
			break;
		case LNG_OPCODE_NUM_LOAD:
			opcodes += 8;
			break;
		case LNG_OPCODE_NUM_SUM:
			break;
		case LNG_OPCODE_NUM_SUB:
			break;
		case LNG_OPCODE_NUM_MUL:
			break;
		case LNG_OPCODE_NUM_DIV:
			break;
		case LNG_OPCODE_NUM_RET:
			opcodes += 8;
			break;
		case LNG_OPCODE_BOOL_TRUE:
			break;
		case LNG_OPCODE_BOOL_FALSE:
			break;
		case LNG_OPCODE_JMP:
		case LNG_OPCODE_JMP_TRUE:
		case LNG_OPCODE_JMP_FALSE:
			int32value = lng_io_int32_see_inline(opcodes);
			*((uint32_t*) opcodes) = int32value;
			opcodes += 4;
			break;
		case LNG_OPCODE_STACK_POP_ONE:
			break;
		case LNG_OPCODE_STACK_POP:
		case LNG_OPCODE_STACK_PUSH:
		case LNG_OPCODE_STACK_LOAD:
		case LNG_OPCODE_STACK_STORE:
			uint32value = lng_io_uint32_see_inline(opcodes);
			*((uint32_t*) opcodes) = uint32value;
			opcodes += 4;
			break;
		default: {
			return -1;
		}
		}
	}
	opcodes = opcodes_base;
	//	struct lng_vm_env env;
	//	env.opcodes_base = env.opcodes_current = opcodes;
	// Pilha de Objetos
	struct lng_vm_stack stack_base[stack_max];
	struct lng_vm_stack* stack_current = stack_base - 1;
	struct lng_vm_stack* stack_aux = 0;
	// Pilha de Metodos
	struct lng_vm_stack_method stack_method_base[stack_max];
	struct lng_vm_stack_method* stack_method_current = stack_method_base - 1;
	stack_method_current++;
	// Objetos da Memoria
	struct lng_vm_object object_base[object_max];
	// Objetos do Garbage Collector
	struct lng_vm_object* object_gc[object_max];
	struct lng_vm_object* object_current = object_base - 1;
	// Inicializa o Loop
	for (;;) {
		switch (*opcodes++) {
		case LNG_OPCODE_JMP:
			opcodes += 4 + *((int*) opcodes);
			break;
		case LNG_OPCODE_JMP_TRUE:
			if ((stack_current--)->int32) {
				opcodes += *((int*) opcodes);
			}
			opcodes += 4;
			break;
		case LNG_OPCODE_JMP_FALSE:
			if (!(stack_current--)->int32) {
				opcodes += *((int*) opcodes);
			}
			opcodes += 4;
			break;
		case LNG_OPCODE_INT_LOAD: {
			(++stack_current)->int32 = *((int*) opcodes);
			opcodes += 4;
			break;
		}
		case LNG_OPCODE_INT_SUM: {
			int32value = (stack_current--)->int32;
			stack_current->int32 += int32value;
			break;
		}
		case LNG_OPCODE_INT_SUB: {
			int32value = (stack_current--)->int32;
			stack_current->int32 -= int32value;
			break;
		}
		case LNG_OPCODE_INT_MUL: {
			int32value = (stack_current--)->int32;
			stack_current->int32 *= int32value;
			break;
		}
		case LNG_OPCODE_INT_DIV: {
			int32value = (stack_current--)->int32;
			stack_current->int32 /= int32value;
			break;
		}
		case LNG_OPCODE_INT_EQ: {
			int32value = (stack_current--)->int32;
			stack_current->int32 = stack_current->int32 == int32value;
			break;
		}
		case LNG_OPCODE_INT_NE: {
			int32value = (stack_current--)->int32;
			stack_current->int32 = stack_current->int32 != int32value;
			break;
		}
		case LNG_OPCODE_INT_GT: {
			int32value = (stack_current--)->int32;
			stack_current->int32 = stack_current->int32 > int32value;
			break;
		}
		case LNG_OPCODE_INT_GE: {
			int32value = (stack_current--)->int32;
			stack_current->int32 = stack_current->int32 >= int32value;
			break;
		}
		case LNG_OPCODE_INT_LT: {
			int32value = (stack_current--)->int32;
			stack_current->int32 = stack_current->int32 < int32value;
			break;
		}
		case LNG_OPCODE_INT_LE: {
			int32value = (stack_current--)->int32;
			stack_current->int32 = stack_current->int32 <= int32value;
			break;
		}
		case LNG_OPCODE_INT_PREINC: {
			stack_aux = stack_current + *((int32_t*) opcodes);
			stack_aux->int32++;
			*++stack_current = *stack_aux;
			opcodes += 4;
			break;
		}
		case LNG_OPCODE_INT_PREDEC: {
			stack_aux = stack_current + *((int32_t*) opcodes);
			stack_aux->int32--;
			*++stack_current = *stack_aux;
			opcodes += 4;
			break;
		}
		case LNG_OPCODE_INT_POSINC: {
			stack_aux = stack_current + *((int32_t*) opcodes);
			*++stack_current = *stack_aux;
			stack_aux->int32++;
			opcodes += 4;
			break;
		}
		case LNG_OPCODE_INT_POSDEC: {
			stack_aux = stack_current + *((int32_t*) opcodes);
			*++stack_current = *stack_aux;
			stack_aux->int32--;
			opcodes += 4;
			break;
		}
		case LNG_OPCODE_INT_RET: {
			if (stack_method_current == stack_method_base) {
				return stack_current->int32;
			} else {
				assert(0);
			}
			break;
		}
		case LNG_OPCODE_LONG_LOAD: {
			(++stack_current)->int64 = *((uint64_t*) opcodes);
			opcodes += 8;
			break;
		}
		case LNG_OPCODE_LONG_SUM: {
			int64value = (stack_current--)->int64;
			stack_current->int64 += int64value;
			break;
		}
		case LNG_OPCODE_LONG_SUB: {
			int64value = (stack_current--)->int64;
			stack_current->int64 -= int64value;
			break;
		}
		case LNG_OPCODE_LONG_MUL: {
			int64value = (stack_current--)->int64;
			stack_current->int64 *= int64value;
			break;
		}
		case LNG_OPCODE_LONG_DIV: {
			int64value = (stack_current--)->int64;
			stack_current->int64 /= int64value;
			break;
		}
		case LNG_OPCODE_LONG_RET: {
			//			return stack_current->int64;
			break;
		}
		case LNG_OPCODE_NUM_LOAD: {
			(++stack_current)->num64 = *((double*) opcodes);
			opcodes += 8;
			break;
		}
		case LNG_OPCODE_NUM_SUM: {
			num64value = (stack_current--)->num64;
			stack_current->num64 += num64value;
			break;
		}
		case LNG_OPCODE_NUM_SUB: {
			num64value = (stack_current--)->num64;
			stack_current->num64 -= num64value;
			break;
		}
		case LNG_OPCODE_NUM_MUL: {
			num64value = (stack_current--)->num64;
			stack_current->num64 *= num64value;
			break;
		}
		case LNG_OPCODE_NUM_DIV: {
			num64value = (stack_current--)->num64;
			stack_current->num64 /= num64value;
			break;
		}
		case LNG_OPCODE_NUM_RET: {
			//			return stack_current->num64;
			break;
		}
		case LNG_OPCODE_STACK_POP_ONE: {
			(stack_current--)->data = 0;
			break;
		}
		case LNG_OPCODE_STACK_POP: {
			uint32value = *((uint32_t*) opcodes);
			opcodes += 4;
			for (n = 0; n < uint32value; n++) {
				(stack_current--)->data = 0;
			}
			break;
		}
		case LNG_OPCODE_STACK_PUSH: {
			uint32value = *((uint32_t*) opcodes);
			opcodes += 4;
			stack_current += uint32value;
			break;
		}
		case LNG_OPCODE_STACK_LOAD: {
			int32value = *((uint32_t*) opcodes);
			opcodes += 4;
			stack_current[1] = stack_current[-int32value];
			stack_current++;
			break;
		}
		case LNG_OPCODE_STACK_STORE: {
			int32value = *((uint32_t*) opcodes);
			opcodes += 4;
			stack_current[-int32value] = *stack_current;
			stack_current--;
			break;
		}
		case LNG_OPCODE_BOOL_TRUE:
			(++stack_current)->int32 = 1;
			break;
		case LNG_OPCODE_BOOL_FALSE:
			(++stack_current)->int32 = 0;
			break;
			//		case LNG_OPCODE_STACK_GC:break;
		default: {
			return -1;
		}
		}
		if (0) {
			{
				uint32_t n, object_index = 0;
				{
					struct lng_vm_stack* stack_item = stack_base;
					while (stack_item <= stack_current) {
						if (stack_item->data) {
							object_gc[object_index++] = stack_item->data;
						}
						stack_item++;
					}
				}
				{
					struct lng_vm_object** object_gc_current = object_gc;
					for (n = 0; n < object_index; n++) {
						struct lng_vm_object* object = *object_gc_current++;
						if (!object->gc) {
							object->gc = 1;
							uint32_t classid = object->classid;
							uint8_t* types = class_fields_types[object->classid];
							uint8_t* fields = (uint8_t*) (object + 1);
							uint16_t m, msize = class_fields[classid];
							for (m = 0; m < msize; m++) {
								unsigned char type = *types++;
								switch (type) {
								case LNG_VM_TYPE_OBJ:
								case LNG_VM_TYPE_STR:
									object_gc[object_index++] = (void*) fields;
									fields += sizeof(void*);
									break;
								case LNG_VM_TYPE_BOOL:
									fields += sizeof(uint8_t);
									break;
								case LNG_VM_TYPE_INT:
									fields += sizeof(int32_t);
									break;
								case LNG_VM_TYPE_LONG:
									fields += sizeof(int64_t);
									break;
								case LNG_VM_TYPE_DOUBLE:
									fields += sizeof(double);
									break;
								default:
									return -1;
								}
							}
						}
					}
				}
			}
			struct lng_vm_object* object_item = object_base;
			while (object_item <= object_current) {
				if (object_item->gc) {
					object_item->gc = 0;
				} else {
					uint32_t classid = object_item->classid;
					object_buffer[classid][object_buffer_index[classid]++] = object_item;
				}
				object_item++;
			}
		}
	}
	return 0;
}
