#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "lexical.h"
#include "syntax.h"
#include "node.h"
#include "io.h"

#define lng_syntax_can(SELF, TYPE) (((SELF)->token++)->type == (TYPE))
#define lng_syntax_is(SELF, TYPE) ((SELF)->token->type == (TYPE))
#define lng_syntax_look(SELF, INDEX, TYPE) (((SELF)->token + (INDEX) < (SELF)->tokens + (SELF)->token_count) && (((SELF)->token + (INDEX))->type == (TYPE)))
#define lng_syntax_get(SELF) ((SELF)->token)
#define lng_syntax_next(SELF) ((SELF)->token++)

unsigned char lng_syntax_unit_test(struct lng_syntax_t* self);

struct lng_node_unit_t* lng_syntax_unit_build(struct lng_syntax_t* self);

unsigned char lng_syntax_package_test(struct lng_syntax_t* self);

struct lng_node_package_t* lng_syntax_package_build(struct lng_syntax_t* self);

unsigned char lng_syntax_imports_test(struct lng_syntax_t* self);

struct lng_node_import_t* lng_syntax_imports_build(struct lng_syntax_t* self, unsigned int* import_count);

unsigned char lng_syntax_class_test(struct lng_syntax_t* self);

struct lng_node_class_t* lng_syntax_class_build(struct lng_syntax_t* self);

unsigned char lng_syntax_type_is(struct lng_syntax_t* self);

unsigned char lng_syntax_type_test(struct lng_syntax_t* self);

struct lng_node_type_t* lng_syntax_type_build(struct lng_syntax_t* self);

unsigned char lng_syntax_parameters_test(struct lng_syntax_t* self);

struct lng_node_param_t* lng_syntax_parameters_build(struct lng_syntax_t* self);

unsigned char lng_syntax_command_test(struct lng_syntax_t* self);

struct lng_node_command_t* lng_syntax_command_build(struct lng_syntax_t* self);

struct lng_node_block_t* lng_syntax_block_build(struct lng_syntax_t* self);

unsigned char lng_syntax_block_test(struct lng_syntax_t* self);

unsigned char lng_syntax_expr_test(struct lng_syntax_t* self);

struct lng_node_value_t* lng_syntax_expr_build(struct lng_syntax_t* self);

unsigned char lng_syntax_return_test(struct lng_syntax_t* self);

struct lng_node_return_t* lng_syntax_return_build(struct lng_syntax_t* self);

unsigned char lng_syntax_if_test(struct lng_syntax_t* self);

struct lng_node_if_t* lng_syntax_if_build(struct lng_syntax_t* self);

unsigned char lng_syntax_while_test(struct lng_syntax_t* self);

struct lng_node_while_t* lng_syntax_while_build(struct lng_syntax_t* self);

unsigned char lng_syntax_repeat_test(struct lng_syntax_t* self);

struct lng_node_repeat_t* lng_syntax_repeat_build(struct lng_syntax_t* self);

unsigned char lng_syntax_for_test(struct lng_syntax_t* self);

struct lng_node_command_t* lng_syntax_for_build(struct lng_syntax_t* self);

unsigned char lng_syntax_declare_is(struct lng_syntax_t* self);

unsigned char lng_syntax_declare_test(struct lng_syntax_t* self);

struct lng_node_declare_t* lng_syntax_declare_build(struct lng_syntax_t* self);

unsigned char lng_syntax_expression_test(struct lng_syntax_t* self);

struct lng_node_expression_t* lng_syntax_expression_build(struct lng_syntax_t* self);

unsigned char lng_syntax_ternary_test(struct lng_syntax_t* self);

struct lng_node_value_t* lng_syntax_ternary_build(struct lng_syntax_t* self);

unsigned char lng_syntax_and_test(struct lng_syntax_t* self);

struct lng_node_value_t* lng_syntax_and_build(struct lng_syntax_t* self);

unsigned char lng_syntax_or_test(struct lng_syntax_t* self);

struct lng_node_value_t* lng_syntax_or_build(struct lng_syntax_t* self);

unsigned char lng_syntax_compare_test(struct lng_syntax_t* self);

struct lng_node_value_t* lng_syntax_compare_build(struct lng_syntax_t* self);

unsigned char lng_syntax_sum_test(struct lng_syntax_t* self);

struct lng_node_value_t* lng_syntax_sum_build(struct lng_syntax_t* self);

unsigned char lng_syntax_mul_test(struct lng_syntax_t* self);

struct lng_node_value_t* lng_syntax_mul_build(struct lng_syntax_t* self);

unsigned char lng_syntax_unary_test(struct lng_syntax_t* self);

struct lng_node_value_t* lng_syntax_unary_build(struct lng_syntax_t* self);

unsigned char lng_syntax_literal_test(struct lng_syntax_t* self);

struct lng_node_value_t* lng_syntax_literal_build(struct lng_syntax_t* self);

struct lng_node_vtable_t lng_node_unit_vtable = { //
		LNG_NODE_UNIT_TYPE, //
				(void (*)(struct lng_node_t*)) lng_node_unit_head, //
				(void (*)(struct lng_node_t*)) lng_node_unit_body, //
				(void (*)(struct lng_node_t*, struct lng_compile_t*)) lng_node_unit_link, //
				(uint32 (*)(struct lng_node_t*)) lng_node_unit_size, //
				(void (*)(struct lng_node_t*, unsigned char**)) lng_node_unit_build //
		};

struct lng_node_vtable_t lng_node_package_vtable = { //
		LNG_NODE_PACKAGE_TYPE, //
				(void (*)(struct lng_node_t*)) lng_node_package_head, //
				(void (*)(struct lng_node_t*)) lng_node_package_body, //
				(void (*)(struct lng_node_t*, struct lng_compile_t*)) lng_node_package_link, //
				(uint32 (*)(struct lng_node_t*)) lng_node_package_size, //
				(void (*)(struct lng_node_t*, unsigned char**)) lng_node_package_build //
		};

struct lng_node_vtable_t lng_node_import_vtable = { //
		LNG_NODE_IMPORT_TYPE, //
				(void (*)(struct lng_node_t*)) lng_node_import_head, //
				(void (*)(struct lng_node_t*)) lng_node_import_body, //
				(void (*)(struct lng_node_t*, struct lng_compile_t*)) lng_node_import_link, //
				(uint32 (*)(struct lng_node_t*)) lng_node_import_size, //
				(void (*)(struct lng_node_t*, unsigned char**)) lng_node_import_build //
		};

struct lng_node_vtable_t lng_node_class_vtable = { //
		LNG_NODE_CLASS_TYPE, //
				(void (*)(struct lng_node_t*)) lng_node_class_head, //
				(void (*)(struct lng_node_t*)) lng_node_class_body, //
				(void (*)(struct lng_node_t*, struct lng_compile_t*)) lng_node_class_link, //
				(uint32 (*)(struct lng_node_t*)) lng_node_class_size, //
				(void (*)(struct lng_node_t*, unsigned char**)) lng_node_class_build //
		};

struct lng_node_vtable_t lng_node_field_vtable = { //
		LNG_NODE_FIELD_TYPE, //
				(void (*)(struct lng_node_t*)) lng_node_field_head, //
				(void (*)(struct lng_node_t*)) lng_node_field_body, //
				(void (*)(struct lng_node_t*, struct lng_compile_t*)) lng_node_field_link, //
				(uint32 (*)(struct lng_node_t*)) lng_node_field_size, //
				(void (*)(struct lng_node_t*, unsigned char**)) lng_node_field_build //
		};

struct lng_node_vtable_t lng_node_method_vtable = { //
		LNG_NODE_METHOD_TYPE, //
				(void (*)(struct lng_node_t*)) lng_node_method_head, //
				(void (*)(struct lng_node_t*)) lng_node_method_body, //
				(void (*)(struct lng_node_t*, struct lng_compile_t*)) lng_node_method_link, //
				(uint32 (*)(struct lng_node_t*)) lng_node_method_size, //
				(void (*)(struct lng_node_t*, unsigned char**)) lng_node_method_build //
		};

struct lng_node_vtable_t lng_node_type_int_vtable = { //
		LNG_NODE_INT_TYPE_TYPE, //
				(void (*)(struct lng_node_t*)) lng_node_type_int_head, //
				(void (*)(struct lng_node_t*)) lng_node_type_int_body, //
				(void (*)(struct lng_node_t*, struct lng_compile_t*)) lng_node_type_int_link, //
				(uint32 (*)(struct lng_node_t*)) lng_node_type_int_size, //
				(void (*)(struct lng_node_t*, unsigned char**)) lng_node_type_int_build //
		};

struct lng_node_vtable_t lng_node_type_bool_vtable = { //
		LNG_NODE_BOOL_TYPE_TYPE, //
				(void (*)(struct lng_node_t*)) lng_node_type_bool_head, //
				(void (*)(struct lng_node_t*)) lng_node_type_bool_body, //
				(void (*)(struct lng_node_t*, struct lng_compile_t*)) lng_node_type_bool_link, //
				(uint32 (*)(struct lng_node_t*)) lng_node_type_bool_size, //
				(void (*)(struct lng_node_t*, unsigned char**)) lng_node_type_bool_build //
		};

struct lng_node_vtable_t lng_node_type_str_vtable = { //
		LNG_NODE_STR_TYPE_TYPE, //
				(void (*)(struct lng_node_t*)) lng_node_type_str_head, //
				(void (*)(struct lng_node_t*)) lng_node_type_str_body, //
				(void (*)(struct lng_node_t*, struct lng_compile_t*)) lng_node_type_str_link, //
				(uint32 (*)(struct lng_node_t*)) lng_node_type_str_size, //
				(void (*)(struct lng_node_t*, unsigned char**)) lng_node_type_str_build //
		};

struct lng_node_vtable_t lng_node_param_vtable = { //
		LNG_NODE_PARAMETER_TYPE, //
				(void (*)(struct lng_node_t*)) lng_node_param_head, //
				(void (*)(struct lng_node_t*)) lng_node_param_body, //
				(void (*)(struct lng_node_t*, struct lng_compile_t*)) lng_node_param_link, //
				(uint32 (*)(struct lng_node_t*)) lng_node_param_size, //
				(void (*)(struct lng_node_t*, unsigned char**)) lng_node_param_build //
		};

