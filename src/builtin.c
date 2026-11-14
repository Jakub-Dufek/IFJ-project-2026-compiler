/**
* @file bultin.c
* @internal Part of IFJ-project-2026-compiler, Team 274855
*
* @brief TODO
* @author Samuel Vit Lehotsky (xlehots00)
* @date 9. 11. 2026
*/

#include "builtin.h"

symtable_bvs_node_t* init_builtin(symtable_bvs_node_t *global_table) {
    symbol_data_t *symb_print;
    symb_print->data_type = T_UNIT_LITERAL;
    symb_print->id_type = ID_FUNC;
    symb_print->func_info.param_num = 1;
    symb_print->func_info.param_types->first->type = T_KEY_ANY;
    symb_print->func_info.param_types->first->next_ptr = NULL;
    symb_print->func_info.param_types->last = symb_print->func_info.param_types->first;
    symb_print->func_info.is_defined = true;
    symb_print->func_info.is_main = false;

    global_table = symtable_insert(global_table, "print", *symb_print);

    symbol_data_t *symb_read_line;
    symb_read_line->data_type = T_STRING_LITERAL;
    symb_read_line->id_type = ID_FUNC;
    symb_read_line->func_info.param_num = 0;
    symb_read_line->func_info.param_types->first->type = T_NO_TYPE;
    symb_read_line->func_info.param_types->first->next_ptr = NULL;
    symb_read_line->func_info.param_types->last = symb_read_line->func_info.param_types->first;
    symb_read_line->func_info.is_defined = true;
    symb_read_line->func_info.is_main = false;

    global_table = symtable_insert(global_table, "readLine", *symb_read_line);

    symbol_data_t *symb_read_int;
    symb_read_int->data_type = T_INT_LITERAL;
    symb_read_int->id_type = ID_FUNC;
    symb_read_int->func_info.param_num = 0;
    symb_read_int->func_info.param_types->first->type = T_NO_TYPE;
    symb_read_int->func_info.param_types->first->next_ptr = NULL;
    symb_read_int->func_info.param_types->last = symb_read_int->func_info.param_types->first;
    symb_read_int->func_info.is_defined = true;
    symb_read_int->func_info.is_main = false;

    global_table = symtable_insert(global_table, "readInt", *symb_read_int);

    symbol_data_t *symb_read_double;
    symb_read_double->data_type = T_DOUBLE_LITERAL;
    symb_read_double->id_type = ID_FUNC;
    symb_read_double->func_info.param_num = 0;
    symb_read_double->func_info.param_types->first->type = T_NO_TYPE;
    symb_read_double->func_info.param_types->first->next_ptr = NULL;
    symb_read_double->func_info.param_types->last = symb_read_double->func_info.param_types->first;
    symb_read_double->func_info.is_defined = true;
    symb_read_double->func_info.is_main = false;

    global_table = symtable_insert(global_table, "readDouble", *symb_read_double);

    symbol_data_t *symb_to_str;
    symb_to_str->data_type = T_STRING_LITERAL;
    symb_to_str->id_type = ID_FUNC;
    symb_to_str->func_info.param_num = 1;
    symb_to_str->func_info.param_types->first->type = T_KEY_ANY;
    symb_to_str->func_info.param_types->first->next_ptr = NULL;
    symb_to_str->func_info.param_types->last = symb_to_str->func_info.param_types->first;
    symb_to_str->func_info.is_defined = true;
    symb_to_str->func_info.is_main = false;

    global_table = symtable_insert(global_table, "toStr", *symb_to_str);

    symbol_data_t *symb_to_int;
    symb_to_int->data_type = T_INT_LITERAL;
    symb_to_int->id_type = ID_FUNC;
    symb_to_int->func_info.param_num = 1;
    symb_to_int->func_info.param_types->first->type = T_DOUBLE_LITERAL;
    symb_to_int->func_info.param_types->first->next_ptr = NULL;
    symb_to_int->func_info.param_types->last = symb_to_int->func_info.param_types->first;
    symb_to_int->func_info.is_defined = true;
    symb_to_int->func_info.is_main = false;

    global_table = symtable_insert(global_table, "toInt", *symb_to_int);

    symbol_data_t *symb_str_length;
    symb_str_length->data_type = T_INT_LITERAL;
    symb_str_length->id_type = ID_FUNC;
    symb_str_length->func_info.param_num = 1;
    symb_str_length->func_info.param_types->first->type = T_STRING_LITERAL;
    symb_str_length->func_info.param_types->first->next_ptr = NULL;
    symb_str_length->func_info.param_types->last = symb_str_length->func_info.param_types->first;
    symb_str_length->func_info.is_defined = true;
    symb_str_length->func_info.is_main = false;

    global_table = symtable_insert(global_table, "strLength", *symb_str_length);

    symbol_data_t *symb_sub_str;
    param_type_t *sub_str_start;
    sub_str_start->type = T_INT_LITERAL;
    sub_str_start->next_ptr = symb_sub_str->func_info.param_types->last;
    symb_sub_str->data_type = T_STRING_LITERAL;
    symb_sub_str->id_type = ID_FUNC;
    symb_sub_str->func_info.param_num = 3;
    symb_sub_str->func_info.param_types->first->type = T_STRING_LITERAL;
    symb_sub_str->func_info.param_types->first->next_ptr = sub_str_start;
    symb_sub_str->func_info.param_types->last->type = T_INT_LITERAL;
    symb_sub_str->func_info.param_types->last->next_ptr = NULL;
    symb_sub_str->func_info.is_defined = true;
    symb_sub_str->func_info.is_main = false; 

    global_table = symtable_insert(global_table, "subStr", *symb_sub_str);

    symbol_data_t *symb_ord;
    symb_ord->data_type = T_INT_LITERAL;
    symb_ord->id_type = ID_FUNC;
    symb_ord->func_info.param_num = 2;
    symb_ord->func_info.param_types->first->type = T_STRING_LITERAL;
    symb_ord->func_info.param_types->first->next_ptr = symb_ord->func_info.param_types->last;
    symb_ord->func_info.param_types->last->type = T_INT_LITERAL;
    symb_ord->func_info.param_types->last->next_ptr = NULL;
    symb_ord->func_info.is_defined = true;
    symb_ord->func_info.is_main = false;

    global_table = symtable_insert(global_table, "ord", *symb_ord);

    symbol_data_t *symb_chr;
    symb_chr->data_type = T_STRING_LITERAL;
    symb_chr->id_type = ID_FUNC;
    symb_chr->func_info.param_num = 1;
    symb_chr->func_info.param_types->first->type = T_INT_LITERAL;
    symb_chr->func_info.param_types->first->next_ptr = NULL;
    symb_chr->func_info.param_types->last = symb_chr->func_info.param_types->first;
    symb_chr->func_info.is_defined = true;
    symb_chr->func_info.is_main = false;

    global_table = symtable_insert(global_table, "chr", *symb_chr);

    return global_table;
}