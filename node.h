#ifndef LNG_NODE_H
#define LNG_NODE_H

#include "compile.h"

#define LNG_NODE_UNIT_TYPE 1
#define LNG_NODE_BLOCK_TYPE 2
#define LNG_NODE_WHILE_TYPE 3
#define LNG_NODE_REPEAT_TYPE 4
#define LNG_NODE_FORN_TYPE 5
#define LNG_NODE_FOREACH_TYPE 6
#define LNG_NODE_IF_TYPE 7
#define LNG_NODE_EXPRESSION_TYPE 8
#define LNG_NODE_TERNARY_TYPE 9
#define LNG_NODE_ID_TYPE 10
#define LNG_NODE_INT_TYPE 11
#define LNG_NODE_STR_TYPE 12
#define LNG_NODE_BOOL_TYPE 13
#define LNG_NODE_AND_TYPE 14
#define LNG_NODE_OR_TYPE 15
#define LNG_NODE_EQ_TYPE 16
#define LNG_NODE_NE_TYPE 17
#define LNG_NODE_LT_TYPE 18
#define LNG_NODE_LE_TYPE 19
#define LNG_NODE_GT_TYPE 20
#define LNG_NODE_GE_TYPE 21
#define LNG_NODE_NOT_TYPE 22
#define LNG_NODE_SHL_TYPE 23
#define LNG_NODE_SHR_TYPE 24
#define LNG_NODE_SUM_TYPE 25
#define LNG_NODE_SUB_TYPE 26
#define LNG_NODE_MUL_TYPE 27
#define LNG_NODE_DIV_TYPE 28
#define LNG_NODE_MOD_TYPE 29
#define LNG_NODE_PREINC_TYPE 30
#define LNG_NODE_PREDEC_TYPE 31
#define LNG_NODE_POSINC_TYPE 32
#define LNG_NODE_POSDEC_TYPE 33
#define LNG_NODE_INT_TYPE_TYPE 34
#define LNG_NODE_STR_TYPE_TYPE 35
#define LNG_NODE_BOOL_TYPE_TYPE 36
#define LNG_NODE_PACKAGE_TYPE 37
#define LNG_NODE_IMPORT_TYPE 38
#define LNG_NODE_CLASS_TYPE 39
#define LNG_NODE_FIELD_TYPE 40
#define LNG_NODE_METHOD_TYPE 41
#define LNG_NODE_VARIABLE_TYPE 42
#define LNG_NODE_DECLARE_TYPE 43
#define LNG_NODE_ARRAY_GET_TYPE 44
#define LNG_NODE_ARRAY_SET_TYPE 45
#define LNG_NODE_ARRAY_LEN_TYPE 46
#define LNG_NODE_ARRAY_INC_TYPE 47
#define LNG_NODE_PARAMETER_TYPE 48
#define LNG_NODE_COMMA_TYPE 49
#define LNG_NODE_ASSIGN_TYPE 50
#define LNG_NODE_ARRAY_TYPE 51
#define LNG_NODE_NEG_TYPE 52
#define LNG_NODE_RETURN_TYPE 53

struct lng_compile_t {
	struct lng_syntax_t* syntax;
	struct lng_node_t* node;
	unsigned char test;
	unsigned long size;
	unsigned char* bytes_next;
	unsigned char* bytes;
	unsigned char reg;
	unsigned int stack_offset;
};

struct lng_node_t {
	struct lng_node_vtable_t* vtable;
	struct lng_node_t* parent;
	uint32_t bytes_sizeof;
};

struct lng_node_linked_t {
	struct lng_node_t* node;
	struct lng_node_linked_t* next;
	struct lng_node_linked_t* back;
};

struct lng_node_unit_t {
	struct lng_node_vtable_t* vtable;
	struct lng_node_t* parent;
	uint32_t bytes_sizeof;
	struct lng_node_package_t* package;
	struct lng_node_import_t* imports;
	unsigned int import_count;
	struct lng_node_class_t* clazz;
	const unsigned char* text;
	struct lng_token_t* tokens;
	const char* errors[32];
	unsigned char error_index;
};

struct lng_node_command_t {
	struct lng_node_vtable_t* vtable;
	struct lng_node_t* parent;
	uint32_t bytes_sizeof;
};

struct lng_node_block_t {
	struct lng_node_vtable_t* vtable;
	struct lng_node_t* parent;
	uint32_t bytes_sizeof;
	struct lng_node_command_t** cmds;
	struct lng_node_linked_t* declares;
};

struct lng_node_while_t {
	struct lng_node_vtable_t* vtable;
	struct lng_node_t* parent;
	uint32_t bytes_sizeof;
	struct lng_node_value_t* cond;
	struct lng_node_command_t* cmd;
};

