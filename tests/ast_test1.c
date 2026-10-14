#include "../src/ast.h"

#define LINE_NUM 999

/*
    var x : Int = z + 5
*/
int main()
{
    ast_init_arena(); // DO NOT REMOVE, KEEP AT BEGINNING

    // var x : Int =
    ast_node_t *declaration = ast_node_init(AST_DECLARATION, LINE_NUM);
    declaration->data->declaration_node.data_type = T_KEY_INT;
    declaration->data->declaration_node.is_mutable = true;
    char x_str[MAX_LENGTH] = "x";
    strcpy(declaration->data->declaration_node.id_val, x_str);

    // z
    ast_node_t *z = ast_node_init(EXPR_IDENTIFIER, LINE_NUM);
    char z_str[MAX_LENGTH] = "z";
    strcpy(z->data->id_leaf_node.id_val, z_str);

    // 5
    ast_node_t *five = ast_node_init(EXPR_LITERAL, LINE_NUM);
    five->data->lit_leaf_node.lit_type = T_INT_LITERAL;
    five->data->lit_leaf_node.lit_value.int_value = 5;

    // z + 5
    ast_node_t *plus = ast_node_init(EXPR_ADD, LINE_NUM);
    plus->data->op_node.left = z;
    plus->data->op_node.right = five;

    // var x : Int = z + 5
    declaration->data->declaration_node.expression = plus;



    
    /*
                                YOUR CODE HERE
        YOUR CODE HERE
                                YOUR CODE HERE
        YOUR CODE HERE
                                YOUR CODE HERE
    */




    ast_free_arena(); // DO NOT REMOVE, KEEP AT END
}