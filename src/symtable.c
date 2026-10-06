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
//       init?, 
//       Zvážit varianty Find,Ins (rekurzivní nebo ne), ¨
//       stejné identifikátory
//       volání funkcí před lexikální definicí?
//       destruktor?
//       Spojit enumy s Kubou?
//       popsat funkce?
//       MAX_LENGTH pro Key/symbol  !!!!!

// libraries
#include "symtable.h"
#include <stdlib.h>
#include <string.h> 

bvs_node_t * symtable_init() {
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
        return root_ptr;
    }
}

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
}