struct lng_node_repeat_t {
	struct lng_node_vtable_t* vtable;
	struct lng_node_t* parent;
	uint32_t bytes_sizeof;
	struct lng_node_value_t* cond;
	struct lng_node_command_t* cmd;
};

struct lng_node_forn_t {
	struct lng_node_vtable_t* vtable;
	struct lng_node_t* parent;
	uint32_t bytes_sizeof;
	struct lng_node_command_t* init;
	struct lng_node_value_t* cond;
	struct lng_node_command_t* inc;
	struct lng_node_command_t* cmd;
};

struct lng_node_foreach_t {
	struct lng_node_vtable_t* vtable;
	struct lng_node_t* parent;
	uint32_t bytes_sizeof;
	struct lng_token_t* name;
	struct lng_node_value_t* begin;
	struct lng_node_value_t* end;
	struct lng_node_command_t* cmd;
};

struct lng_node_if_t {
	struct lng_node_vtable_t* vtable;
	struct lng_node_t* parent;
	uint32_t bytes_sizeof;
	struct lng_node_value_t* cond;
	struct lng_node_command_t* true_cmd;
	struct lng_node_command_t* false_cmd;
};

struct lng_node_return_t {
	struct lng_node_vtable_t* vtable;
	struct lng_node_t* parent;
	uint32_t bytes_sizeof;
	struct lng_node_value_t* left;
	uint32_t pop_size;
};

struct lng_node_expression_t {
	struct lng_node_vtable_t* vtable;
	struct lng_node_t* parent;
	uint32_t bytes_sizeof;
	struct lng_node_value_t* left;
};

struct lng_node_unary_t {
	struct lng_node_vtable_t* vtable;
	struct lng_node_t* parent;
	uint32_t bytes_sizeof;
	struct lng_node_value_t* left;
};

struct lng_node_binary_t {
	struct lng_node_vtable_t* vtable;
	struct lng_node_t* parent;
	uint32_t bytes_sizeof;
	struct lng_node_value_t* left;
	struct lng_node_value_t* right;
};

struct lng_node_ternary_t {
	struct lng_node_vtable_t* vtable;
	struct lng_node_t* parent;
	uint32_t bytes_sizeof;
	struct lng_node_value_t* left;
	struct lng_node_value_t* right;
	struct lng_node_value_t* center;
};

struct lng_node_primitive_t {
	struct lng_node_vtable_t* vtable;
	struct lng_node_t* parent;
	uint32_t bytes_sizeof;
};

struct lng_node_value_t {
	struct lng_node_vtable_t* vtable;
	struct lng_node_t* parent;
	uint32_t bytes_sizeof;
};

struct lng_node_id_t {
	struct lng_node_vtable_t* vtable;
	struct lng_node_t* parent;
	uint32_t bytes_sizeof;
	struct lng_token_t* name;
	uint32_t index;
};

struct lng_node_int_t {
	struct lng_node_vtable_t* vtable;
	struct lng_node_t* parent;
	uint32_t bytes_sizeof;
	int value;
};

struct lng_node_str_t {
	struct lng_node_vtable_t* vtable;
	struct lng_node_t* parent;
	uint32_t bytes_sizeof;
	int value;
};

struct lng_node_bool_t {
	struct lng_node_vtable_t* vtable;
	struct lng_node_t* parent;
	uint32_t bytes_sizeof;
	unsigned char value;
};

struct lng_node_and_t {
	struct lng_node_vtable_t* vtable;
	struct lng_node_t* parent;
	uint32_t bytes_sizeof;
	struct lng_node_value_t* left;
	struct lng_node_value_t* right;
};

struct lng_node_or_t {
	struct lng_node_vtable_t* vtable;
	struct lng_node_t* parent;
	uint32_t bytes_sizeof;
	struct lng_node_value_t* left;
	struct lng_node_value_t* right;
};

struct lng_node_eq_t {
	struct lng_node_vtable_t* vtable;
	struct lng_node_t* parent;
	uint32_t bytes_sizeof;
	struct lng_node_value_t* left;
	struct lng_node_value_t* right;
};

struct lng_node_ne_t {
	struct lng_node_vtable_t* vtable;
	struct lng_node_t* parent;
	uint32_t bytes_sizeof;
	struct lng_node_value_t* left;
	struct lng_node_value_t* right;
};

struct lng_node_ge_t {
	struct lng_node_vtable_t* vtable;
	struct lng_node_t* parent;
	uint32_t bytes_sizeof;
	struct lng_node_value_t* left;
	struct lng_node_value_t* right;
};

struct lng_node_gt_t {
	struct lng_node_vtable_t* vtable;
	struct lng_node_t* parent;
	uint32_t bytes_sizeof;
	struct lng_node_value_t* left;
	struct lng_node_value_t* right;
};

struct lng_node_lt_t {
	struct lng_node_vtable_t* vtable;
	struct lng_node_t* parent;
	uint32_t bytes_sizeof;
	struct lng_node_value_t* left;
	struct lng_node_value_t* right;
};