struct lng_node_vtable_t lng_node_expression_vtable = { //
		LNG_NODE_EXPRESSION_TYPE, //
				(void (*)(struct lng_node_t*)) lng_node_expression_head, //
				(void (*)(struct lng_node_t*)) lng_node_expression_body, //
				(void (*)(struct lng_node_t*, struct lng_compile_t*)) lng_node_expression_link, //
				(uint32 (*)(struct lng_node_t*)) lng_node_expression_size, //
				(void (*)(struct lng_node_t*, unsigned char**)) lng_node_expression_build //
		};

struct lng_node_vtable_t lng_node_declare_vtable = { //
		LNG_NODE_DECLARE_TYPE, //
				(void (*)(struct lng_node_t*)) lng_node_declare_head, //
				(void (*)(struct lng_node_t*)) lng_node_declare_body, //
				(void (*)(struct lng_node_t*, struct lng_compile_t*)) lng_node_declare_link, //
				(uint32 (*)(struct lng_node_t*)) lng_node_declare_size, //
				(void (*)(struct lng_node_t*, unsigned char**)) lng_node_declare_build //
		};

struct lng_node_vtable_t lng_node_return_vtable = { //
		LNG_NODE_RETURN_TYPE, //
				(void (*)(struct lng_node_t*)) lng_node_return_head, //
				(void (*)(struct lng_node_t*)) lng_node_return_body, //
				(void (*)(struct lng_node_t*, struct lng_compile_t*)) lng_node_return_link, //
				(uint32 (*)(struct lng_node_t*)) lng_node_return_size, //
				(void (*)(struct lng_node_t*, unsigned char**)) lng_node_return_build //
		};

struct lng_node_vtable_t lng_node_block_vtable = { //
		LNG_NODE_BLOCK_TYPE, //
				(void (*)(struct lng_node_t*)) lng_node_block_head, //
				(void (*)(struct lng_node_t*)) lng_node_block_body, //
				(void (*)(struct lng_node_t*, struct lng_compile_t*)) lng_node_block_link, //
				(uint32 (*)(struct lng_node_t*)) lng_node_block_size, //
				(void (*)(struct lng_node_t*, unsigned char**)) lng_node_block_build //
		};

struct lng_node_vtable_t lng_node_if_vtable = { //
		LNG_NODE_IF_TYPE, //
				(void (*)(struct lng_node_t*)) lng_node_if_head, //
				(void (*)(struct lng_node_t*)) lng_node_if_body, //
				(void (*)(struct lng_node_t*, struct lng_compile_t*)) lng_node_if_link, //
				(uint32 (*)(struct lng_node_t*)) lng_node_if_size, //
				(void (*)(struct lng_node_t*, unsigned char**)) lng_node_if_build //
		};

struct lng_node_vtable_t lng_node_while_vtable = { //
		LNG_NODE_WHILE_TYPE, //
				(void (*)(struct lng_node_t*)) lng_node_while_head, //
				(void (*)(struct lng_node_t*)) lng_node_while_body, //
				(void (*)(struct lng_node_t*, struct lng_compile_t*)) lng_node_while_link, //
				(uint32 (*)(struct lng_node_t*)) lng_node_while_size, //
				(void (*)(struct lng_node_t*, unsigned char**)) lng_node_while_build //
		};

struct lng_node_vtable_t lng_node_repeat_vtable = { //
		LNG_NODE_REPEAT_TYPE, //
				(void (*)(struct lng_node_t*)) lng_node_repeat_head, //
				(void (*)(struct lng_node_t*)) lng_node_repeat_body, //
				(void (*)(struct lng_node_t*, struct lng_compile_t*)) lng_node_repeat_link, //
				(uint32 (*)(struct lng_node_t*)) lng_node_repeat_size, //
				(void (*)(struct lng_node_t*, unsigned char**)) lng_node_repeat_build //
		};

struct lng_node_vtable_t lng_node_forn_vtable = { //
		LNG_NODE_FORN_TYPE, //
				(void (*)(struct lng_node_t*)) lng_node_forn_head, //
				(void (*)(struct lng_node_t*)) lng_node_forn_body, //
				(void (*)(struct lng_node_t*, struct lng_compile_t*)) lng_node_forn_link, //
				(uint32 (*)(struct lng_node_t*)) lng_node_forn_size, //
				(void (*)(struct lng_node_t*, unsigned char**)) lng_node_forn_build //
		};

struct lng_node_vtable_t lng_node_foreach_vtable = { //
		LNG_NODE_FOREACH_TYPE, //
				(void (*)(struct lng_node_t*)) lng_node_foreach_head, //
				(void (*)(struct lng_node_t*)) lng_node_foreach_body, //
				(void (*)(struct lng_node_t*, struct lng_compile_t*)) lng_node_foreach_link, //
				(uint32 (*)(struct lng_node_t*)) lng_node_foreach_size, //
				(void (*)(struct lng_node_t*, unsigned char**)) lng_node_foreach_build //
		};

struct lng_node_vtable_t lng_node_id_vtable = { //
		LNG_NODE_ID_TYPE, //
				(void (*)(struct lng_node_t*)) lng_node_id_head, //
				(void (*)(struct lng_node_t*)) lng_node_id_body, //
				(void (*)(struct lng_node_t*, struct lng_compile_t*)) lng_node_id_link, //
				(uint32 (*)(struct lng_node_t*)) lng_node_id_size, //
				(void (*)(struct lng_node_t*, unsigned char**)) lng_node_id_build //
		};

struct lng_node_vtable_t lng_node_int_vtable = { //
		LNG_NODE_INT_TYPE, //
				(void (*)(struct lng_node_t*)) lng_node_int_head, //
				(void (*)(struct lng_node_t*)) lng_node_int_body, //
				(void (*)(struct lng_node_t*, struct lng_compile_t*)) lng_node_int_link, //
				(uint32 (*)(struct lng_node_t*)) lng_node_int_size, //
				(void (*)(struct lng_node_t*, unsigned char**)) lng_node_int_build //
		};

struct lng_node_vtable_t lng_node_bool_vtable = { //
		LNG_NODE_BOOL_TYPE, //
				(void (*)(struct lng_node_t*)) lng_node_bool_head, //
				(void (*)(struct lng_node_t*)) lng_node_bool_body, //
				(void (*)(struct lng_node_t*, struct lng_compile_t*)) lng_node_bool_link, //
				(uint32 (*)(struct lng_node_t*)) lng_node_bool_size, //
				(void (*)(struct lng_node_t*, unsigned char**)) lng_node_bool_build //
		};

struct lng_node_vtable_t lng_node_str_vtable = { //
		LNG_NODE_STR_TYPE, //
				(void (*)(struct lng_node_t*)) lng_node_str_head, //
				(void (*)(struct lng_node_t*)) lng_node_str_body, //
				(void (*)(struct lng_node_t*, struct lng_compile_t*)) lng_node_str_link, //
				(uint32 (*)(struct lng_node_t*)) lng_node_str_size, //
				(void (*)(struct lng_node_t*, unsigned char**)) lng_node_str_build //
		};

struct lng_node_vtable_t lng_node_assign_vtable = { //
		LNG_NODE_ASSIGN_TYPE, //
				(void (*)(struct lng_node_t*)) lng_node_assign_head, //
				(void (*)(struct lng_node_t*)) lng_node_assign_body, //
				(void (*)(struct lng_node_t*, struct lng_compile_t*)) lng_node_assign_link, //
				(uint32 (*)(struct lng_node_t*)) lng_node_assign_size, //
				(void (*)(struct lng_node_t*, unsigned char**)) lng_node_assign_build //
		};

struct lng_node_vtable_t lng_node_array_vtable = { //
		LNG_NODE_ARRAY_TYPE, //
				(void (*)(struct lng_node_t*)) lng_node_array_head, //
				(void (*)(struct lng_node_t*)) lng_node_array_body, //
				(void (*)(struct lng_node_t*, struct lng_compile_t*)) lng_node_array_link, //
				(uint32 (*)(struct lng_node_t*)) lng_node_array_size, //
				(void (*)(struct lng_node_t*, unsigned char**)) lng_node_array_build //
		};

struct lng_node_vtable_t lng_node_neg_vtable = { //
		LNG_NODE_NEG_TYPE, //
				(void (*)(struct lng_node_t*)) lng_node_neg_head, //
				(void (*)(struct lng_node_t*)) lng_node_neg_body, //
				(void (*)(struct lng_node_t*, struct lng_compile_t*)) lng_node_neg_link, //
				(uint32 (*)(struct lng_node_t*)) lng_node_neg_size, //
				(void (*)(struct lng_node_t*, unsigned char**)) lng_node_neg_build //
		};

struct lng_node_vtable_t lng_node_ternary_vtable = { //
		LNG_NODE_TERNARY_TYPE, //
				(void (*)(struct lng_node_t*)) lng_node_ternary_head, //
				(void (*)(struct lng_node_t*)) lng_node_ternary_body, //
				(void (*)(struct lng_node_t*, struct lng_compile_t*)) lng_node_ternary_link, //
				(uint32 (*)(struct lng_node_t*)) lng_node_ternary_size, //
				(void (*)(struct lng_node_t*, unsigned char**)) lng_node_ternary_build //
		};

struct lng_node_vtable_t lng_node_and_vtable = { //
		LNG_NODE_AND_TYPE, //
				(void (*)(struct lng_node_t*)) lng_node_and_head, //
				(void (*)(struct lng_node_t*)) lng_node_and_body, //
				(void (*)(struct lng_node_t*, struct lng_compile_t*)) lng_node_and_link, //
				(uint32 (*)(struct lng_node_t*)) lng_node_and_size, //
				(void (*)(struct lng_node_t*, unsigned char**)) lng_node_and_build //
		};

struct lng_node_vtable_t lng_node_or_vtable = { //
		LNG_NODE_OR_TYPE, //
				(void (*)(struct lng_node_t*)) lng_node_or_head, //
				(void (*)(struct lng_node_t*)) lng_node_or_body, //
				(void (*)(struct lng_node_t*, struct lng_compile_t*)) lng_node_or_link, //
				(uint32 (*)(struct lng_node_t*)) lng_node_or_size, //
				(void (*)(struct lng_node_t*, unsigned char**)) lng_node_or_build //
		};

struct lng_node_vtable_t lng_node_eq_vtable = { //
		LNG_NODE_EQ_TYPE, //
				(void (*)(struct lng_node_t*)) lng_node_eq_head, //
				(void (*)(struct lng_node_t*)) lng_node_eq_body, //
				(void (*)(struct lng_node_t*, struct lng_compile_t*)) lng_node_eq_link, //
				(uint32 (*)(struct lng_node_t*)) lng_node_eq_size, //
				(void (*)(struct lng_node_t*, unsigned char**)) lng_node_eq_build //
		};

