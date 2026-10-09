/**
* @file symtable.c
* @internal Part of IFJ-project-2026-compiler
*
* @brief symbol table structure
* @author Ondřej Kycl (xkyclon00) 
* @date 04.10.2026
* 
* @details This file contains symbol table binary tree along with the user functions
*/

//TODO : Upravit popisky,
//       GetData funkce
//       stejné identifikátory
//       volání funkcí před lexikální definicí?
//       Spojit enumy s Kubou?
//       popsat funkce, prejmenovat ?
//       MAX_LENGTH pro Key/symbol?

// libraries
#include "symtable.h"
#include <stdlib.h>
#include <string.h> 

bvs_node_t *symtable_initialize() {
    return NULL;
}

bvs_node_t *find_symbol(bvs_node_t *root_ptr, char *k){
    if (root_ptr == NULL){
        return NULL;
    }
    else {
        if (strcmp(root_ptr->key, k) != 0){
            if (strcmp(root_ptr->key, k) > 0){
                return find_symbol(root_ptr->l_ptr, k);
            }
            else {
                return find_symbol(root_ptr->r_ptr, k);
            }
        }
        else {
            return root_ptr;
        }          
    } 
}

bvs_node_t *create_node(char *key, symbol_data_t symb){
    bvs_node_t *new_node = (bvs_node_t *) malloc(sizeof(bvs_node_t));
    if (new_node == NULL){
        return NULL;
    }
    new_node->key = (char *)malloc((strlen(key) + 1) * sizeof(char));
    strcpy(new_node->key, key);

    new_node->symbol_data = symb;
    new_node->l_ptr = NULL;
    new_node->r_ptr = NULL;
    new_node->weight = 0;

    return new_node;
}

bvs_node_t *insert(bvs_node_t *root_ptr, char *key, symbol_data_t symb){
    if (root_ptr == NULL){
        return create_node(key, symb);
    }
    else {
        if (strcmp(root_ptr->key, key) > 0){
            root_ptr->l_ptr = insert(root_ptr->l_ptr, key, symb);
        }
        else{
            if (strcmp(root_ptr->key, key) < 0){
                root_ptr->r_ptr = insert(root_ptr->r_ptr, key, symb);
            }
            else{
                root_ptr->symbol_data = symb;
                //return NULL ?;
                //error
            }
        }

        root_ptr->height = max(height(root_ptr->l_ptr), height(root_ptr->r_ptr)) + 1;
        root_ptr->weight = height(root_ptr->r_ptr) - height(root_ptr->l_ptr);
        
        if (root_ptr->weight == -2){
            if (strcmp(root_ptr->l_ptr->key, key) > 0){
                root_ptr = right_rotate(root_ptr);
            }
            else{
                root_ptr->l_ptr = left_rotate(root_ptr->l_ptr);
                root_ptr = right_rotate(root_ptr);
            }
        }
        else if (root_ptr->weight == 2){
            if (strcmp(root_ptr->r_ptr->key, key) < 0){
                root_ptr = left_rotate(root_ptr);
            }
            else{
                root_ptr->r_ptr = right_rotate(root_ptr->r_ptr);
                root_ptr = left_rotate(root_ptr);
            }
        }

        return root_ptr;
    }
}

int max(int n1, int n2){ 
    if (n1 > n2) {
        return n1;
    }
    return n2;
}
int height(bvs_node_t *ptr){
    if (ptr == NULL) {
        return 0;
    }
    return ptr->height;
}

bvs_node_t *left_rotate(bvs_node_t *y){
    bvs_node_t *x = y->r_ptr;
    y->r_ptr = x->l_ptr;
    x->l_ptr = y;
    // switched nodes weight correction
    y->height = max(height(y->l_ptr), height(y->r_ptr)) + 1;
    x->height = max(height(x->l_ptr), height(x->r_ptr)) + 1;

    return x;
}
bvs_node_t *right_rotate(bvs_node_t *y){
    bvs_node_t *x = y->l_ptr;
    y->l_ptr = x->r_ptr;
    x->r_ptr = y;
    // switched nodes height correction
    y->weight = max(height(y->l_ptr), height(y->r_ptr)) + 1;
    x->weight = max(height(x->l_ptr), height(x->r_ptr)) + 1;

    return x;
}


/*
void dlr_rot(bvs_node_t *crit_parent, bvs_node_t *crit){
    bvs_node_t *a = crit;
    bvs_node_t *b = a->l_ptr;
    bvs_node_t *c = b->r_ptr;

    b->r_ptr = c->l_ptr;
    a->l_ptr = c->r_ptr;
    c->l_ptr = b;
    c->r_ptr = a;

    // merge above crit
    if (crit_parent->l_ptr == crit){
        crit_parent->l_ptr = c;
    }
    else {
        crit_parent->r_ptr = c;
    }
    // weight correction for involved nodes
    if (c->weight == 1){
        b->weight = -1;
        a->weight = 0;
    }
    else if (c->weight == -1){
        b->weight = 0;
        a->weight = 1;
    }
    else {
        b->weight = 0;
        a->weight = 0;
    }
}*/

/*
bvs_node_t *bvs_min(bvs_node_t *root_ptr){
    if (root_ptr == NULL) return NULL;

    if (root_ptr->l_ptr == NULL){
        return root_ptr;
    }
    else {
        return bvs_min(root_ptr->l_ptr);
    }
}

bvs_node_t *bvs_delete(bvs_node_t *root_ptr, char *k){
    if (root_ptr == NULL){
        return NULL;
    }
    else {
        if (strcmp(root_ptr->key, k) > 0){
            root_ptr->l_ptr = bvs_delete(root_ptr->l_ptr,k);
            return root_ptr;
        }
        else {
            if (strcmp(root_ptr->key, k) < 0){
                root_ptr->r_ptr = bvs_delete(root_ptr->r_ptr,k);
                return root_ptr;
            }
            else {
                return bvs_delete_match(root_ptr);
            }
        }
    }
}

bvs_node_t *bvs_delete_match(bvs_node_t *root_ptr){
    bvs_node_t *only_child;
    
    if (root_ptr->l_ptr == NULL && root_ptr->r_ptr == NULL){
        free(root_ptr->key);
        free(root_ptr);
        return NULL;
    }
    else {
        if (root_ptr->l_ptr != NULL && root_ptr->r_ptr != NULL){
            bvs_node_t *min = bvs_min(root_ptr->r_ptr);
                        
            free(root_ptr->key);
            root_ptr->key = (char *)malloc((strlen(min->key) + 1) * sizeof(char));
            strcpy(root_ptr->key, min->key);

            root_ptr->symbol_data = min->symbol_data;
            root_ptr->r_ptr = bvs_delete(root_ptr->r_ptr, min->key);
            return root_ptr;
        }
        else {
            if (root_ptr->l_ptr == NULL){
                only_child = root_ptr->r_ptr;
            }
            else{
                only_child = root_ptr->l_ptr;
            }
            free(root_ptr->key);
            free(root_ptr);
                       
            return only_child;
        }
    }
}*/

void symtable_destructor(bvs_node_t *root_ptr){
    if(root_ptr == NULL){
        return;
    }
    if (root_ptr->l_ptr != NULL){
        bvs_destructor(root_ptr->l_ptr);
    }
    if (root_ptr->r_ptr != NULL){
        bvs_destructor(root_ptr->r_ptr);
    }
    free(root_ptr->key);
    free(root_ptr);
}

