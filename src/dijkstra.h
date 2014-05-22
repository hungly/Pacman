/**
  * Dijkstra algorithm used to determined the shortest path from a node to another node.
  * This algorithm is adapted from the Rosetta's code (2014) page with appropriate modification
  * to accept input provide by my program and provide output in appropriate format to be used by my AI
  * Reference: http://rosettacode.org/wiki/Dijkstra's_algorithm#C
  */

typedef struct node_t node_t, *heap_t;
typedef struct edge_t edge_t;

/**
  * Custom variable that represent the link (path) between two node and the cost of the connection.
  */
struct edge_t {
	node_t *destination_node;
	edge_t *sibling;
	int len;
};

/**
  * Custom variable that represent a node in the diagram with the connection with other node and it distance from original node
  */
struct node_t {
	edge_t *edge;
	node_t *via;
	int distance;
	int name;
	int heap_idx;
};

/**
  * Add an edge to the digram with start, end node and the cost.
  *
  * @param start pointer to the start node of this edge
  * @param end pointer to the end node of this edge
  * @param distance the cost to go from start node to end node
  */
void add_edge(node_t *start, node_t *end, int distance);

/**
  * Remove an edge from the memory.
  */
void free_edges();

/**
  * Set the distance from the original node to the curent node.
  *
  * @param nd pointer to the current checked node
  * @param via pointer to the previous node that lead to the current node from the original node.
  * @param d distance from the original node to this node
  */
void set_dist(node_t *destination_node, node_t *via, int d);

/**
  * Pop a node from the queue
  */
node_t * pop_queue();

/**
  * Calculate all the shortest path from one node to all other node in the memory.
  *
  * @param start pointer to the original node
  */
void calc_all(node_t *start);

/**
  * Show the next node to get to if you want to go from 1 node to other node.
  *
  * @param ns pointer to the starting node
  * @param nd pointer to the destination node
  *
  * @return the node which need to be reached if you want to archive the shortest path from 1 node to another node
  */
int show_path(node_t *start_node, node_t *destination_node);

/**
  * Calculate all the shortest path from ebery node to all other nodes that existed in the map.
  *
  * @param num_nodes indicates the number of nodes in the current loaded map
  * @param nodes_cost pointer to the pointer to an integer indicate an 2D array which store cost to go from one node to the node connected directly to it
  * @param routing_talbe pointer to the pointer to an integer indicates an 2D array which will store the result of the dijkstra algorithm for the AI to read from
  */
int dijkstra(const int num_nodes, int ** nodes_cost, int ** routing_table);