struct lng_node_vtable_t lng_node_ne_vtable = { //
		LNG_NODE_NE_TYPE, //
				(void (*)(struct lng_node_t*)) lng_node_ne_head, //
				(void (*)(struct lng_node_t*)) lng_node_ne_body, //
				(void (*)(struct lng_node_t*, struct lng_compile_t*)) lng_node_ne_link, //
				(uint32 (*)(struct lng_node_t*)) lng_node_ne_size, //
				(void (*)(struct lng_node_t*, unsigned char**)) lng_node_ne_build //
		};

struct lng_node_vtable_t lng_node_gt_vtable = { //
		LNG_NODE_GT_TYPE, //
				(void (*)(struct lng_node_t*)) lng_node_gt_head, //
				(void (*)(struct lng_node_t*)) lng_node_gt_body, //
				(void (*)(struct lng_node_t*, struct lng_compile_t*)) lng_node_gt_link, //
				(uint32 (*)(struct lng_node_t*)) lng_node_gt_size, //
				(void (*)(struct lng_node_t*, unsigned char**)) lng_node_gt_build //
		};

struct lng_node_vtable_t lng_node_ge_vtable = { //
		LNG_NODE_GE_TYPE, //
				(void (*)(struct lng_node_t*)) lng_node_ge_head, //
				(void (*)(struct lng_node_t*)) lng_node_ge_body, //
				(void (*)(struct lng_node_t*, struct lng_compile_t*)) lng_node_ge_link, //
				(uint32 (*)(struct lng_node_t*)) lng_node_ge_size, //
				(void (*)(struct lng_node_t*, unsigned char**)) lng_node_ge_build //
		};

struct lng_node_vtable_t lng_node_lt_vtable = { //
		LNG_NODE_LT_TYPE, //
				(void (*)(struct lng_node_t*)) lng_node_lt_head, //
				(void (*)(struct lng_node_t*)) lng_node_lt_body, //
				(void (*)(struct lng_node_t*, struct lng_compile_t*)) lng_node_lt_link, //
				(uint32 (*)(struct lng_node_t*)) lng_node_lt_size, //
				(void (*)(struct lng_node_t*, unsigned char**)) lng_node_lt_build //
		};

struct lng_node_vtable_t lng_node_le_vtable = { //
		LNG_NODE_LE_TYPE, //
				(void (*)(struct lng_node_t*)) lng_node_le_head, //
				(void (*)(struct lng_node_t*)) lng_node_le_body, //
				(void (*)(struct lng_node_t*, struct lng_compile_t*)) lng_node_le_link, //
				(uint32 (*)(struct lng_node_t*)) lng_node_le_size, //
				(void (*)(struct lng_node_t*, unsigned char**)) lng_node_le_build //
		};

struct lng_node_vtable_t lng_node_sum_vtable = { //
		LNG_NODE_SUM_TYPE, //
				(void (*)(struct lng_node_t*)) lng_node_sum_head, //
				(void (*)(struct lng_node_t*)) lng_node_sum_body, //
				(void (*)(struct lng_node_t*, struct lng_compile_t*)) lng_node_sum_link, //
				(uint32 (*)(struct lng_node_t*)) lng_node_sum_size, //
				(void (*)(struct lng_node_t*, unsigned char**)) lng_node_sum_build //
		};

struct lng_node_vtable_t lng_node_sub_vtable = { //
		LNG_NODE_SUB_TYPE, //
				(void (*)(struct lng_node_t*)) lng_node_sub_head, //
				(void (*)(struct lng_node_t*)) lng_node_sub_body, //
				(void (*)(struct lng_node_t*, struct lng_compile_t*)) lng_node_sub_link, //
				(uint32 (*)(struct lng_node_t*)) lng_node_sub_size, //
				(void (*)(struct lng_node_t*, unsigned char**)) lng_node_sub_build //
		};

struct lng_node_vtable_t lng_node_mul_vtable = { //
		LNG_NODE_MUL_TYPE, //
				(void (*)(struct lng_node_t*)) lng_node_mul_head, //
				(void (*)(struct lng_node_t*)) lng_node_mul_body, //
				(void (*)(struct lng_node_t*, struct lng_compile_t*)) lng_node_mul_link, //
				(uint32 (*)(struct lng_node_t*)) lng_node_mul_size, //
				(void (*)(struct lng_node_t*, unsigned char**)) lng_node_mul_build //
		};

struct lng_node_vtable_t lng_node_div_vtable = { //
		LNG_NODE_DIV_TYPE, //
				(void (*)(struct lng_node_t*)) lng_node_div_head, //
				(void (*)(struct lng_node_t*)) lng_node_div_body, //
				(void (*)(struct lng_node_t*, struct lng_compile_t*)) lng_node_div_link, //
				(uint32 (*)(struct lng_node_t*)) lng_node_div_size, //
				(void (*)(struct lng_node_t*, unsigned char**)) lng_node_div_build //
		};

struct lng_node_vtable_t lng_node_not_vtable = { //
		LNG_NODE_NOT_TYPE, //
				(void (*)(struct lng_node_t*)) lng_node_not_head, //
				(void (*)(struct lng_node_t*)) lng_node_not_body, //
				(void (*)(struct lng_node_t*, struct lng_compile_t*)) lng_node_not_link, //
				(uint32 (*)(struct lng_node_t*)) lng_node_not_size, //
				(void (*)(struct lng_node_t*, unsigned char**)) lng_node_not_build //
		};

struct lng_node_vtable_t lng_node_preinc_vtable = { //
		LNG_NODE_PREINC_TYPE, //
				(void (*)(struct lng_node_t*)) lng_node_preinc_head, //
				(void (*)(struct lng_node_t*)) lng_node_preinc_body, //
				(void (*)(struct lng_node_t*, struct lng_compile_t*)) lng_node_preinc_link, //
				(uint32 (*)(struct lng_node_t*)) lng_node_preinc_size, //
				(void (*)(struct lng_node_t*, unsigned char**)) lng_node_preinc_build //
		};

struct lng_node_vtable_t lng_node_predec_vtable = { //
		LNG_NODE_PREDEC_TYPE, //
				(void (*)(struct lng_node_t*)) lng_node_predec_head, //
				(void (*)(struct lng_node_t*)) lng_node_predec_body, //
				(void (*)(struct lng_node_t*, struct lng_compile_t*)) lng_node_predec_link, //
				(uint32 (*)(struct lng_node_t*)) lng_node_predec_size, //
				(void (*)(struct lng_node_t*, unsigned char**)) lng_node_predec_build //
		};

struct lng_node_vtable_t lng_node_posinc_vtable = { //
		LNG_NODE_POSINC_TYPE, //
				(void (*)(struct lng_node_t*)) lng_node_posinc_head, //
				(void (*)(struct lng_node_t*)) lng_node_posinc_body, //
				(void (*)(struct lng_node_t*, struct lng_compile_t*)) lng_node_posinc_link, //
				(uint32 (*)(struct lng_node_t*)) lng_node_posinc_size, //
				(void (*)(struct lng_node_t*, unsigned char**)) lng_node_posinc_build //
		};

struct lng_node_vtable_t lng_node_posdec_vtable = { //
		LNG_NODE_POSDEC_TYPE, //
				(void (*)(struct lng_node_t*)) lng_node_posdec_head, //
				(void (*)(struct lng_node_t*)) lng_node_posdec_body, //
				(void (*)(struct lng_node_t*, struct lng_compile_t*)) lng_node_posdec_link, //
				(uint32 (*)(struct lng_node_t*)) lng_node_posdec_size, //
				(void (*)(struct lng_node_t*, unsigned char**)) lng_node_posdec_build //
		};

struct lng_node_unit_t* lng_syntax_unit(struct lng_syntax_t* self) {
	self->test = 1;
	self->memory_size = 0;
	unsigned char error = lng_syntax_unit_test(self);
	if (error) {
		return 0;
	}
	self->test = 0;
	self->token = self->tokens;
	assert(self->memory_size);
	unsigned char* memory = (unsigned char*) calloc(self->memory_size, 1);
	if (!memory) {
		return 0;
	}
	self->memory = memory;
	struct lng_node_unit_t* unit = lng_syntax_unit_build(self);
	unit->parent = 0;
	//    size_t delta = self->memory - memory;
	assert(self->memory == memory + self->memory_size);
	assert((void*)memory == (void*)unit);
	return unit;
}

struct lng_syntax_t* lng_syntax_new(const unsigned char* text) {
	struct lng_syntax_t* self = (struct lng_syntax_t*) calloc(1, sizeof(struct lng_syntax_t));
	if (!self) {
		return 0;
	}
	self->tokens = self->token = lng_lexical(text, &self->token_count);
	if (!self->tokens) {
		free(self);
		return 0;
	}
	self->text = text;
	self->test = 1;
	return self;
}

void lng_syntax_free(struct lng_syntax_t* self) {
	free(self->tokens);
	free(self);
}

unsigned char lng_syntax_id_test(struct lng_syntax_t* self) {
	if (!lng_syntax_is(self, LNG_LEXICAL_ID)) {
		return 1;
	}
	lng_syntax_next(self);
	return 0;
}

unsigned char lng_syntax_ids_test(struct lng_syntax_t* self) {
	for (;;) {
		if (!lng_syntax_is(self, LNG_LEXICAL_ID)) {
			return 1;
		}
		lng_syntax_next(self);
		if (!lng_syntax_is(self, '.')) {
			break;
		}
		lng_syntax_next(self);
	}
	return 0;
}

struct lng_token_t* lng_syntax_ids_build(struct lng_syntax_t* self) {
	struct lng_token_t* token = lng_syntax_next(self);
	while (lng_syntax_is(self, '.')) {
		lng_syntax_next(self);
		token = lng_syntax_next(self);
	}
	return token;
}

unsigned char lng_syntax_unit_test(struct lng_syntax_t* self) {
	self->memory_size += sizeof(struct lng_node_unit_t);
	if (lng_syntax_package_test(self)) {
		return 1;
	}
	if (lng_syntax_imports_test(self)) {
		return 1;
	}
	if (lng_syntax_class_test(self)) {
		return 1;
	}
	if (!lng_syntax_is(self, LNG_LEXICAL_EOF)) {
		return 1;
	}
	return 0;
}

