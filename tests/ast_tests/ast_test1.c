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
    
    // 1. Vytvoříme si tabulku symbolů speciálně pro tento test
    symtable_bvs_node_t *symtable = symtable_initialize();

    // 2. Ručně do ní vložíme proměnnou 'z' (např. typu Int), aby test prošel
    symbol_data_t z_sym;
    z_sym.data_type = T_KEY_INT;
    z_sym.id_type = ID_VAR; // nebo ID_VAL
    symtable = symtable_insert(symtable, "z", z_sym);

    // 3. Spustíme naši rekurzivní sémantickou analýzu a PŘEDÁME jí naši tabulku
    sem_anal(declaration, &symtable);

    // 4. Uklidíme
    symtable_destructor(symtable);

    (void)declaration; // FEEL FREE TO REMOVE, IS HERE FOR CFLAGS+ PURPOSES



    ast_free_arena(); // DO NOT REMOVE, KEEP AT END
}