struct lng_node_le_t {
	struct lng_node_vtable_t* vtable;
	struct lng_node_t* parent;
	uint32_t bytes_sizeof;
	struct lng_node_value_t* left;
	struct lng_node_value_t* right;
};

struct lng_node_shl_t {
	struct lng_node_vtable_t* vtable;
	struct lng_node_t* parent;
	uint32_t bytes_sizeof;
	struct lng_node_value_t* left;
	struct lng_node_value_t* right;
};

struct lng_node_shr_t {
	struct lng_node_vtable_t* vtable;
	struct lng_node_t* parent;
	uint32_t bytes_sizeof;
	struct lng_node_value_t* left;
	struct lng_node_value_t* right;
};

struct lng_node_sum_t {
	struct lng_node_vtable_t* vtable;
	struct lng_node_t* parent;
	uint32_t bytes_sizeof;
	struct lng_node_value_t* left;
	struct lng_node_value_t* right;
};

struct lng_node_sub_t {
	struct lng_node_vtable_t* vtable;
	struct lng_node_t* parent;
	uint32_t bytes_sizeof;
	struct lng_node_value_t* left;
	struct lng_node_value_t* right;
};

struct lng_node_mul_t {
	struct lng_node_vtable_t* vtable;
	struct lng_node_t* parent;
	uint32_t bytes_sizeof;
	struct lng_node_value_t* left;
	struct lng_node_value_t* right;
};

struct lng_node_div_t {
	struct lng_node_vtable_t* vtable;
	struct lng_node_t* parent;
	uint32_t bytes_sizeof;
	struct lng_node_value_t* left;
	struct lng_node_value_t* right;
};

struct lng_node_not_t {
	struct lng_node_vtable_t* vtable;
	struct lng_node_t* parent;
	uint32_t bytes_sizeof;
	struct lng_node_value_t* left;
};

struct lng_node_neg_t {
	struct lng_node_vtable_t* vtable;
	struct lng_node_t* parent;
	uint32_t bytes_sizeof;
	struct lng_node_value_t* left;
};

struct lng_node_preinc_t {
	struct lng_node_vtable_t* vtable;
	struct lng_node_t* parent;
	uint32_t bytes_sizeof;
	struct lng_node_value_t* left;
	uint32_t index;
};

struct lng_node_predec_t {
	struct lng_node_vtable_t* vtable;
	struct lng_node_t* parent;
	uint32_t bytes_sizeof;
	struct lng_node_value_t* left;
	uint32_t index;
};

struct lng_node_posinc_t {
	struct lng_node_vtable_t* vtable;
	struct lng_node_t* parent;
	uint32_t bytes_sizeof;
	struct lng_node_value_t* left;
	uint32_t index;
};

struct lng_node_posdec_t {
	struct lng_node_vtable_t* vtable;
	struct lng_node_t* parent;
	uint32_t bytes_sizeof;
	struct lng_node_value_t* left;
	uint32_t index;
};

struct lng_node_type_t {
	struct lng_node_vtable_t* vtable;
	struct lng_node_t* parent;
	uint32_t bytes_sizeof;
};

struct lng_node_type_int_t {
	struct lng_node_vtable_t* vtable;
	struct lng_node_t* parent;
	uint32_t bytes_sizeof;
	struct lng_token_t* token;
};

struct lng_node_type_str_t {
	struct lng_node_vtable_t* vtable;
	struct lng_node_t* parent;
	uint32_t bytes_sizeof;
	struct lng_token_t* token;
};

struct lng_node_type_bool_t {
	struct lng_node_vtable_t* vtable;
	struct lng_node_t* parent;
	uint32_t bytes_sizeof;
	struct lng_token_t* token;
};

struct lng_node_package_t {
	struct lng_node_vtable_t* vtable;
	struct lng_node_t* parent;
	uint32_t bytes_sizeof;
	struct lng_token_t* name;
};

struct lng_node_import_t {
	struct lng_node_vtable_t* vtable;
	struct lng_node_t* parent;
	uint32_t bytes_sizeof;
	struct lng_token_t* name;
};

struct lng_node_assign_t {
	struct lng_node_vtable_t* vtable;
	struct lng_node_t* parent;
	uint32_t bytes_sizeof;
	struct lng_node_value_t* left;
	struct lng_node_value_t* value;
};

struct lng_node_declare_t {
	struct lng_node_vtable_t* vtable;
	struct lng_node_t* parent;
	uint32_t bytes_sizeof;
	struct lng_node_type_t* type;
	struct lng_token_t* name;
	struct lng_node_value_t* value;
	uint32_t index;
};

struct lng_node_array_t {
	struct lng_node_vtable_t* vtable;
	struct lng_node_t* parent;
	uint32_t bytes_sizeof;
	struct lng_node_value_t* left;
	struct lng_node_value_t* value;
};