struct lng_node_unit_t* lng_syntax_unit_build(struct lng_syntax_t* self) {
	struct lng_node_unit_t* node = (struct lng_node_unit_t*) self->memory;
	self->memory += sizeof(struct lng_node_unit_t);
	node->vtable = &lng_node_unit_vtable;
	struct lng_node_package_t* package = lng_syntax_package_build(self);
	struct lng_node_import_t* imports = lng_syntax_imports_build(self, &node->import_count);
	struct lng_node_class_t* clazz = lng_syntax_class_build(self);
	node->package = package;
	node->package->parent = (struct lng_node_t*) node;
	node->imports = imports;
	node->imports->parent = (struct lng_node_t*) node;
	node->clazz = clazz;
	node->clazz->parent = (struct lng_node_t*) node;
	return node;
}

unsigned char lng_syntax_package_test(struct lng_syntax_t* self) {
	self->memory_size += sizeof(struct lng_node_package_t);
	if (!lng_syntax_is(self, LNG_LEXICAL_PACKAGE_WORD)) {
		return 1;
	}
	lng_syntax_next(self);
	if (lng_syntax_ids_test(self)) {
		return 1;
	}
	return 0;
}

struct lng_node_package_t* lng_syntax_package_build(struct lng_syntax_t* self) {
	lng_syntax_next(self);
	struct lng_token_t* token = lng_syntax_ids_build(self);
	struct lng_node_package_t* node = (struct lng_node_package_t*) self->memory;
	self->memory += sizeof(struct lng_node_package_t);
	node->vtable = &lng_node_package_vtable;
	node->name = token;
	return node;
}

static unsigned int lng_syntax_imports_count(struct lng_syntax_t* self) {
	unsigned int size = 0;
	struct lng_token_t* token = self->token;
	while (!lng_syntax_is(self, LNG_LEXICAL_CLASS_WORD)) {
		if (lng_syntax_is(self, LNG_LEXICAL_IMPORT_WORD)) {
			size++;
		}
		lng_syntax_next(self);
	}
	self->token = token;
	return size;
}

unsigned char lng_syntax_imports_test(struct lng_syntax_t* self) {
	unsigned int size = 0;
	while (lng_syntax_is(self, LNG_LEXICAL_IMPORT_WORD)) {
		lng_syntax_next(self);
		if (lng_syntax_ids_test(self)) {
			return 1;
		}
		size++;
	}
	self->memory_size += (size + 1) * sizeof(struct lng_node_import_t);
	return 0;
}

struct lng_node_import_t* lng_syntax_imports_build(struct lng_syntax_t* self, unsigned int* import_count) {
	struct lng_node_import_t* array = (struct lng_node_import_t*) self->memory;
	unsigned int n, size = lng_syntax_imports_count(self);
	self->memory += (size + 1) * sizeof(struct lng_node_import_t);
	for (n = 0; n < size; n++) {
		lng_syntax_next(self);
		struct lng_token_t* token = lng_syntax_ids_build(self);
		struct lng_node_import_t* node = array + n;
		node->vtable = &lng_node_import_vtable;
		node->name = token;
	}
	struct lng_node_import_t* node = array + size;
	node->vtable = 0;
	*import_count = size;
	return array;
}

unsigned char lng_syntax_class_test(struct lng_syntax_t* self) {
	if (!lng_syntax_is(self, LNG_LEXICAL_CLASS_WORD)) {
		return 1;
	}
	lng_syntax_next(self);
	self->memory_size += sizeof(struct lng_node_class_t);
	if (!lng_syntax_is(self, LNG_LEXICAL_ID)) {
		return 1;
	}
	lng_syntax_next(self);
	if (!lng_syntax_is(self, LNG_LEXICAL_DO_WORD)) {
		return 1;
	}
	lng_syntax_next(self);
	while (!lng_syntax_is(self, LNG_LEXICAL_END_WORD) && !lng_syntax_is(self, LNG_LEXICAL_EOF)) {
		uint8 is_static = 0;
		for (;;) {
			if (lng_syntax_is(self, LNG_LEXICAL_STATIC_WORD) && !is_static) {
				lng_syntax_next(self);
				is_static = 1;
			} else {
				break;
			}
		}
		if (lng_syntax_type_test(self)) {
			return 1;
		}
		if (!lng_syntax_is(self, LNG_LEXICAL_ID)) {
			return 1;
		}
		lng_syntax_next(self);
		if (lng_syntax_is(self, '(')) {
			if (lng_syntax_parameters_test(self)) {
				return 1;
			}
			if (lng_syntax_block_test(self)) {
				return 1;
			}
			self->memory_size += sizeof(struct lng_node_method_t);
		} else {
			self->memory_size += sizeof(struct lng_node_field_t);
		}
	}
	self->memory_size += sizeof(struct lng_node_field_t);
	self->memory_size += sizeof(struct lng_node_method_t);
	if (!lng_syntax_is(self, LNG_LEXICAL_END_WORD)) {
		return 1;
	}
	lng_syntax_next(self);
	return 0;
}

static void lng_syntax_class_count(struct lng_syntax_t* self, uint16_t* fields, uint16_t* methods) {
	struct lng_token_t* token = self->token;
	*fields = 0;
	*methods = 0;
	while (!lng_syntax_is(self, LNG_LEXICAL_END_WORD)) {
		for (;;) {
			if (lng_syntax_is(self, LNG_LEXICAL_STATIC_WORD)) {
				lng_syntax_next(self);
			} else {
				break;
			}
		}
		lng_syntax_next(self);
		lng_syntax_next(self);
		if (lng_syntax_is(self, '(')) {
			lng_syntax_next(self);
			while (!lng_syntax_is(self, LNG_LEXICAL_DO_WORD)) {
				lng_syntax_next(self);
			}
			lng_syntax_next(self);
			int counter = 1;
			while (counter != 0) {
				if (lng_syntax_is(self, LNG_LEXICAL_END_WORD)) {
					counter--;
				} else if (lng_syntax_is(self, LNG_LEXICAL_DO_WORD)) {
					counter++;
				}
				lng_syntax_next(self);
			}
			(*methods)++;
		} else {
			(*fields)++;
		}
	}
	self->token = token;
}

struct lng_node_class_t* lng_syntax_class_build(struct lng_syntax_t* self) {
	lng_syntax_next(self);
	struct lng_node_class_t* node = (struct lng_node_class_t*) self->memory;
	self->memory += sizeof(struct lng_node_class_t);
	node->vtable = &lng_node_class_vtable;
	node->name = lng_syntax_next(self);
	lng_syntax_next(self);
	lng_syntax_class_count(self, &node->field_count, &node->method_count);
	node->fields = (struct lng_node_field_t*) self->memory;
	self->memory += (node->field_count + 1) * sizeof(struct lng_node_field_t);
	node->methods = (struct lng_node_method_t*) self->memory;
	self->memory += (node->method_count + 1) * sizeof(struct lng_node_method_t);
	unsigned int field_index = 0, method_index = 0;
	unsigned int n, size = node->field_count + node->method_count;
	for (n = 0; n < size; n++) {
		uint8 is_static = 0;
		for (;;) {
			if (lng_syntax_is(self, LNG_LEXICAL_STATIC_WORD) && !is_static) {
				lng_syntax_next(self);
				is_static = 1;
			} else {
				break;
			}
		}
		struct lng_node_type_t* type = lng_syntax_type_build(self);
		struct lng_token_t* name = lng_syntax_next(self);
		if (lng_syntax_is(self, '(')) {
			struct lng_node_param_t* params = lng_syntax_parameters_build(self);
			struct lng_node_block_t* cmd = lng_syntax_block_build(self);
			struct lng_node_method_t* method = node->methods + method_index++;
			method->vtable = &lng_node_method_vtable;
			method->clazz = node;
			method->is_static = 1;
			method->name = name;
			method->type = type;
			method->type->parent = (struct lng_node_t*) method;
			method->parameters = params;
			method->parameters->parent = (struct lng_node_t*) method;
			method->cmd = cmd;
			method->cmd->parent = (struct lng_node_t*) method;
			method->parent = (struct lng_node_t*) node;
		} else {
			struct lng_node_field_t* field = node->fields + field_index++;
			field->vtable = &lng_node_field_vtable;
			field->name = name;
			field->parent = (struct lng_node_t*) node;
		}
	}
	return node;
}

unsigned char lng_syntax_type_is(struct lng_syntax_t* self) {
	if (lng_syntax_is(self, LNG_LEXICAL_INT_WORD) || lng_syntax_is(self, LNG_LEXICAL_BOOLEAN_WORD) || lng_syntax_is(self, LNG_LEXICAL_STRING_WORD)) {
		return 1;
	} else {
		return 0;
	}
}

unsigned char lng_syntax_type_test(struct lng_syntax_t* self) {
	if (lng_syntax_is(self, LNG_LEXICAL_INT_WORD)) {
		lng_syntax_next(self);
		self->memory_size += sizeof(struct lng_node_type_int_t);
		return 0;
	} else if (lng_syntax_is(self, LNG_LEXICAL_BOOLEAN_WORD)) {
		lng_syntax_next(self);
		self->memory_size += sizeof(struct lng_node_type_bool_t);
		return 0;
	} else if (lng_syntax_is(self, LNG_LEXICAL_STRING_WORD)) {
		lng_syntax_next(self);
		self->memory_size += sizeof(struct lng_node_type_str_t);
		return 0;
	} else {
		return 1;
	}
}

struct lng_node_type_t* lng_syntax_type_build(struct lng_syntax_t* self) {
	if (lng_syntax_is(self, LNG_LEXICAL_INT_WORD)) {
		struct lng_token_t* token = lng_syntax_next(self);
		struct lng_node_type_int_t* node = (struct lng_node_type_int_t*) self->memory;
		self->memory += sizeof(struct lng_node_type_int_t);
		node->vtable = &lng_node_type_int_vtable;
		node->token = token;
		return (struct lng_node_type_t*) node;
	} else if (lng_syntax_is(self, LNG_LEXICAL_BOOLEAN_WORD)) {
		struct lng_token_t* token = lng_syntax_next(self);
		struct lng_node_type_bool_t* node = (struct lng_node_type_bool_t*) self->memory;
		self->memory += sizeof(struct lng_node_type_bool_t);
		node->vtable = &lng_node_type_bool_vtable;
		node->token = token;
		return (struct lng_node_type_t*) node;
	} else if (lng_syntax_is(self, LNG_LEXICAL_STRING_WORD)) {
		struct lng_token_t* token = lng_syntax_next(self);
		struct lng_node_type_str_t* node = (struct lng_node_type_str_t*) self->memory;
		self->memory += sizeof(struct lng_node_type_str_t);
		node->vtable = &lng_node_type_str_vtable;
		node->token = token;
		return (struct lng_node_type_t*) node;
	} else {
		assert(0);
		return 0;
	}
}

