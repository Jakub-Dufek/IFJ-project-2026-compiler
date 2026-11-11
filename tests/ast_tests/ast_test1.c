#include "../../src/ast.h"
#include "../../src/memory.h"
#include "../../src/semantic_anal.h"
#include "ast_print_module.h"

#define LINE_NUM 999

/*
    var x : Int = z + 5
*/
int main()
{
    scratch_init();
    arena_init(); // DO NOT REMOVE, KEEP AT BEGINNING

    // z
    scratch_append('z');
    char *z_str = scratch_save_to_arena();
    ast_node_t *z_id = expr_insert_id_leaf(z_str, LINE_NUM);

    // 5
    token_value_t five_int; five_int.int_value = 5;
    ast_node_t *five_lit = expr_insert_lit_leaf(five_int, LINE_NUM, T_INT_LITERAL);

    // z + 5
    ast_node_t *plus = expr_insert_op(T_PLUS, LINE_NUM, z_id, five_lit);


    // var x : Int = z + 5
    scratch_append('x');
    char *x_str = scratch_save_to_arena();
    ast_node_t *declaration = ast_insert_var_declaration(x_str, LINE_NUM, true, T_KEY_INT, plus);
    


    
    ast_print(declaration, 0);
    printf("======================================\n\
            ======================================\n\
            ======================================\n");

    run_sem_anal(declaration);

    ast_print(declaration, 0);



    scratch_free();
    arena_free_all(); // DO NOT REMOVE, KEEP AT END
}