struct lng_node_class_t {
	struct lng_node_vtable_t* vtable;
	struct lng_node_t* parent;
	uint32_t bytes_sizeof;
	struct lng_token_t* name;
	struct lng_node_method_t* methods;
	uint16_t method_count;
	struct lng_node_field_t* fields;
	uint16_t field_count;
};

struct lng_node_field_t {
	struct lng_node_vtable_t* vtable;
	struct lng_node_t* parent;
	uint32_t bytes_sizeof;
	struct lng_node_type_t* clazz;
	struct lng_token_t* name;
};

struct lng_node_method_t {
	struct lng_node_vtable_t* vtable;
	struct lng_node_t* parent;
	uint32_t bytes_sizeof;
	unsigned char is_static;
	struct lng_node_class_t* clazz;
	struct lng_token_t* name;
	struct lng_node_type_t* type;
	struct lng_node_param_t* parameters;
	struct lng_node_block_t* cmd;
	uint32_t declares;
};

struct lng_node_param_t {
	struct lng_node_vtable_t* vtable;
	struct lng_node_t* parent;
	uint32_t bytes_sizeof;
	struct lng_node_type_t* type;
	struct lng_token_t* name;
};

struct lng_node_vtable_t {
	unsigned char classid;
	void (*head)(struct lng_node_t*);
	void (*body)(struct lng_node_t*);
	void (*link)(struct lng_node_t*, struct lng_compile_t*);
	uint32_t (*size)(struct lng_node_t*);
	void (*build)(struct lng_node_t*, unsigned char** bytes);
};

struct lng_node_t* lng_node_find(struct lng_node_t* root, unsigned char type);

int32_t lng_node_declare_find(struct lng_node_t* parent, struct lng_token_t* name);

void lng_node_unit_head(struct lng_node_unit_t* self);

void lng_node_unit_body(struct lng_node_unit_t* self);

void lng_node_unit_link(struct lng_node_unit_t* self, struct lng_compile_t* compiler);

uint32_t lng_node_unit_size(struct lng_node_unit_t* self);

void lng_node_unit_build(struct lng_node_unit_t* self, unsigned char** bytes);

void lng_node_package_head(struct lng_node_package_t* self);

void lng_node_package_body(struct lng_node_package_t* self);

void lng_node_package_link(struct lng_node_package_t* self, struct lng_compile_t* compiler);

uint32_t lng_node_package_size(struct lng_node_unit_t* self);

void lng_node_package_build(struct lng_node_package_t* self, unsigned char** bytes);

void lng_node_import_head(struct lng_node_import_t* self);

void lng_node_import_body(struct lng_node_import_t* self);

void lng_node_import_link(struct lng_node_import_t* self, struct lng_compile_t* compiler);

uint32_t lng_node_import_size(struct lng_node_unit_t* self);

void lng_node_import_build(struct lng_node_import_t* self, unsigned char** bytes);

void lng_node_class_head(struct lng_node_class_t* self);

void lng_node_class_body(struct lng_node_class_t* self);

void lng_node_class_link(struct lng_node_class_t* self, struct lng_compile_t* compiler);

uint32_t lng_node_class_size(struct lng_node_class_t* self);

void lng_node_class_build(struct lng_node_class_t* self, unsigned char** bytes);

void lng_node_field_head(struct lng_node_field_t* self);

void lng_node_field_body(struct lng_node_field_t* self);

void lng_node_field_link(struct lng_node_field_t* self, struct lng_compile_t* compiler);

uint32_t lng_node_field_size(struct lng_node_field_t* self);

void lng_node_field_build(struct lng_node_field_t* self, unsigned char** bytes);

void lng_node_method_head(struct lng_node_method_t* self);

void lng_node_method_body(struct lng_node_method_t* self);

void lng_node_method_link(struct lng_node_method_t* self, struct lng_compile_t* compiler);

uint32_t lng_node_method_size(struct lng_node_method_t* self);

void lng_node_method_build(struct lng_node_method_t* self, unsigned char** bytes);

void lng_node_type_int_head(struct lng_node_type_int_t* self);

void lng_node_type_int_body(struct lng_node_type_int_t* self);

void lng_node_type_int_link(struct lng_node_type_int_t* self, struct lng_compile_t* compiler);

uint32_t lng_node_type_int_size(struct lng_node_type_int_t* self);

void lng_node_type_int_build(struct lng_node_type_int_t* self, unsigned char** bytes);

void lng_node_type_bool_head(struct lng_node_type_bool_t* self);

void lng_node_type_bool_body(struct lng_node_type_bool_t* self);

void lng_node_type_bool_link(struct lng_node_type_bool_t* self, struct lng_compile_t* compiler);

uint32_t lng_node_type_bool_size(struct lng_node_type_bool_t* self);

