/** A struct which used to indicate cell for ghost pathfinding */

struct node {
	struct node * parent; 
    int x;
    int y;
    int f;
    int g;
    int h;
    struct node * next_node;
};

/** A list which will have a pointer to a node as it root */

struct list {
	struct node * root;
};

/**
  * Create a list
  *
  * @return the pointer to a newly created list 
  */
struct list * list_create();

/**
  * Create a node
  *
  * @return the pointer to a newly created node
  */
struct node * node_create();

/** 
  * Find a node with least f (distance from start plus distance from destination) 
  * in a list
  *
  * @param the_list pointer to the list in which you want to search for 
  * 
  * @return pointer to the node in the list that have least f
  */
struct node * node_with_least_f(struct list * the_list);

/**
  * Pop (remove) a node out of a list (the node is still exist)
  *
  * @param the_list pointer to the list we want to remove a node from
  * @pram node_to_pop pointer to the node we want to pop
  */
void pop_a_node_off_list(struct list * the_list, struct node * node_to_pop);

/**
  * Add a node to a list
  *
  * @param the_list pointer to the list which we want to add a node to
  * @param node_to_add pointer to a node which we want to add to a list
  */
void add_a_node_to_list(struct list * the_list, struct node * node_to_add);

/**
  * Find the starting node of a node (exclude the 1st)
  *
  * @param the_node pointer to that node
  *
  * @return the starting of that node
  */
struct node * starting_node(struct node * the_node);

/**
  * Check if there is a better node is exist in a list or not (same position but lower f) 
  * 
  * @param the_list pointer to the list we want to check
  * @param node_to_search: pointer to the node we want to check
  *
  * @return 1 if exist and 0 if not exist
  */
int better_node_exist(struct list * the_list, struct node * node_to_search);

/**
  * Free a list
  *
  * @param the_list pointer to the list we want to free 
  */
void free_list(struct list * the_list);

/**
  * Free current working node and all list
  *
  * @param current_node pointer to the current node
  * @param parrent_node pointer to the parent of current node
  * @param open_list pointer to a list (specifically open list in this case)
  * @param close_list pointer to a list (specifically close list in this case)
  */
void free_node_and_list(struct node * current_node, struct node * parent_node, struct list * open_list, struct list * close_list);