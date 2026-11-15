#include "../../src/ast.h"
#include "../../src/memory.h"
#include "../../src/semantic_anal.h"
#include "../../src/IFJcode_generator.h"
#include "ast_print_module.h"

#include <stdio.h>

#define LINE_NUM 999

/*
    @main
    def main(): Unit =
        if 1 != 4 then
            var x = 12
            return x
        else
            ()
        return              */
int main()
{
    arena_init(); // DO NOT REMOVE, KEEP AT BEGINNING
    scratch_init();

    // 1
    token_value_t one_int; one_int.int_value = 1;
    ast_node_t *one_lit = expr_insert_lit_leaf(one_int, LINE_NUM, T_INT_LITERAL);

    // 4
    token_value_t four_int; one_int.int_value = 4;
    ast_node_t *four_lit = expr_insert_lit_leaf(four_int, LINE_NUM, T_INT_LITERAL);

    // 1 != 4
    ast_node_t *nequals = expr_insert_op(T_NEQ, LINE_NUM, one_lit, four_lit);

    // 12
    token_value_t twelve_int; one_int.int_value = 12;
    ast_node_t *twelve_lit = expr_insert_lit_leaf(twelve_int, LINE_NUM, T_INT_LITERAL);

    // var x = 12
    scratch_append('x');
    char *x_str = scratch_save_to_arena();
    ast_node_t *x_declaration = ast_insert_var_declaration(x_str, LINE_NUM, true, T_NO_TYPE, twelve_lit);

    // x
    ast_node_t *x_id = expr_insert_id_leaf(x_str, LINE_NUM);

    // return x
    ast_node_t *retx = ast_insert_return(x_id, LINE_NUM);

//  var x = 12
//  return x
    x_declaration->next = retx;
    ast_node_t *then_body = ast_insert_block(x_declaration, LINE_NUM, retx);

    // ()
    token_value_t unit_val; unit_val.int_value = 0;
    ast_node_t *unit_lit = expr_insert_lit_leaf(unit_val, LINE_NUM, T_UNIT_LITERAL);

//  ()
    ast_node_t *else_body = ast_insert_block(unit_lit, LINE_NUM, unit_lit);


/*  if 1 != 4 then
        var x = 12
        return x
    else
        ()                  */
    ast_node_t *if_stmt = ast_insert_if(nequals, LINE_NUM, then_body, else_body);

    // return
    ast_node_t *ret = ast_insert_return(NULL, LINE_NUM);

/*  if 1 != 4 then
        var x = 12
        return x
    else
        ()
    return                  */
    if_stmt->next = ret;
    ast_node_t *body = ast_insert_block(if_stmt, LINE_NUM, ret);


    scratch_append('m'); scratch_append('a'); scratch_append('i'); scratch_append('n');
    char *main_str = scratch_save_to_arena();
    ast_node_t *definition = ast_insert_fn_definition(main_str, LINE_NUM, T_KEY_UNIT, NULL, body, true);




    ast_print(definition, 0);
    printf("======================================\n\
            ======================================\n\
            ======================================\n");

    run_sem_anal(definition);

    ast_print(definition, 0);





    scratch_free();
    arena_free_all(); // DO NOT REMOVE, KEEP AT END
}