void lng_node_type_bool_build(struct lng_node_type_bool_t* self, unsigned char** bytes);

void lng_node_type_str_head(struct lng_node_type_str_t* self);

void lng_node_type_str_body(struct lng_node_type_str_t* self);

void lng_node_type_str_link(struct lng_node_type_str_t* self, struct lng_compile_t* compiler);

uint32_t lng_node_type_str_size(struct lng_node_type_str_t* self);

void lng_node_type_str_build(struct lng_node_type_str_t* self, unsigned char** bytes);

void lng_node_param_head(struct lng_node_param_t* self);

void lng_node_param_body(struct lng_node_param_t* self);

void lng_node_param_link(struct lng_node_param_t* self, struct lng_compile_t* compiler);

uint32_t lng_node_param_size(struct lng_node_unit_t* self);

void lng_node_param_build(struct lng_node_param_t* self, unsigned char** bytes);

void lng_node_block_head(struct lng_node_block_t* self);

void lng_node_block_body(struct lng_node_block_t* self);

void lng_node_block_link(struct lng_node_block_t* self, struct lng_compile_t* compiler);

uint32_t lng_node_block_size(struct lng_node_block_t* self);

void lng_node_block_build(struct lng_node_block_t* self, unsigned char** bytes);

void lng_node_return_head(struct lng_node_return_t* self);

void lng_node_return_body(struct lng_node_return_t* self);

void lng_node_return_link(struct lng_node_return_t* self, struct lng_compile_t* compiler);

uint32_t lng_node_return_size(struct lng_node_return_t* self);

void lng_node_return_build(struct lng_node_return_t* self, unsigned char** bytes);

void lng_node_expression_head(struct lng_node_expression_t* self);

void lng_node_expression_body(struct lng_node_expression_t* self);

void lng_node_expression_link(struct lng_node_expression_t* self, struct lng_compile_t* compiler);

uint32_t lng_node_expression_size(struct lng_node_expression_t* self);

void lng_node_expression_build(struct lng_node_expression_t* self, unsigned char** bytes);

void lng_node_declare_head(struct lng_node_declare_t* self);

void lng_node_declare_body(struct lng_node_declare_t* self);

void lng_node_declare_link(struct lng_node_declare_t* self, struct lng_compile_t* compiler);

uint32_t lng_node_declare_size(struct lng_node_declare_t* self);

void lng_node_declare_build(struct lng_node_declare_t* self, unsigned char** bytes);

void lng_node_int_head(struct lng_node_int_t* self);

void lng_node_int_body(struct lng_node_int_t* self);

void lng_node_int_link(struct lng_node_int_t* self, struct lng_compile_t* compiler);

uint32_t lng_node_int_size(struct lng_node_int_t* self);

void lng_node_int_build(struct lng_node_int_t* self, unsigned char** bytes);

void lng_node_str_head(struct lng_node_str_t* self);

void lng_node_str_body(struct lng_node_str_t* self);

void lng_node_str_link(struct lng_node_str_t* self, struct lng_compile_t* compiler);

uint32_t lng_node_str_size(struct lng_node_str_t* self);

void lng_node_str_build(struct lng_node_str_t* self, unsigned char** bytes);

void lng_node_array_head(struct lng_node_array_t* self);

void lng_node_array_body(struct lng_node_array_t* self);

void lng_node_array_link(struct lng_node_array_t* self, struct lng_compile_t* compiler);

uint32_t lng_node_array_size(struct lng_node_array_t* self);

void lng_node_array_build(struct lng_node_array_t* self, unsigned char** bytes);

void lng_node_assign_head(struct lng_node_assign_t* self);

void lng_node_assign_body(struct lng_node_assign_t* self);

void lng_node_assign_link(struct lng_node_assign_t* self, struct lng_compile_t* compiler);

uint32_t lng_node_assign_size(struct lng_node_assign_t* self);

void lng_node_assign_build(struct lng_node_assign_t* self, unsigned char** bytes);

void lng_node_bool_head(struct lng_node_bool_t* self);

void lng_node_bool_body(struct lng_node_bool_t* self);

void lng_node_bool_link(struct lng_node_bool_t* self, struct lng_compile_t* compiler);

uint32_t lng_node_bool_size(struct lng_node_bool_t* self);

void lng_node_bool_build(struct lng_node_bool_t* self, unsigned char** bytes);

void lng_node_neg_head(struct lng_node_neg_t* self);

void lng_node_neg_body(struct lng_node_neg_t* self);

void lng_node_neg_link(struct lng_node_neg_t* self, struct lng_compile_t* compiler);

uint32_t lng_node_neg_size(struct lng_node_neg_t* self);

void lng_node_neg_build(struct lng_node_neg_t* self, unsigned char** bytes);

void lng_node_id_head(struct lng_node_id_t* self);

void lng_node_id_body(struct lng_node_id_t* self);

