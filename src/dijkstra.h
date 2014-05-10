/**
  * Dijkstra algorithm used to determined the shortest path from a node to another node.
  * This algorithm is adapted from the Rosetta's code (2014) page with appropriate modification
  * to accept input provide by my program and provide output in appropriate format to be used by my AI
  * Reference: http://rosettacode.org/wiki/Dijkstra's_algorithm#C
  */

typedef struct node_t node_t, *heap_t;
typedef struct edge_t edge_t;
struct edge_t {
	node_t *nd;	/* target of this edge */
	edge_t *sibling;/* for singly linked list */
	int len;	/* edge cost */
};
struct node_t {
	edge_t *edge;	/* singly linked list of edges */
	node_t *via;	/* where previous node is in shortest path */
	double dist;	/* distance from origining node */
	int name;	/* the, er, name */
	int heap_idx;	/* link to heap position for updating distance */
};

void add_edge(node_t *a, node_t *b, double d);

void free_edges();

void set_dist(node_t *nd, node_t *via, double d);

node_t * pop_queue();

void calc_all(node_t *start);

int show_path(node_t *ns, node_t *nd);

int dijkstra(const int num_nodes, int ** nodes_cost, int ** routing_table);