#include "astar.h"
#include <stdlib.h>

struct list * list_create(){
    struct list * the_list = malloc(sizeof(struct list));
    the_list->root = NULL;
    return the_list;
}

struct node * node_create(){
    struct node * new_node = malloc(sizeof (struct node));
    new_node->parent = NULL; 
    new_node->x = 0;
    new_node->y = 0;
    new_node->f = 0;
    new_node->g = 0;
    new_node->h = 0;
    new_node->next_node = NULL;
    return new_node;
};

struct node * node_with_least_f(struct list * the_list){
    /* The adding process is optimized so that the root is always the one with least */
    return the_list->root;
}

void pop_a_node_off_list(struct list * the_list, struct node * node_to_pop){ 
    struct node * current_node = the_list->root;
    if (current_node->x == node_to_pop->x && current_node->y == node_to_pop->y){
        if (current_node->next_node != NULL){
            struct node * temp_node = current_node->next_node;
            current_node->next_node = NULL;
            the_list->root = temp_node;
        } else {
            the_list->root = NULL;
        }
        return;
    }
    while (current_node->next_node != NULL){
        if (current_node->next_node->x == node_to_pop->x && current_node->next_node->y == node_to_pop->y){
            if (current_node->next_node->next_node != NULL){
                struct node * temp_node = current_node->next_node->next_node;
                current_node->next_node->next_node = NULL;
                current_node->next_node = temp_node;
            } else {
                current_node->next_node = NULL;
            }
            return;
        } else {
            current_node = current_node -> next_node;
        }
    }
}

void add_a_node_to_list(struct list * the_list, struct node * node_to_add){
    if (the_list->root == NULL){
        the_list->root = node_to_add;
    } else {
        struct node * current_node = the_list->root;
        if (current_node->f > node_to_add->f){
            the_list->root = node_to_add;
            node_to_add->next_node = current_node;
            return;
        }
        while (current_node->next_node != NULL){
            if (node_to_add->f < current_node->next_node->f){
                node_to_add->next_node = current_node->next_node;
                current_node->next_node = node_to_add;
                return;
            }
            current_node = current_node->next_node;
        }
        current_node->next_node = node_to_add;
    }
}

struct node * starting_node(struct node * the_node){
	if (the_node->parent->parent != NULL)
		return starting_node(the_node->parent);
	else 
		return the_node;
}

int better_node_exist(struct list * the_list, struct node * node_to_search){
    if (the_list->root != NULL){
        struct node * current_node = the_list->root;
        while (current_node != NULL){
            if (current_node->x == node_to_search->x && current_node->y == node_to_search->y && current_node->f < node_to_search->f){
                return 1;
            }
            current_node = current_node->next_node;
        }
        return 0;
    } else {
        return 0;
    }
}

void free_list(struct list * the_list){
    if (the_list->root != NULL){
        struct node * current_node = the_list->root;
        while (current_node != NULL){
            struct node * temp_node = current_node;
            current_node = current_node->next_node;
            free(temp_node);
        }
    }
    free(the_list);
}

void free_node_and_list(struct node * current_node, struct node * parent_node, struct list * open_list, struct list * close_list){
    free(current_node);
    free(parent_node);
    free_list(open_list);
    free_list(close_list);
}