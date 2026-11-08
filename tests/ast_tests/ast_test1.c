#include "../../src/ast.h"
#include "../../src/memory.h"
#include "../../src/semantic_anal.h"

#define LINE_NUM 999

/*
    var x : Int = z + 5
*/
int main()
{
    arena_init(); // DO NOT REMOVE, KEEP AT BEGINNING

    // z
    char z_str[MAX_LENGTH] = "z";
    ast_node_t *z_id = expr_insert_id_leaf(z_str, LINE_NUM);

    // 5
    token_value_t five_int; five_int.int_value = 5;
    ast_node_t *five_lit = expr_insert_lit_leaf(five_int, LINE_NUM, T_INT_LITERAL);

    // z + 5
    ast_node_t *plus = expr_insert_op(T_PLUS, LINE_NUM, z_id, five_lit);


    // var x : Int = z + 5
    char x_str[MAX_LENGTH] = "x";
    ast_node_t *declaration = ast_insert_var_declaration(x_str, LINE_NUM, true, T_KEY_INT, plus);
    
    // 1. Lokální inicializace zásobníku
    symtable_stack_t stack;
    symtable_stack_initialize(&stack); 
    symtable_stack_push(&stack); // Globální scope

    // 2. Ruční vložení 'z', aby test nevyhodil Error 3
    symbol_data_t z_sym;
    z_sym.data_type = T_KEY_INT;
    z_sym.id_type = ID_VAR; 
    stack.top->symtable = symtable_insert(stack.top->symtable, "z", z_sym);

    // 3. Spuštění analýzy s předaným zásobníkem
    sem_anal(declaration, &stack);

    // 4. Úklid
    symtable_stack_pop(&stack);

    (void)declaration; // FEEL FREE TO REMOVE, IS HERE FOR CFLAGS+ PURPOSES



    arena_free_all(); // DO NOT REMOVE, KEEP AT END
}