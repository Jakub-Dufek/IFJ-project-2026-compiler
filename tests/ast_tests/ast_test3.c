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
        return 10+55          */
int main()
{
    scratch_init();
    arena_init(); // DO NOT REMOVE, KEEP AT BEGINNING

    // 10
    token_value_t ten_int; ten_int.int_value = 10;
    ast_node_t *ten_lit = expr_insert_lit_leaf(ten_int, LINE_NUM, T_INT_LITERAL);

    // 55
    token_value_t fifty_five_int; fifty_five_int.int_value = 55;
    ast_node_t *fifty_five_lit = expr_insert_lit_leaf(fifty_five_int, LINE_NUM, T_INT_LITERAL);

    // 10 + 55
    ast_node_t *plus = expr_insert_op(T_PLUS, LINE_NUM, ten_lit, fifty_five_lit);

    // return 10 + 55
    ast_node_t *ret = ast_insert_return(plus, LINE_NUM);

    // (INDENT) return 10 + 55
    ast_node_t *body = ast_insert_block(ret, LINE_NUM, ret);

    /*  
        @main
        def main(): Unit =
            return 10+55          */
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