unsigned int lng_syntax_parameters_count(struct lng_syntax_t* self) {
	struct lng_token_t* token = self->token;
	lng_syntax_next(self);
	if (lng_syntax_is(self, ')')) {
		self->token = token;
		return 0;
	}
	unsigned int size = 1;
	while (!lng_syntax_is(self, ')') && !lng_syntax_is(self, LNG_LEXICAL_EOF)) {
		if (lng_syntax_is(self, ',')) {
			size++;
		}
		lng_syntax_next(self);
	}
	self->token = token;
	return size;
}

unsigned char lng_syntax_parameters_test(struct lng_syntax_t* self) {
	if (!lng_syntax_is(self, '(')) {
		return 1;
	}
	lng_syntax_next(self);
	while (!lng_syntax_is(self, ')') && !lng_syntax_is(self, LNG_LEXICAL_EOF)) {
		if (lng_syntax_type_test(self)) {
			return 1;
		}
		if (lng_syntax_id_test(self)) {
			return 1;
		}
		self->memory_size += sizeof(struct lng_node_param_t);
		if (!lng_syntax_is(self, ',')) {
			break;
		}
		lng_syntax_next(self);
	}
	if (!lng_syntax_is(self, ')')) {
		return 1;
	}
	lng_syntax_next(self);
	self->memory_size += sizeof(struct lng_node_param_t);
	return 0;
}

struct lng_node_param_t* lng_syntax_parameters_build(struct lng_syntax_t* self) {
	unsigned int n, size = lng_syntax_parameters_count(self);
	lng_syntax_next(self);
	struct lng_node_param_t* array = (struct lng_node_param_t*) self->memory;
	self->memory += (size + 1) * sizeof(struct lng_node_param_t);
	for (n = 0; n < size; n++) {
		struct lng_node_type_t* type = lng_syntax_type_build(self);
		struct lng_token_t* name = lng_syntax_next(self);
		struct lng_node_param_t* node = array + n;
		node->vtable = &lng_node_param_vtable;
		node->type = type;
		node->type->parent = (struct lng_node_t*) node;
		node->name = name;
		if (n != size - 1) {
			lng_syntax_next(self);
		}
	}
	lng_syntax_next(self);
	return array;
}

unsigned char lng_syntax_command_test(struct lng_syntax_t* self) {
	if (lng_syntax_is(self, ';')) {
		lng_syntax_next(self);
		return 0;
	} else if (lng_syntax_is(self, LNG_LEXICAL_DO_WORD)) {
		return lng_syntax_block_test(self);
	} else if (lng_syntax_is(self, LNG_LEXICAL_IF_WORD)) {
		return lng_syntax_if_test(self);
	} else if (lng_syntax_is(self, LNG_LEXICAL_WHILE_WORD)) {
		return lng_syntax_while_test(self);
	} else if (lng_syntax_is(self, LNG_LEXICAL_REPEAT_WORD)) {
		return lng_syntax_repeat_test(self);
	} else if (lng_syntax_is(self, LNG_LEXICAL_RETURN_WORD)) {
		return lng_syntax_return_test(self);
	} else if (lng_syntax_is(self, LNG_LEXICAL_FOR_WORD)) {
		return lng_syntax_for_test(self);
	} else if (lng_syntax_declare_is(self)) {
		return lng_syntax_declare_test(self);
	} else {
		return lng_syntax_expression_test(self);
	}
}

struct lng_node_command_t* lng_syntax_command_build(struct lng_syntax_t* self) {
	if (lng_syntax_is(self, ';')) {
		lng_syntax_next(self);
		return 0;
	} else if (lng_syntax_is(self, LNG_LEXICAL_DO_WORD)) {
		return (struct lng_node_command_t*) lng_syntax_block_build(self);
	} else if (lng_syntax_is(self, LNG_LEXICAL_IF_WORD)) {
		return (struct lng_node_command_t*) lng_syntax_if_build(self);
	} else if (lng_syntax_is(self, LNG_LEXICAL_WHILE_WORD)) {
		return (struct lng_node_command_t*) lng_syntax_while_build(self);
	} else if (lng_syntax_is(self, LNG_LEXICAL_REPEAT_WORD)) {
		return (struct lng_node_command_t*) lng_syntax_repeat_build(self);
	} else if (lng_syntax_is(self, LNG_LEXICAL_RETURN_WORD)) {
		return (struct lng_node_command_t*) lng_syntax_return_build(self);
	} else if (lng_syntax_is(self, LNG_LEXICAL_FOR_WORD)) {
		return (struct lng_node_command_t*) lng_syntax_for_build(self);
	} else if (lng_syntax_declare_is(self)) {
		return (struct lng_node_command_t*) lng_syntax_declare_build(self);
	} else {
		return (struct lng_node_command_t*) lng_syntax_expression_build(self);
	}
}

unsigned char lng_syntax_block_test(struct lng_syntax_t* self) {
	if (!lng_syntax_is(self, LNG_LEXICAL_DO_WORD)) {
		return 1;
	}
	lng_syntax_next(self);
	if (lng_syntax_is(self, LNG_LEXICAL_END_WORD)) {
		lng_syntax_next(self);
		return 0;
	}
	self->memory_size += sizeof(struct lng_node_block_t);
	do {
		if (lng_syntax_command_test(self)) {
			return 1;
		}
		self->memory_size += sizeof(struct lng_node_command_t*);
	} while (!lng_syntax_is(self, LNG_LEXICAL_END_WORD) && !lng_syntax_is(self, LNG_LEXICAL_EOF));
	if (!lng_syntax_is(self, LNG_LEXICAL_END_WORD)) {
		return 1;
	}
	lng_syntax_next(self);
	self->memory_size += sizeof(struct lng_node_command_t*);
	return 0;
}

size_t lng_syntax_block_count(struct lng_syntax_t* self) {
	struct lng_token_t* token = self->token;
	size_t memory_size = self->memory_size;
	size_t size = 0;
	while (!lng_syntax_is(self, LNG_LEXICAL_END_WORD)) {
		lng_syntax_command_test(self);
		size++;
	}
	self->memory_size = memory_size;
	self->token = token;
	return size;
}

static void lng_syntax_block_register_declare(struct lng_syntax_t* self, struct lng_node_block_t* block_node, struct lng_node_command_t* cmd_node) {
	unsigned char classid = cmd_node->vtable->classid;
	switch (classid) {
	case LNG_NODE_DECLARE_TYPE: {
		struct lng_node_linked_t* linked = (struct lng_node_linked_t*) self->memory;
		self->memory += sizeof(struct lng_node_linked_t);
		linked->back = 0;
		linked->next = block_node->declares;
		linked->node = (struct lng_node_t*) cmd_node;
		if (block_node->declares) {
			block_node->declares->back = linked;
		}
		block_node->declares = linked;
		break;
	}
	case LNG_NODE_FORN_TYPE: {
		struct lng_node_forn_t* forn_node = (struct lng_node_forn_t*) cmd_node;
		if (forn_node->init) {
			lng_syntax_block_register_declare(self, block_node, forn_node->init);
		}
		if (forn_node->inc) {
			lng_syntax_block_register_declare(self, block_node, forn_node->inc);
		}
		break;
	}
	}
}

struct lng_node_block_t* lng_syntax_block_build(struct lng_syntax_t* self) {
	lng_syntax_next(self);
	if (lng_syntax_is(self, LNG_LEXICAL_END_WORD)) {
		lng_syntax_next(self);
		static struct lng_node_block_t node = { &lng_node_block_vtable, 0 };
		return &node;
	}
	struct lng_node_block_t* node = (struct lng_node_block_t*) self->memory;
	self->memory += sizeof(struct lng_node_block_t);
	node->vtable = &lng_node_block_vtable;
	node->declares = 0;
	size_t n, size = lng_syntax_block_count(self);
	struct lng_node_command_t** array = (struct lng_node_command_t**) self->memory;
	self->memory += (size + 1) * sizeof(struct lng_node_command_t*);
	for (n = 0; n < size; n++) {
		struct lng_node_command_t* cmd = lng_syntax_command_build(self);
		if (cmd) {
			array[n] = cmd;
			cmd->parent = (struct lng_node_t*) node;
			lng_syntax_block_register_declare(self, node, cmd);
		}
	}
	lng_syntax_next(self);
	node->cmds = array;
	return node;
}

unsigned char lng_syntax_return_test(struct lng_syntax_t* self) {
	self->memory_size += sizeof(struct lng_node_return_t);
	if (!lng_syntax_is(self, LNG_LEXICAL_RETURN_WORD)) {
		return 1;
	}
	lng_syntax_next(self);
	if (lng_syntax_expr_test(self)) {
		return 1;
	}
	return 0;
}

struct lng_node_return_t* lng_syntax_return_build(struct lng_syntax_t* self) {
	struct lng_node_return_t* node = (struct lng_node_return_t*) self->memory;
	self->memory += sizeof(struct lng_node_return_t);
	node->vtable = &lng_node_return_vtable;
	lng_syntax_next(self);
	node->left = lng_syntax_expr_build(self);
	node->left->parent = (struct lng_node_t*) node;
	return node;
}

unsigned char lng_syntax_if_test(struct lng_syntax_t* self) {
	self->memory_size += sizeof(struct lng_node_if_t);
	if (!lng_syntax_is(self, LNG_LEXICAL_IF_WORD)) {
		return 1;
	}
	lng_syntax_next(self);
	if (lng_syntax_expr_test(self)) {
		return 1;
	}
	if (lng_syntax_command_test(self)) {
		return 1;
	}
	if (lng_syntax_is(self, LNG_LEXICAL_ELSE_WORD)) {
		lng_syntax_next(self);
		if (lng_syntax_command_test(self)) {
			return 1;
		}
	}
	return 0;
}

struct lng_node_if_t* lng_syntax_if_build(struct lng_syntax_t* self) {
	struct lng_node_if_t* node = (struct lng_node_if_t*) self->memory;
	self->memory += sizeof(struct lng_node_if_t);
	node->vtable = &lng_node_if_vtable;
	lng_syntax_next(self);
	node->cond = lng_syntax_expr_build(self);
	node->cond->parent = (struct lng_node_t*) node;
	node->true_cmd = lng_syntax_command_build(self);
	node->true_cmd->parent = (struct lng_node_t*) node;
	if (lng_syntax_is(self, LNG_LEXICAL_ELSE_WORD)) {
		lng_syntax_next(self);
		node->false_cmd = lng_syntax_command_build(self);
		node->false_cmd->parent = (struct lng_node_t*) node;
	}
	return node;
}