void lng_node_id_link(struct lng_node_id_t* self, struct lng_compile_t* compiler);

uint32_t lng_node_id_size(struct lng_node_id_t* self);

void lng_node_id_build(struct lng_node_id_t* self, unsigned char** bytes);

void lng_node_ternary_head(struct lng_node_ternary_t* self);

void lng_node_ternary_body(struct lng_node_ternary_t* self);

void lng_node_ternary_link(struct lng_node_ternary_t* self, struct lng_compile_t* compiler);

uint32_t lng_node_ternary_size(struct lng_node_ternary_t* self);

void lng_node_ternary_build(struct lng_node_ternary_t* self, unsigned char** bytes);

void lng_node_and_head(struct lng_node_and_t* self);

void lng_node_and_body(struct lng_node_and_t* self);

void lng_node_and_link(struct lng_node_and_t* self, struct lng_compile_t* compiler);

uint32_t lng_node_and_size(struct lng_node_and_t* self);

void lng_node_and_build(struct lng_node_and_t* self, unsigned char** bytes);

void lng_node_or_head(struct lng_node_or_t* self);

void lng_node_or_body(struct lng_node_or_t* self);

void lng_node_or_link(struct lng_node_or_t* self, struct lng_compile_t* compiler);

uint32_t lng_node_or_size(struct lng_node_or_t* self);

void lng_node_or_build(struct lng_node_or_t* self, unsigned char** bytes);

void lng_node_eq_head(struct lng_node_eq_t* self);

void lng_node_eq_body(struct lng_node_eq_t* self);

void lng_node_eq_link(struct lng_node_eq_t* self, struct lng_compile_t* compiler);

uint32_t lng_node_eq_size(struct lng_node_eq_t* self);

void lng_node_eq_build(struct lng_node_eq_t* self, unsigned char** bytes);

void lng_node_ne_head(struct lng_node_ne_t* self);

void lng_node_ne_body(struct lng_node_ne_t* self);

void lng_node_ne_link(struct lng_node_ne_t* self, struct lng_compile_t* compiler);

uint32_t lng_node_ne_size(struct lng_node_ne_t* self);

void lng_node_ne_build(struct lng_node_ne_t* self, unsigned char** bytes);

void lng_node_gt_head(struct lng_node_gt_t* self);

void lng_node_gt_body(struct lng_node_gt_t* self);

void lng_node_gt_link(struct lng_node_gt_t* self, struct lng_compile_t* compiler);

uint32_t lng_node_gt_size(struct lng_node_gt_t* self);

void lng_node_gt_build(struct lng_node_gt_t* self, unsigned char** bytes);

void lng_node_ge_head(struct lng_node_ge_t* self);

void lng_node_ge_body(struct lng_node_ge_t* self);

void lng_node_ge_link(struct lng_node_ge_t* self, struct lng_compile_t* compiler);

uint32_t lng_node_ge_size(struct lng_node_ge_t* self);

void lng_node_ge_build(struct lng_node_ge_t* self, unsigned char** bytes);

void lng_node_lt_head(struct lng_node_lt_t* self);

void lng_node_lt_body(struct lng_node_lt_t* self);

void lng_node_lt_link(struct lng_node_lt_t* self, struct lng_compile_t* compiler);

uint32_t lng_node_lt_size(struct lng_node_lt_t* self);

void lng_node_lt_build(struct lng_node_lt_t* self, unsigned char** bytes);

void lng_node_le_head(struct lng_node_le_t* self);

void lng_node_le_body(struct lng_node_le_t* self);

void lng_node_le_link(struct lng_node_le_t* self, struct lng_compile_t* compiler);

uint32_t lng_node_le_size(struct lng_node_le_t* self);

void lng_node_le_build(struct lng_node_le_t* self, unsigned char** bytes);

void lng_node_sum_head(struct lng_node_sum_t* self);

void lng_node_sum_body(struct lng_node_sum_t* self);

void lng_node_sum_link(struct lng_node_sum_t* self, struct lng_compile_t* compiler);

uint32_t lng_node_sum_size(struct lng_node_sum_t* self);

void lng_node_sum_build(struct lng_node_sum_t* self, unsigned char** bytes);

void lng_node_sub_head(struct lng_node_sub_t* self);

void lng_node_sub_body(struct lng_node_sub_t* self);

void lng_node_sub_link(struct lng_node_sub_t* self, struct lng_compile_t* compiler);

uint32_t lng_node_sub_size(struct lng_node_sub_t* self);

void lng_node_sub_build(struct lng_node_sub_t* self, unsigned char** bytes);

void lng_node_mul_head(struct lng_node_mul_t* self);

void lng_node_mul_body(struct lng_node_mul_t* self);

void lng_node_mul_link(struct lng_node_mul_t* self, struct lng_compile_t* compiler);

uint32_t lng_node_mul_size(struct lng_node_mul_t* self);

