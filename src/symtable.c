/**
* @file symtable.c
* @internal Part of IFJ-project-2026-compiler
*
* @brief symbol table structure implementation
* @author Ondřej Kycl (xkyclon00) 
* @date 04.10.2026
* 
* @details This file contains a symbol table implemented with height self-balancing binary tree data structure,
*stack structure storing individual symbol tables, also function parameter type list structure
*/

#include "symtable.h"


// SYMTABLE

symtable_bvs_node_t *symtable_initialize() {
    return NULL;
}

int bvs_max(int n1, int n2){ 
    if (n1 > n2) {
        return n1;
    }
    return n2;
}
int bvs_height(symtable_bvs_node_t *ptr){
    if (ptr == NULL) {
        return 0;
    }
    return ptr->height;
}

symtable_bvs_node_t *symtable_left_rotate(symtable_bvs_node_t *y){
    symtable_bvs_node_t *x = y->r_ptr;
    y->r_ptr = x->l_ptr;
    x->l_ptr = y;
    // switched nodes weight correction
    y->height = bvs_max(bvs_height(y->l_ptr), bvs_height(y->r_ptr)) + 1;
    x->height = bvs_max(bvs_height(x->l_ptr), bvs_height(x->r_ptr)) + 1;

    return x;
}
symtable_bvs_node_t *symtable_right_rotate(symtable_bvs_node_t *y){
    symtable_bvs_node_t *x = y->l_ptr;
    y->l_ptr = x->r_ptr;
    x->r_ptr = y;
    // switched nodes height correction
    y->height = bvs_max(bvs_height(y->l_ptr), bvs_height(y->r_ptr)) + 1;
    x->height = bvs_max(bvs_height(x->l_ptr), bvs_height(x->r_ptr)) + 1;

    return x;
}

symtable_bvs_node_t *symtable_find_symbol(symtable_bvs_node_t *root_ptr, char *k){
    if (root_ptr == NULL){// recursive condition
        return NULL; 
    }
    else {
        if (strcmp(root_ptr->key, k) != 0){
            if (strcmp(root_ptr->key, k) > 0){
                return symtable_find_symbol(root_ptr->l_ptr, k); //go left
            }
            else {
                return symtable_find_symbol(root_ptr->r_ptr, k); //go right
            }
        }
        else {
            return root_ptr;//symbol found
        }          
    } 
}

symtable_bvs_node_t *symtable_create_node(char *key, symbol_data_t symb){
    symtable_bvs_node_t *new_node = (symtable_bvs_node_t *) malloc(sizeof(symtable_bvs_node_t));
    if (new_node == NULL){
        error_exit(INTERNAL_ERR, 0, "Internal allocation fault of a symtable node"); 
    }
    //pass information to new node
    new_node->key = key;
    new_node->symbol_data = symb;
    new_node->l_ptr = NULL;
    new_node->r_ptr = NULL;
    new_node->height = 1;

    return new_node;
}

symtable_bvs_node_t *symtable_insert(symtable_bvs_node_t *root_ptr, char *key, symbol_data_t symb){
    if (root_ptr == NULL){
        return symtable_create_node(key, symb);
    }
    else {
        if (strcmp(root_ptr->key, key) > 0){ //go left
            root_ptr->l_ptr = symtable_insert(root_ptr->l_ptr, key, symb);
        }
        else{ //fo right
            if (strcmp(root_ptr->key, key) < 0){
                root_ptr->r_ptr = symtable_insert(root_ptr->r_ptr, key, symb);
            }
            else{// Symbol alredy there
                error_exit(SEM_REDEF_ERR, 0, "Redefinition of a symbol");
            }
        }

        root_ptr->height = bvs_max(bvs_height(root_ptr->l_ptr), bvs_height(root_ptr->r_ptr)) + 1;
        root_ptr->weight = bvs_height(root_ptr->r_ptr) - bvs_height(root_ptr->l_ptr);
        
        // Rotations to balance the tree if necessary after insertion

        if (root_ptr->weight == -2){
            if (strcmp(root_ptr->l_ptr->key, key) > 0){
                root_ptr = symtable_right_rotate(root_ptr);
            }
            else{
                root_ptr->l_ptr = symtable_left_rotate(root_ptr->l_ptr);
                root_ptr = symtable_right_rotate(root_ptr);
            }
        }
        else if (root_ptr->weight == 2){
            if (strcmp(root_ptr->r_ptr->key, key) < 0){
                root_ptr = symtable_left_rotate(root_ptr);
            }
            else{
                root_ptr->r_ptr = symtable_right_rotate(root_ptr->r_ptr);
                root_ptr = symtable_left_rotate(root_ptr);
            }
        }

        return root_ptr;
    }
}

void symtable_destructor(symtable_bvs_node_t *root_ptr){ //Postorder
    if(root_ptr == NULL){
        return;
    }
    if (root_ptr->l_ptr != NULL){
        symtable_destructor(root_ptr->l_ptr);
    }
    if (root_ptr->r_ptr != NULL){
        symtable_destructor(root_ptr->r_ptr);
    }

    if (root_ptr->symbol_data.id_type == ID_FUNC){
        param_type_list_destructor(&root_ptr->symbol_data.func_info.param_types);
    }
    free(root_ptr);
}


// STACK

void symtable_stack_initialize(symtable_stack_t *stack){
    stack->top = NULL;
}

void symtable_stack_push(symtable_stack_t *stack){
    symtable_instance_t *new_instance = malloc(sizeof(symtable_instance_t));
    if (new_instance == NULL){
        error_exit(INTERNAL_ERR, 0, "Internal allocation fault of a symtable instance");
    }
    new_instance->symtable = symtable_initialize();
    new_instance->next = stack->top;
    stack->top = new_instance;
}

void symtable_stack_pop(symtable_stack_t *stack){
    if (stack->top != NULL){
        symtable_instance_t *old_top = stack->top;
        stack->top = stack->top->next;

        symtable_destructor(old_top->symtable);
        free(old_top);
    }
}

bool symtable_stack_is_empty(symtable_stack_t *stack) {
    if (stack == NULL) {
        return true;
    }
    return (stack->top == NULL);
}

symtable_bvs_node_t *symtable_stack_search(symtable_stack_t *stack, char *key){
    symtable_instance_t *current = stack->top;
    symtable_bvs_node_t *found;
    while (current != NULL){
        found = symtable_find_symbol(current->symtable, key);
        if (found != NULL){
            return found;
        }
        current = current->next;
    }
    return NULL;
}


// LINKED LIST

void param_type_list_initialize(param_type_list_t *l) {
    if (l != NULL) {
        l->first = NULL;
        l->last = NULL;
    }
}

void param_type_list_insert(param_type_list_t *l, data_type type){
    if (l != NULL){
        param_type_t *new_param_type = (param_type_t*)malloc(sizeof(param_type_t)); //new node in list
        if (new_param_type == NULL){
            error_exit(INTERNAL_ERR, 0, "Internal allocation fault of a parameter type");
        }
        
        new_param_type->type = type;
        new_param_type->next_ptr = NULL;

        if (l->first == NULL){
            l->first = new_param_type;
            l->last = new_param_type;
        }
        else { // save nodes in unreversed order 
            l->last->next_ptr = new_param_type;
            l->last = new_param_type;
        }
    }
}

void param_type_list_destructor(param_type_list_t *l) {
    if (l == NULL) {
        return;
    }

    param_type_t *current = l->first;
    while (current != NULL) {
        param_type_t *next = current->next_ptr;
        free(current);
        current = next;
    }
}