unsigned char lng_syntax_while_test(struct lng_syntax_t* self) {
	self->memory_size += sizeof(struct lng_node_while_t);
	if (!lng_syntax_is(self, LNG_LEXICAL_WHILE_WORD)) {
		return 1;
	}
	lng_syntax_next(self);
	if (lng_syntax_expr_test(self)) {
		return 1;
	}
	if (lng_syntax_command_test(self)) {
		return 1;
	}
	return 0;
}

struct lng_node_while_t* lng_syntax_while_build(struct lng_syntax_t* self) {
	struct lng_node_while_t* node = (struct lng_node_while_t*) self->memory;
	self->memory += sizeof(struct lng_node_while_t);
	node->vtable = &lng_node_while_vtable;
	lng_syntax_next(self);
	node->cond = lng_syntax_expr_build(self);
	node->cond->parent = (struct lng_node_t*) node;
	node->cmd = lng_syntax_command_build(self);
	node->cmd->parent = (struct lng_node_t*) node;
	return node;
}

unsigned char lng_syntax_repeat_test(struct lng_syntax_t* self) {
	self->memory_size += sizeof(struct lng_node_while_t);
	if (!lng_syntax_is(self, LNG_LEXICAL_REPEAT_WORD)) {
		return 1;
	}
	lng_syntax_next(self);
	if (lng_syntax_command_test(self)) {
		return 1;
	}
	if (!lng_syntax_is(self, LNG_LEXICAL_WHILE_WORD)) {
		return 1;
	}
	lng_syntax_next(self);
	if (lng_syntax_expr_test(self)) {
		return 1;
	}
	return 0;
}

struct lng_node_repeat_t* lng_syntax_repeat_build(struct lng_syntax_t* self) {
	struct lng_node_repeat_t* node = (struct lng_node_repeat_t*) self->memory;
	self->memory += sizeof(struct lng_node_repeat_t);
	node->vtable = &lng_node_repeat_vtable;
	lng_syntax_next(self);
	node->cmd = lng_syntax_command_build(self);
	node->cmd->parent = (struct lng_node_t*) node;
	lng_syntax_next(self);
	node->cond = lng_syntax_expr_build(self);
	node->cond->parent = (struct lng_node_t*) node;
	return node;
}

unsigned char lng_syntax_for_test(struct lng_syntax_t* self) {
	if (!lng_syntax_is(self, LNG_LEXICAL_FOR_WORD)) {
		return 1;
	}
	lng_syntax_next(self);
	if (lng_syntax_is(self, LNG_LEXICAL_ID)) {
		self->memory_size += sizeof(struct lng_node_foreach_t);
		lng_syntax_next(self);
		if (!lng_syntax_is(self, '=')) {
			return 1;
		}
		lng_syntax_next(self);
		if (lng_syntax_expr_test(self)) {
			return 1;
		}
		if (!lng_syntax_is(self, ',')) {
			return 1;
		}
		lng_syntax_next(self);
		if (lng_syntax_expr_test(self)) {
			return 1;
		}
		if (lng_syntax_command_test(self)) {
			return 1;
		}
		return 0;
	} else if (lng_syntax_is(self, '(')) {
		self->memory_size += sizeof(struct lng_node_forn_t);
		lng_syntax_next(self);
		if (!lng_syntax_is(self,';')) {
			if (lng_syntax_command_test(self)) {
				return 1;
			}
		}
		if (!lng_syntax_is(self,';')) {
			return 1;
		}
		lng_syntax_next(self);
		if (!lng_syntax_is(self,';')) {
			if (lng_syntax_expr_test(self)) {
				return 1;
			}
		}
		if (!lng_syntax_is(self,';')) {
			return 1;
		}
		lng_syntax_next(self);
		if (!lng_syntax_is(self,')')) {
			if (lng_syntax_command_test(self)) {
				return 1;
			}
		}
		if (!lng_syntax_is(self,')')) {
			return 1;
		}
		lng_syntax_next(self);
		if (lng_syntax_command_test(self)) {
			return 1;
		}
		return 0;
	} else {
		return 1;
	}
}

struct lng_node_command_t* lng_syntax_for_build(struct lng_syntax_t* self) {
	lng_syntax_next(self);
	if (lng_syntax_is(self, LNG_LEXICAL_ID)) {
		struct lng_node_foreach_t* node = (struct lng_node_foreach_t*) self->memory;
		self->memory += sizeof(struct lng_node_foreach_t);
		node->vtable = &lng_node_foreach_vtable;
		node->name = lng_syntax_next(self);
		lng_syntax_next(self);
		node->begin = lng_syntax_expr_build(self);
		node->begin->parent = (struct lng_node_t*) node;
		lng_syntax_next(self);
		node->end = lng_syntax_expr_build(self);
		node->end->parent = (struct lng_node_t*) node;
		node->cmd = lng_syntax_command_build(self);
		node->cmd->parent = (struct lng_node_t*) node;
		return (struct lng_node_command_t*) node;
	} else {
		struct lng_node_forn_t* node = (struct lng_node_forn_t*) self->memory;
		self->memory += sizeof(struct lng_node_forn_t);
		node->vtable = &lng_node_forn_vtable;
		lng_syntax_next(self);
		if (!lng_syntax_is(self,';')) {
			node->init = lng_syntax_command_build(self);
			node->init->parent = (struct lng_node_t*) node;
		}
		lng_syntax_next(self);
		if (!lng_syntax_is(self,';')) {
			node->cond = lng_syntax_expr_build(self);
			node->cond->parent = (struct lng_node_t*) node;
		}
		lng_syntax_next(self);
		if (!lng_syntax_is(self,')')) {
			node->inc = lng_syntax_command_build(self);
			node->inc->parent = (struct lng_node_t*) node;
		}
		lng_syntax_next(self);
		node->cmd = lng_syntax_command_build(self);
		node->cmd->parent = (struct lng_node_t*) node;
		return (struct lng_node_command_t*) node;
	}
}

unsigned char lng_syntax_declare_is(struct lng_syntax_t* self) {
	int index = lng_syntax_type_is(self);
	if (index <= 0) {
		return 0;
	}
	if (!lng_syntax_look(self, index, LNG_LEXICAL_ID)) {
		return 0;
	}
	return 1;
}

unsigned char lng_syntax_declare_test(struct lng_syntax_t* self) {
	self->memory_size += sizeof(struct lng_node_declare_t);
	if (lng_syntax_type_test(self)) {
		return 1;
	}
	if (!lng_syntax_can(self, LNG_LEXICAL_ID)) {
		return 1;
	}
	if (lng_syntax_is(self, '=')) {
		lng_syntax_next(self);
		if (lng_syntax_expr_test(self)) {
			return 1;
		}
	}
	self->memory_size += sizeof(struct lng_node_linked_t);
	return 0;
}

struct lng_node_declare_t* lng_syntax_declare_build(struct lng_syntax_t* self) {
	struct lng_node_declare_t* node = (struct lng_node_declare_t*) self->memory;
	self->memory += sizeof(struct lng_node_declare_t);
	node->vtable = &lng_node_declare_vtable;
	node->type = lng_syntax_type_build(self);
	node->type->parent = (struct lng_node_t*) node;
	node->name = lng_syntax_next(self);
	if (lng_syntax_is(self, '=')) {
		lng_syntax_next(self);
		node->value = lng_syntax_expr_build(self);
		node->value->parent = (struct lng_node_t*) node;
	}
	return node;
}

unsigned char lng_syntax_expression_test(struct lng_syntax_t* self) {
	self->memory_size += sizeof(struct lng_node_expression_t);
	if (lng_syntax_expr_test(self)) {
		return 1;
	}
	return 0;
}

struct lng_node_expression_t* lng_syntax_expression_build(struct lng_syntax_t* self) {
	struct lng_node_expression_t* node = (struct lng_node_expression_t*) self->memory;
	self->memory += sizeof(struct lng_node_expression_t);
	node->vtable = &lng_node_expression_vtable;
	node->left = lng_syntax_expr_build(self);
	node->left->parent = (struct lng_node_t*) node;
	return node;
}

unsigned char lng_syntax_expr_test(struct lng_syntax_t* self) {
	return lng_syntax_ternary_test(self);
}

struct lng_node_value_t* lng_syntax_expr_build(struct lng_syntax_t* self) {
	return lng_syntax_ternary_build(self);
}

unsigned char lng_syntax_ternary_test(struct lng_syntax_t* self) {
	if (lng_syntax_and_test(self)) {
		return 1;
	}
	while (lng_syntax_is(self, '?')) {
		lng_syntax_next(self);
		if (lng_syntax_and_test(self)) {
			return 1;
		}
		if (!lng_syntax_is(self, ':')) {
			return 1;
		}
		lng_syntax_next(self);
		if (lng_syntax_and_test(self)) {
			return 1;
		}
		self->memory_size += sizeof(struct lng_node_ternary_t);
	}
	return 0;
}

struct lng_node_value_t* lng_syntax_ternary_build(struct lng_syntax_t* self) {
	struct lng_node_value_t* left = lng_syntax_and_build(self);
	while (lng_syntax_is(self, '?')) {
		struct lng_node_ternary_t* node = (struct lng_node_ternary_t*) self->memory;
		self->memory += sizeof(struct lng_node_ternary_t);
		node->vtable = &lng_node_ternary_vtable;
		node->left = left;
		node->left->parent = (struct lng_node_t*) node;
		lng_syntax_next(self);
		node->right = lng_syntax_and_build(self);
		node->right->parent = (struct lng_node_t*) node;
		lng_syntax_next(self);
		node->center = lng_syntax_and_build(self);
		node->center->parent = (struct lng_node_t*) node;
		left = (struct lng_node_value_t*) node;
	}
	return left;
}

unsigned char lng_syntax_and_test(struct lng_syntax_t* self) {
	if (lng_syntax_or_test(self)) {
		return 1;
	}
	while (lng_syntax_is(self, LNG_LEXICAL_AND_WORD)) {
		lng_syntax_next(self);
		if (lng_syntax_or_test(self)) {
			return 1;
		}
		self->memory_size += sizeof(struct lng_node_and_t);
	}
	return 0;
}