void lng_node_mul_build(struct lng_node_mul_t* self, unsigned char** bytes);

void lng_node_div_head(struct lng_node_div_t* self);

void lng_node_div_body(struct lng_node_div_t* self);

void lng_node_div_link(struct lng_node_div_t* self, struct lng_compile_t* compiler);

uint32_t lng_node_div_size(struct lng_node_div_t* self);

void lng_node_div_build(struct lng_node_div_t* self, unsigned char** bytes);

void lng_node_not_head(struct lng_node_not_t* self);

void lng_node_not_body(struct lng_node_not_t* self);

void lng_node_not_link(struct lng_node_not_t* self, struct lng_compile_t* compiler);

uint32_t lng_node_not_size(struct lng_node_not_t* self);

void lng_node_not_build(struct lng_node_not_t* self, unsigned char** bytes);

void lng_node_preinc_head(struct lng_node_preinc_t* self);

void lng_node_preinc_body(struct lng_node_preinc_t* self);

void lng_node_preinc_link(struct lng_node_preinc_t* self, struct lng_compile_t* compiler);

uint32_t lng_node_preinc_size(struct lng_node_preinc_t* self);

void lng_node_preinc_build(struct lng_node_preinc_t* self, unsigned char** bytes);

void lng_node_predec_head(struct lng_node_predec_t* self);

void lng_node_predec_body(struct lng_node_predec_t* self);

void lng_node_predec_link(struct lng_node_predec_t* self, struct lng_compile_t* compiler);

uint32_t lng_node_predec_size(struct lng_node_predec_t* self);

void lng_node_predec_build(struct lng_node_predec_t* self, unsigned char** bytes);

void lng_node_posinc_head(struct lng_node_posinc_t* self);

void lng_node_posinc_body(struct lng_node_posinc_t* self);

void lng_node_posinc_link(struct lng_node_posinc_t* self, struct lng_compile_t* compiler);

uint32_t lng_node_posinc_size(struct lng_node_posinc_t* self);

void lng_node_posinc_build(struct lng_node_posinc_t* self, unsigned char** bytes);

void lng_node_posdec_head(struct lng_node_posdec_t* self);

void lng_node_posdec_body(struct lng_node_posdec_t* self);

void lng_node_posdec_link(struct lng_node_posdec_t* self, struct lng_compile_t* compiler);

uint32_t lng_node_posdec_size(struct lng_node_posdec_t* self);

void lng_node_posdec_build(struct lng_node_posdec_t* self, unsigned char** bytes);

void lng_node_if_head(struct lng_node_if_t* self);

void lng_node_if_body(struct lng_node_if_t* self);

void lng_node_if_link(struct lng_node_if_t* self, struct lng_compile_t* compiler);

uint32_t lng_node_if_size(struct lng_node_if_t* self);

void lng_node_if_build(struct lng_node_if_t* self, unsigned char** bytes);

void lng_node_while_head(struct lng_node_while_t* self);

void lng_node_while_body(struct lng_node_while_t* self);

void lng_node_while_link(struct lng_node_while_t* self, struct lng_compile_t* compiler);

uint32_t lng_node_while_size(struct lng_node_while_t* self);

void lng_node_while_build(struct lng_node_while_t* self, unsigned char** bytes);

void lng_node_repeat_head(struct lng_node_repeat_t* self);

void lng_node_repeat_body(struct lng_node_repeat_t* self);

void lng_node_repeat_link(struct lng_node_repeat_t* self, struct lng_compile_t* compiler);

uint32_t lng_node_repeat_size(struct lng_node_repeat_t* self);

void lng_node_repeat_build(struct lng_node_repeat_t* self, unsigned char** bytes);

void lng_node_forn_head(struct lng_node_forn_t* self);

void lng_node_forn_body(struct lng_node_forn_t* self);

void lng_node_forn_link(struct lng_node_forn_t* self, struct lng_compile_t* compiler);

uint32_t lng_node_forn_size(struct lng_node_forn_t* self);

void lng_node_forn_build(struct lng_node_forn_t* self, unsigned char** bytes);

void lng_node_foreach_head(struct lng_node_foreach_t* self);

void lng_node_foreach_body(struct lng_node_foreach_t* self);

void lng_node_foreach_link(struct lng_node_foreach_t* self, struct lng_compile_t* compiler);

uint32_t lng_node_foreach_size(struct lng_node_foreach_t* self);

void lng_node_foreach_build(struct lng_node_foreach_t* self, unsigned char** bytes);

unsigned char* lng_compile(struct lng_node_unit_t** nodes, uint8_t node_size);

struct lng_node_unit_t* lng_node(const unsigned char* text);

void lng_node_free(struct lng_node_unit_t* self);

#define lng_opcode_bool_true(BYTES) *((BYTES)++) = LNG_OPCODE_BOOL_TRUE;
#define lng_opcode_bool_false(BYTES) *((BYTES)++) = LNG_OPCODE_BOOL_FALSE;

