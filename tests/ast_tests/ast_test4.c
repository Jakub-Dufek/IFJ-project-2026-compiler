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
        while 1 == 1.0 do
            return          */
int main()
{
    arena_init(); // DO NOT REMOVE, KEEP AT BEGINNING
    scratch_init();

    // 1
    token_value_t one_int; one_int.int_value = 1;
    ast_node_t *one_lit1 = expr_insert_lit_leaf(one_int, LINE_NUM, T_INT_LITERAL);

    // 1.0
    token_value_t one_double; one_double.double_value = 1.0;
    ast_node_t *one_lit2 = expr_insert_lit_leaf(one_double, LINE_NUM, T_DOUBLE_LITERAL);

    // 10 == 55
    ast_node_t *equals = expr_insert_op(T_EQ, LINE_NUM, one_lit1, one_lit2);

    // return
    ast_node_t *ret = ast_insert_return(NULL, LINE_NUM);

    // (INDENT) return
    ast_node_t *body = ast_insert_block(ret, LINE_NUM, ret);

/*  while 1 == 1.0 do
        return          */
    ast_node_t *cycle = ast_insert_while(equals, LINE_NUM, body);

    /*  
        @main
        def main(): Unit =
            while 1 == 1.0 do
                return          */
    scratch_append('m'); scratch_append('a'); scratch_append('i'); scratch_append('n');
    char *main_str = scratch_save_to_arena();
    ast_node_t *definition = ast_insert_fn_definition(main_str, LINE_NUM, T_KEY_UNIT, NULL, cycle, true);





    ast_print(definition, 0);
    printf("======================================\n\
            ======================================\n\
            ======================================\n");

    run_sem_anal(definition);

    ast_print(definition, 0);




    scratch_free();
    arena_free_all(); // DO NOT REMOVE, KEEP AT END
}