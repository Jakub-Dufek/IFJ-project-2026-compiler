#include "../../src/ast.h"
#include "memory.h"
#include "semantic_anal.h"

#define LINE_NUM 999

/*
    var x : Int = z + 5
*/
int main()
{
    ast_init_arena(); // DO NOT REMOVE, KEEP AT BEGINNING

    // 10
    token_value_t ten_int; ten_int.int_value = 5;
    ast_node_t *ten_lit = expr_insert_lit_leaf(ten_int, LINE_NUM, T_INT_LITERAL);

    // 55
    token_value_t fifty_five_int; fifty_five_int.int_value = 5;
    ast_node_t *fifty_five_lit = expr_insert_lit_leaf(fifty_five_int, LINE_NUM, T_INT_LITERAL);

    // 10 + 55
    ast_node_t *plus = expr_insert_op(T_PLUS, LINE_NUM, ten_lit, fifty_five_lit);

    // var x : Int = 10 + 55
    char x_str[MAX_LENGTH] = "x";
    ast_node_t *declaration = ast_insert_var_declaration(x_str, LINE_NUM, true, T_KEY_INT, plus);

    // (INDENT) var x : Int = 10 + 55
    ast_node_t *block = ast_insert_block(declaration, LINE_NUM, declaration);

    // ()
    token_value_t null_lit = {0};
    ast_node_t *unit_lit = expr_insert_lit_leaf(null_lit, LINE_NUM, T_KEY_UNIT);

/*  @main
    def main(): Unit =
        var x : Int = 10 + 55   */
    char main_str[MAX_LENGTH] = "main";
    ast_node_t *definition = ast_insert_fn_definition(main_str, LINE_NUM, T_KEY_UNIT, block, unit_lit, true);





                    // INSERT CODE HERE
                // INSERT CODE HERE
            // INSERT CODE HERE
        // INSERT CODE HERE
    // INSERT CODE HERE
    // INSERT CODE HERE
        // INSERT CODE HERE
            // INSERT CODE HERE
                // INSERT CODE HERE
                    // INSERT CODE HERE





    (void)definition; // FEEL FREE TO REMOVE, IS HERE FOR CFLAGS+ PURPOSES



    ast_free_arena(); // DO NOT REMOVE, KEEP AT END
}