struct lng_node_value_t* lng_syntax_and_build(struct lng_syntax_t* self) {
	struct lng_node_value_t* left = lng_syntax_or_build(self);
	while (lng_syntax_is(self, LNG_LEXICAL_AND_WORD)) {
		lng_syntax_next(self);
		struct lng_node_and_t* node = (struct lng_node_and_t*) self->memory;
		self->memory += sizeof(struct lng_node_and_t);
		node->vtable = &lng_node_and_vtable;
		node->left = left;
		node->right = lng_syntax_or_build(self);
		node->left->parent = (struct lng_node_t*) node;
		node->right->parent = (struct lng_node_t*) node;
		left = (struct lng_node_value_t*) node;
	}
	return left;
}

unsigned char lng_syntax_or_test(struct lng_syntax_t* self) {
	if (lng_syntax_compare_test(self)) {
		return 1;
	}
	while (lng_syntax_is(self, LNG_LEXICAL_OR_WORD)) {
		lng_syntax_next(self);
		if (lng_syntax_compare_test(self)) {
			return 1;
		}
		self->memory_size += sizeof(struct lng_node_or_t);
	}
	return 0;
}

struct lng_node_value_t* lng_syntax_or_build(struct lng_syntax_t* self) {
	struct lng_node_value_t* left = lng_syntax_compare_build(self);
	while (lng_syntax_is(self, LNG_LEXICAL_OR_WORD)) {
		lng_syntax_next(self);
		struct lng_node_or_t* node = (struct lng_node_or_t*) self->memory;
		self->memory += sizeof(struct lng_node_or_t);
		node->vtable = &lng_node_or_vtable;
		node->left = left;
		node->right = lng_syntax_compare_build(self);
		node->left->parent = (struct lng_node_t*) node;
		node->right->parent = (struct lng_node_t*) node;
		left = (struct lng_node_value_t*) node;
	}
	return left;
}

unsigned char lng_syntax_compare_test(struct lng_syntax_t* self) {
	if (lng_syntax_sum_test(self)) {
		return 1;
	}
	for (;;) {
		if (lng_syntax_is(self, LNG_LEXICAL_EQUAL_EQUAL)) {
			lng_syntax_next(self);
			if (lng_syntax_sum_test(self)) {
				return 1;
			}
			self->memory_size += sizeof(struct lng_node_eq_t);
		} else if (lng_syntax_is(self, LNG_LEXICAL_DIFERENT)) {
			lng_syntax_next(self);
			if (lng_syntax_sum_test(self)) {
				return 1;
			}
			self->memory_size += sizeof(struct lng_node_ne_t);
		} else if (lng_syntax_is(self, '<')) {
			lng_syntax_next(self);
			if (lng_syntax_sum_test(self)) {
				return 1;
			}
			self->memory_size += sizeof(struct lng_node_lt_t);
		} else if (lng_syntax_is(self, LNG_LEXICAL_LOWER_EQUAL)) {
			lng_syntax_next(self);
			if (lng_syntax_sum_test(self)) {
				return 1;
			}
			self->memory_size += sizeof(struct lng_node_le_t);
		} else if (lng_syntax_is(self, '>')) {
			lng_syntax_next(self);
			if (lng_syntax_sum_test(self)) {
				return 1;
			}
			self->memory_size += sizeof(struct lng_node_gt_t);
		} else if (lng_syntax_is(self, LNG_LEXICAL_GREATER_EQUAL)) {
			lng_syntax_next(self);
			if (lng_syntax_sum_test(self)) {
				return 1;
			}
			self->memory_size += sizeof(struct lng_node_ge_t);
		} else {
			break;
		}
	}
	return 0;
}

struct lng_node_value_t* lng_syntax_compare_build(struct lng_syntax_t* self) {
	struct lng_node_value_t* left = lng_syntax_sum_build(self);
	for (;;) {
		if (lng_syntax_is(self, LNG_LEXICAL_EQUAL_EQUAL)) {
			lng_syntax_next(self);
			struct lng_node_eq_t* node = (struct lng_node_eq_t*) self->memory;
			self->memory += sizeof(struct lng_node_eq_t);
			node->vtable = &lng_node_eq_vtable;
			node->left = left;
			node->right = lng_syntax_sum_build(self);
			node->left->parent = (struct lng_node_t*) node;
			node->right->parent = (struct lng_node_t*) node;
			left = (struct lng_node_value_t*) node;
		} else if (lng_syntax_is(self, LNG_LEXICAL_DIFERENT)) {
			lng_syntax_next(self);
			struct lng_node_ne_t* node = (struct lng_node_ne_t*) self->memory;
			self->memory += sizeof(struct lng_node_ne_t);
			node->vtable = &lng_node_ne_vtable;
			node->left = left;
			node->right = lng_syntax_sum_build(self);
			node->left->parent = (struct lng_node_t*) node;
			node->right->parent = (struct lng_node_t*) node;
			left = (struct lng_node_value_t*) node;
		} else if (lng_syntax_is(self, '<')) {
			lng_syntax_next(self);
			struct lng_node_lt_t* node = (struct lng_node_lt_t*) self->memory;
			self->memory += sizeof(struct lng_node_lt_t);
			node->vtable = &lng_node_lt_vtable;
			node->left = left;
			node->right = lng_syntax_sum_build(self);
			node->left->parent = (struct lng_node_t*) node;
			node->right->parent = (struct lng_node_t*) node;
			left = (struct lng_node_value_t*) node;
		} else if (lng_syntax_is(self, LNG_LEXICAL_LOWER_EQUAL)) {
			lng_syntax_next(self);
			struct lng_node_le_t* node = (struct lng_node_le_t*) self->memory;
			self->memory += sizeof(struct lng_node_le_t);
			node->vtable = &lng_node_le_vtable;
			node->left = left;
			node->right = lng_syntax_sum_build(self);
			node->left->parent = (struct lng_node_t*) node;
			node->right->parent = (struct lng_node_t*) node;
			left = (struct lng_node_value_t*) node;
		} else if (lng_syntax_is(self, '>')) {
			lng_syntax_next(self);
			struct lng_node_gt_t* node = (struct lng_node_gt_t*) self->memory;
			self->memory += sizeof(struct lng_node_gt_t);
			node->vtable = &lng_node_gt_vtable;
			node->left = left;
			node->right = lng_syntax_sum_build(self);
			node->left->parent = (struct lng_node_t*) node;
			node->right->parent = (struct lng_node_t*) node;
			left = (struct lng_node_value_t*) node;
		} else if (lng_syntax_is(self, LNG_LEXICAL_GREATER_EQUAL)) {
			lng_syntax_next(self);
			struct lng_node_ge_t* node = (struct lng_node_ge_t*) self->memory;
			self->memory += sizeof(struct lng_node_ge_t);
			node->vtable = &lng_node_ge_vtable;
			node->left = left;
			node->right = lng_syntax_sum_build(self);
			node->left->parent = (struct lng_node_t*) node;
			node->right->parent = (struct lng_node_t*) node;
			left = (struct lng_node_value_t*) node;
		} else {
			break;
		}
	}
	return left;
}

unsigned char lng_syntax_sum_test(struct lng_syntax_t* self) {
	if (lng_syntax_mul_test(self)) {
		return 1;
	}
	for (;;) {
		if (lng_syntax_is(self, '+')) {
			lng_syntax_next(self);
			if (lng_syntax_mul_test(self)) {
				return 1;
			}
			self->memory_size += sizeof(struct lng_node_sum_t);
		} else if (lng_syntax_is(self, '-')) {
			lng_syntax_next(self);
			if (lng_syntax_mul_test(self)) {
				return 1;
			}
			self->memory_size += sizeof(struct lng_node_sub_t);
		} else {
			break;
		}
	}
	return 0;
}

struct lng_node_value_t* lng_syntax_sum_build(struct lng_syntax_t* self) {
	struct lng_node_value_t* left = lng_syntax_mul_build(self);
	for (;;) {
		if (lng_syntax_is(self, '+')) {
			lng_syntax_next(self);
			struct lng_node_sum_t* node = (struct lng_node_sum_t*) self->memory;
			self->memory += sizeof(struct lng_node_sum_t);
			node->vtable = &lng_node_sum_vtable;
			node->left = left;
			node->right = lng_syntax_mul_build(self);
			node->left->parent = (struct lng_node_t*) node;
			node->right->parent = (struct lng_node_t*) node;
			left = (struct lng_node_value_t*) node;
		} else if (lng_syntax_is(self, '-')) {
			lng_syntax_next(self);
			struct lng_node_sub_t* node = (struct lng_node_sub_t*) self->memory;
			self->memory += sizeof(struct lng_node_sub_t);
			node->vtable = &lng_node_sub_vtable;
			node->left = left;
			node->right = lng_syntax_mul_build(self);
			node->left->parent = (struct lng_node_t*) node;
			node->right->parent = (struct lng_node_t*) node;
			left = (struct lng_node_value_t*) node;
		} else {
			break;
		}
	}
	return left;
}

unsigned char lng_syntax_mul_test(struct lng_syntax_t* self) {
	if (lng_syntax_unary_test(self)) {
		return 1;
	}
	for (;;) {
		if (lng_syntax_is(self, '*')) {
			lng_syntax_next(self);
			if (lng_syntax_unary_test(self)) {
				return 1;
			}
			self->memory_size += sizeof(struct lng_node_mul_t);
		} else if (lng_syntax_is(self, '/')) {
			lng_syntax_next(self);
			if (lng_syntax_unary_test(self)) {
				return 1;
			}
			self->memory_size += sizeof(struct lng_node_div_t);
		} else {
			break;
		}
	}
	return 0;
}

struct lng_node_value_t* lng_syntax_mul_build(struct lng_syntax_t* self) {
	struct lng_node_value_t* left = lng_syntax_unary_build(self);
	for (;;) {
		if (lng_syntax_is(self, '*')) {
			lng_syntax_next(self);
			struct lng_node_mul_t* node = (struct lng_node_mul_t*) self->memory;
			self->memory += sizeof(struct lng_node_mul_t);
			node->vtable = &lng_node_mul_vtable;
			node->left = left;
			node->right = lng_syntax_unary_build(self);
			node->left->parent = (struct lng_node_t*) node;
			node->right->parent = (struct lng_node_t*) node;
			left = (struct lng_node_value_t*) node;
		} else if (lng_syntax_is(self, '/')) {
			lng_syntax_next(self);
			struct lng_node_div_t* node = (struct lng_node_div_t*) self->memory;
			self->memory += sizeof(struct lng_node_div_t);
			node->vtable = &lng_node_div_vtable;
			node->left = left;
			node->right = lng_syntax_unary_build(self);
			node->left->parent = (struct lng_node_t*) node;
			node->right->parent = (struct lng_node_t*) node;
			left = (struct lng_node_value_t*) node;
		} else {
			break;
		}
	}
	return left;
}