#define lng_opcode_int_return(BYTES, VALUE) \
*((BYTES)++) = LNG_OPCODE_INT_RET; \
lng_io_int_write(BYTES, VALUE); \
BYTES += 4;

#define lng_opcode_int_load(BYTES, VALUE) \
*((BYTES)++) = LNG_OPCODE_INT_LOAD; \
lng_io_int_write(BYTES, VALUE); \
BYTES += 4;

#define lng_opcode_int_sum(BYTES) \
*((BYTES)++) = LNG_OPCODE_INT_SUM; \

#define lng_opcode_int_sub(BYTES) \
*((BYTES)++) = LNG_OPCODE_INT_SUB; \

#define lng_opcode_int_mul(BYTES) \
*((BYTES)++) = LNG_OPCODE_INT_MUL; \

#define lng_opcode_int_div(BYTES) \
*((BYTES)++) = LNG_OPCODE_INT_DIV; \

#define lng_opcode_int_eq(BYTES) \
*((BYTES)++) = LNG_OPCODE_INT_EQ; \

#define lng_opcode_int_ne(BYTES) \
*((BYTES)++) = LNG_OPCODE_INT_NE; \

#define lng_opcode_int_gt(BYTES) \
*((BYTES)++) = LNG_OPCODE_INT_GT; \

#define lng_opcode_int_ge(BYTES) \
*((BYTES)++) = LNG_OPCODE_INT_GE; \

#define lng_opcode_int_lt(BYTES) \
*((BYTES)++) = LNG_OPCODE_INT_LT; \

#define lng_opcode_int_le(BYTES) \
*((BYTES)++) = LNG_OPCODE_INT_LE; \

#define lng_opcode_int_preinc(BYTES, VALUE) \
*((BYTES)++) = LNG_OPCODE_INT_PREINC; \
lng_io_uint32_write(BYTES, VALUE); \
BYTES += 4;

#define lng_opcode_int_predec(BYTES, VALUE) \
*((BYTES)++) = LNG_OPCODE_INT_PREDEC; \
lng_io_uint32_write(BYTES, VALUE); \
BYTES += 4;

#define lng_opcode_int_posinc(BYTES, VALUE) \
*((BYTES)++) = LNG_OPCODE_INT_POSINC; \
lng_io_uint32_write(BYTES, VALUE); \
BYTES += 4;

#define lng_opcode_int_posdec(BYTES, VALUE) \
*((BYTES)++) = LNG_OPCODE_INT_POSDEC; \
lng_io_uint32_write(BYTES, VALUE); \
BYTES += 4;

#define lng_opcode_bool_or(BYTES) \
*((BYTES)++) = LNG_OPCODE_BOOL_OR; \

#define lng_opcode_bool_and(BYTES) \
*((BYTES)++) = LNG_OPCODE_BOOL_AND; \

#define lng_opcode_stack_pop(BYTES, VALUE) \
*((BYTES)++) = LNG_OPCODE_STACK_POP; \
lng_io_uint32_write(BYTES, VALUE); \
BYTES += 4;

#define lng_opcode_stack_popone(BYTES) \
*((BYTES)++) = LNG_OPCODE_STACK_POP_ONE;

#define lng_opcode_stack_store(BYTES, VALUE) \
*((BYTES)++) = LNG_OPCODE_STACK_STORE; \
lng_io_uint32_write(BYTES, VALUE); \
BYTES += 4;

#define lng_opcode_stack_load(BYTES, VALUE) \
*((BYTES)++) = LNG_OPCODE_STACK_LOAD; \
lng_io_uint32_write(BYTES, VALUE); \
BYTES += 4;

#define lng_opcode_stack_push(BYTES, VALUE) \
*((BYTES)++) = LNG_OPCODE_STACK_PUSH; \
lng_io_uint32_write(BYTES, VALUE); \
BYTES += 4;

#define lng_opcode_stack_POP(BYTES, VALUE) \
*((BYTES)++) = LNG_OPCODE_STACK_POP; \
lng_io_uint32_write(BYTES, VALUE); \
BYTES += 4;

#define lng_opcode_jmp_false(BYTES, VALUE) \
*((BYTES)++) = LNG_OPCODE_JMP_FALSE; \
lng_io_int_write(BYTES, VALUE); \
BYTES += 4;

#define lng_opcode_jmp_true(BYTES, VALUE) \
*((BYTES)++) = LNG_OPCODE_JMP_TRUE; \
lng_io_int_write(BYTES, VALUE); \
BYTES += 4;

#define lng_opcode_jmp(BYTES, VALUE) \
*((BYTES)++) = LNG_OPCODE_JMP; \
lng_io_int_write(BYTES, VALUE); \
BYTES += 4;

void lng_compile_test();

#endif
