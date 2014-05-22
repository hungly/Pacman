#include "dijkstra.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curses.h>


#define BLOCK_SIZE 15
edge_t *edge_root = 0, *e_next = 0;

void add_edge(node_t *start, node_t *end, int distance) {
	if (e_next == edge_root) {
		edge_root = malloc(sizeof(edge_t) * (BLOCK_SIZE + 1));
		edge_root[BLOCK_SIZE].sibling = e_next;
		e_next = edge_root + BLOCK_SIZE;
	}
	--e_next;
 
	e_next->destination_node = end;
	e_next->len = distance;
	e_next->sibling = start->edge;
	start->edge = e_next;
}
 
void free_edges() {
	for (; edge_root; edge_root = e_next) {
		e_next = edge_root[BLOCK_SIZE].sibling;
		free(edge_root);
	}
}
 
heap_t *heap;
int heap_len;
 
void set_dist(node_t *destination_node, node_t *via, int d) {
	int i, j;
 
	/* already knew better path */
	if (destination_node->via && d >= destination_node->distance) return;
 
	/* find existing heap entry, or create a new one */
	destination_node->distance = d;
	destination_node->via = via;
 
	i = destination_node->heap_idx;
	if (!i) i = ++heap_len;
 
	/* upheap */
	for (; i > 1 && destination_node->distance < heap[j = i/2]->distance; i = j)
		(heap[i] = heap[j])->heap_idx = i;
 
	heap[i] = destination_node;
	destination_node->heap_idx = i;
}
 
node_t * pop_queue() {
	node_t *destination_node, *tmp;
	int i, j;
 
	if (!heap_len) return 0;
 
	/* remove leading element, pull tail element there and downheap */
	destination_node = heap[1];
	tmp = heap[heap_len--];
 
	for (i = 1; i < heap_len && (j = i * 2) <= heap_len; i = j) {
		if (j < heap_len && heap[j]->distance > heap[j+1]->distance) j++;
 
		if (heap[j]->distance >= tmp->distance) break;
		(heap[i] = heap[j])->heap_idx = i;
	}
 
	heap[i] = tmp;
	tmp->heap_idx = i;
 
	return destination_node;
}
 
void calc_all(node_t *start) {
	node_t *lead;
	edge_t *e;
 
	set_dist(start, start, 0);
	while ((lead = pop_queue()))
		for (e = lead->edge; e; e = e->sibling)
			set_dist(e->destination_node, lead, lead->distance + e->len);
}
 
int show_path(node_t *start_node, node_t *destination_node) {
	if (destination_node->via == destination_node)
		return destination_node->name;
	else if (!destination_node->via)
		return 999;
	else {
		if (destination_node->via == start_node) {
			return destination_node->name;
		} else {
			return show_path(start_node, destination_node->via);
		}
	}
}
 
int dijkstra(const int num_nodes, int ** nodes_cost, int ** routing_table) {
	int i;
 
#	define N_NODES num_nodes

	// For each node
	for (int start = 0; start < N_NODES; start++) {
		// create the nodes
		node_t *nodes = calloc(sizeof(node_t), N_NODES);
 	
 		// give the node name (ID)
		for (i = 0; i < N_NODES; i++) {
			nodes[i].name = i;
		}

#	define E(a, b, c) add_edge(nodes + a, nodes + b, c)
		// load the data from the cost table to the memory
		for (int i = 0; i < N_NODES; i++) {
			for (int j = 0; j < N_NODES; j++) {
				if (nodes_cost[i][j] != 0) {
					E(i, j, nodes_cost[i][j]);
				}
			}
		}
#	undef E

		// Create HEAP
		heap = calloc(sizeof(heap_t), N_NODES + 1);
		heap_len = 0;
 		
 		// Calculate all path fro current node to al other nodes
		calc_all(nodes + start);
		for (i = 0; i < N_NODES; i++) {
			routing_table[(nodes + start)->name][(nodes + i)->name] = show_path(nodes + start, nodes + i);
		}

		// Free memory
		free_edges();
		free(heap);
		free(nodes);
	}
	return 0;
}