unsigned char lng_syntax_unary_test(struct lng_syntax_t* self) {
	if (lng_syntax_is(self, '!')) {
		lng_syntax_next(self);
		if (lng_syntax_unary_test(self)) {
			return 1;
		}
		self->memory_size += sizeof(struct lng_node_not_t);
		return 0;
	} else if (lng_syntax_is(self, '-')) {
		lng_syntax_next(self);
		if (lng_syntax_unary_test(self)) {
			return 1;
		}
		self->memory_size += sizeof(struct lng_node_neg_t);
		return 0;
	} else if (lng_syntax_is(self, LNG_LEXICAL_SUM_SUM)) {
		lng_syntax_next(self);
		if (lng_syntax_literal_test(self)) {
			return 1;
		}
		self->memory_size += sizeof(struct lng_node_preinc_t);
		return 0;
	} else if (lng_syntax_is(self, LNG_LEXICAL_SUB_SUB)) {
		lng_syntax_next(self);
		if (lng_syntax_literal_test(self)) {
			return 1;
		}
		self->memory_size += sizeof(struct lng_node_predec_t);
		return 0;
	} else {
		return lng_syntax_literal_test(self);
	}
}

struct lng_node_value_t* lng_syntax_unary_build(struct lng_syntax_t* self) {
	if (lng_syntax_is(self, '!')) {
		lng_syntax_next(self);
		struct lng_node_not_t* node = (struct lng_node_not_t*) self->memory;
		self->memory += sizeof(struct lng_node_not_t);
		node->vtable = &lng_node_not_vtable;
		node->left = lng_syntax_literal_build(self);
		node->left->parent = (struct lng_node_t*) node;
		return (struct lng_node_value_t*) node;
	} else if (lng_syntax_is(self, '-')) {
		lng_syntax_next(self);
		struct lng_node_neg_t* node = (struct lng_node_neg_t*) self->memory;
		self->memory += sizeof(struct lng_node_neg_t);
		node->vtable = &lng_node_neg_vtable;
		node->left = lng_syntax_literal_build(self);
		node->left->parent = (struct lng_node_t*) node;
		return (struct lng_node_value_t*) node;
	} else if (lng_syntax_is(self, LNG_LEXICAL_SUM_SUM)) {
		lng_syntax_next(self);
		struct lng_node_preinc_t* node = (struct lng_node_preinc_t*) self->memory;
		self->memory += sizeof(struct lng_node_preinc_t);
		node->vtable = &lng_node_preinc_vtable;
		node->left = lng_syntax_literal_build(self);
		node->left->parent = (struct lng_node_t*) node;
		return (struct lng_node_value_t*) node;
	} else if (lng_syntax_is(self, LNG_LEXICAL_SUB_SUB)) {
		lng_syntax_next(self);
		struct lng_node_predec_t* node = (struct lng_node_predec_t*) self->memory;
		self->memory += sizeof(struct lng_node_predec_t);
		node->vtable = &lng_node_predec_vtable;
		node->left = lng_syntax_literal_build(self);
		node->left->parent = (struct lng_node_t*) node;
		return (struct lng_node_value_t*) node;
	} else {
		return lng_syntax_literal_build(self);
	}
}

unsigned char lng_syntax_literal_test(struct lng_syntax_t* self) {
	if (lng_syntax_is(self, '(')) {
		lng_syntax_next(self);
		if (lng_syntax_expr_test(self)) {
			return 1;
		}
		if (!lng_syntax_is(self, ')')) {
			return 1;
		}
		lng_syntax_next(self);
		return 0;
	} else if (lng_syntax_is(self, LNG_LEXICAL_NUMBER)) {
		self->memory_size += sizeof(struct lng_node_int_t);
		lng_syntax_next(self);
		return 0;
	} else if (lng_syntax_is(self, LNG_LEXICAL_STRING)) {
		self->memory_size += sizeof(struct lng_node_str_t);
		lng_syntax_next(self);
		return 0;
	} else if (lng_syntax_is(self, LNG_LEXICAL_TRUE_WORD)) {
		self->memory_size += sizeof(struct lng_node_bool_t);
		lng_syntax_next(self);
		return 0;
	} else if (lng_syntax_is(self, LNG_LEXICAL_FALSE_WORD)) {
		self->memory_size += sizeof(struct lng_node_bool_t);
		lng_syntax_next(self);
		return 0;
	} else if (lng_syntax_is(self, LNG_LEXICAL_ID)) {
		self->memory_size += sizeof(struct lng_node_id_t);
		lng_syntax_next(self);
		while (lng_syntax_is(self, '[')) {
			lng_syntax_next(self);
			self->memory_size += sizeof(struct lng_node_array_t);
			if (lng_syntax_expr_test(self)) {
				return 1;
			}
			if (!lng_syntax_is(self, ']')) {
				return 1;
			}
			lng_syntax_next(self);
		}
		for (;;) {
			if (lng_syntax_is(self, LNG_LEXICAL_SUM_SUM)) {
				lng_syntax_next(self);
				self->memory_size += sizeof(struct lng_node_posinc_t);
			} else if (lng_syntax_is(self, LNG_LEXICAL_SUB_SUB)) {
				lng_syntax_next(self);
				self->memory_size += sizeof(struct lng_node_posdec_t);
			} else {
				break;
			}
		}
		while (lng_syntax_is(self, '=')) {
			lng_syntax_next(self);
			self->memory_size += sizeof(struct lng_node_assign_t);
			if (lng_syntax_expr_test(self)) {
				return 1;
			}
		}
		return 0;
	} else {
		return 1;
	}
}

struct lng_node_value_t* lng_syntax_literal_build(struct lng_syntax_t* self) {
	struct lng_token_t* name = self->token;
	if (lng_syntax_is(self, '(')) {
		lng_syntax_next(self);
		struct lng_node_value_t* value = lng_syntax_expr_build(self);
		lng_syntax_next(self);
		return value;
	} else if (lng_syntax_is(self, LNG_LEXICAL_NUMBER)) {
		struct lng_node_int_t* node = (struct lng_node_int_t*) self->memory;
		self->memory += sizeof(struct lng_node_int_t);
		node->vtable = &lng_node_int_vtable;
		node->value = lng_io_string_to_int((char*) self->text + name->begin, name->size);
		lng_syntax_next(self);
		return (struct lng_node_value_t*) node;
	} else if (lng_syntax_is(self, LNG_LEXICAL_STRING)) {
		struct lng_node_str_t* node = (struct lng_node_str_t*) self->memory;
		self->memory += sizeof(struct lng_node_str_t);
		node->vtable = &lng_node_str_vtable;
		node->value = 0;
		lng_syntax_next(self);
		return (struct lng_node_value_t*) node;
	} else if (lng_syntax_is(self, LNG_LEXICAL_TRUE_WORD)) {
		struct lng_node_bool_t* node = (struct lng_node_bool_t*) self->memory;
		self->memory += sizeof(struct lng_node_bool_t);
		node->vtable = &lng_node_bool_vtable;
		node->value = 1;
		lng_syntax_next(self);
		return (struct lng_node_value_t*) node;
	} else if (lng_syntax_is(self, LNG_LEXICAL_FALSE_WORD)) {
		struct lng_node_bool_t* node = (struct lng_node_bool_t*) self->memory;
		self->memory += sizeof(struct lng_node_bool_t);
		node->vtable = &lng_node_bool_vtable;
		node->value = 0;
		lng_syntax_next(self);
		return (struct lng_node_value_t*) node;
	} else if (lng_syntax_is(self, LNG_LEXICAL_ID)) {
		struct lng_node_id_t* id_node = (struct lng_node_id_t*) self->memory;
		struct lng_node_value_t* node = (struct lng_node_value_t*) id_node;
		self->memory += sizeof(struct lng_node_id_t);
		id_node->vtable = &lng_node_id_vtable;
		id_node->name = self->token;
		lng_syntax_next(self);
		while (lng_syntax_is(self, '[')) {
			lng_syntax_next(self);
			struct lng_node_array_t* assign = (struct lng_node_array_t*) self->memory;
			self->memory += sizeof(struct lng_node_array_t);
			assign->vtable = &lng_node_array_vtable;
			assign->left = (struct lng_node_value_t*) node;
			assign->left->parent = (struct lng_node_t*) assign;
			assign->value = lng_syntax_expr_build(self);
			assign->value->parent = (struct lng_node_t*) assign;
			node = (struct lng_node_value_t*) assign;
			lng_syntax_next(self);
		}
		for (;;) {
			if (lng_syntax_is(self, LNG_LEXICAL_SUM_SUM)) {
				lng_syntax_next(self);
				struct lng_node_posinc_t* assign = (struct lng_node_posinc_t*) self->memory;
				self->memory += sizeof(struct lng_node_posinc_t);
				assign->vtable = &lng_node_posinc_vtable;
				assign->left = (struct lng_node_value_t*) node;
				assign->left->parent = (struct lng_node_t*) assign;
				node = (struct lng_node_value_t*) assign;
			} else if (lng_syntax_is(self, LNG_LEXICAL_SUB_SUB)) {
				lng_syntax_next(self);
				struct lng_node_posdec_t* assign = (struct lng_node_posdec_t*) self->memory;
				self->memory += sizeof(struct lng_node_posdec_t);
				assign->vtable = &lng_node_posdec_vtable;
				assign->left = (struct lng_node_value_t*) node;
				assign->left->parent = (struct lng_node_t*) assign;
				node = (struct lng_node_value_t*) assign;
			} else {
				break;
			}
		}
		while (lng_syntax_is(self, '=')) {
			lng_syntax_next(self);
			struct lng_node_assign_t* assign = (struct lng_node_assign_t*) self->memory;
			self->memory += sizeof(struct lng_node_assign_t);
			assign->vtable = &lng_node_assign_vtable;
			assign->left = (struct lng_node_value_t*) node;
			assign->left->parent = (struct lng_node_t*) assign;
			assign->value = lng_syntax_expr_build(self);
			assign->value->parent = (struct lng_node_t*) assign;
			node = (struct lng_node_value_t*) assign;
		}
		return node;
	} else {
		assert(0);
		return 0;
	}
}
