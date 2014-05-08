#include "dijkstra.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curses.h>
 
/* --- edge management --- */
#define BLOCK_SIZE 15
edge_t *edge_root = 0, *e_next = 0;
 
/* Don't mind the memory management stuff, they are besides the point.
   Pretend e_next = malloc(sizeof(edge_t)) */
void add_edge(node_t *a, node_t *b, double d) {
	if (e_next == edge_root) {
		edge_root = malloc(sizeof(edge_t) * (BLOCK_SIZE + 1));
		edge_root[BLOCK_SIZE].sibling = e_next;
		e_next = edge_root + BLOCK_SIZE;
	}
	--e_next;
 
	e_next->nd = b;
	e_next->len = d;
	e_next->sibling = a->edge;
	a->edge = e_next;
}
 
void free_edges() {
	for (; edge_root; edge_root = e_next) {
		e_next = edge_root[BLOCK_SIZE].sibling;
		free(edge_root);
	}
}
 
/* --- priority queue stuff --- */
heap_t *heap;
int heap_len;
 
void set_dist(node_t *nd, node_t *via, double d) {
	int i, j;
 
	/* already knew better path */
	if (nd->via && d >= nd->dist) return;
 
	/* find existing heap entry, or create a new one */
	nd->dist = d;
	nd->via = via;
 
	i = nd->heap_idx;
	if (!i) i = ++heap_len;
 
	/* upheap */
	for (; i > 1 && nd->dist < heap[j = i/2]->dist; i = j)
		(heap[i] = heap[j])->heap_idx = i;
 
	heap[i] = nd;
	nd->heap_idx = i;
}
 
node_t * pop_queue() {
	node_t *nd, *tmp;
	int i, j;
 
	if (!heap_len) return 0;
 
	/* remove leading element, pull tail element there and downheap */
	nd = heap[1];
	tmp = heap[heap_len--];
 
	for (i = 1; i < heap_len && (j = i * 2) <= heap_len; i = j) {
		if (j < heap_len && heap[j]->dist > heap[j+1]->dist) j++;
 
		if (heap[j]->dist >= tmp->dist) break;
		(heap[i] = heap[j])->heap_idx = i;
	}
 
	heap[i] = tmp;
	tmp->heap_idx = i;
 
	return nd;
}
 
/* --- Dijkstra stuff; unreachable nodes will never make into the queue --- */
void calc_all(node_t *start) {
	node_t *lead;
	edge_t *e;
 
	set_dist(start, start, 0);
	while ((lead = pop_queue()))
		for (e = lead->edge; e; e = e->sibling)
			set_dist(e->nd, lead, lead->dist + e->len);
}
 
int show_path(node_t *ns, node_t *nd) {
	if (nd->via == nd)
		return nd->name;
	else if (!nd->via)
		return 999;
	else {
		if (nd->via == ns) {
			return nd->name;
		} else {
			return show_path(ns, nd->via);
		}
	}
}
 
int dijkstra(const int num_nodes, int ** nodes_cost, int ** routing_table) {
	int i;
 
#	define N_NODES num_nodes

	for (int start = 0; start < N_NODES; start++) {
		node_t *nodes = calloc(sizeof(node_t), N_NODES);
 
		for (i = 0; i < N_NODES; i++) {
			nodes[i].name = i;
		}
 
#	define E(a, b, c) add_edge(nodes + a, nodes + b, c)
		for (int i = 0; i < N_NODES; i++) {
			for (int j = 0; j < N_NODES; j++) {
				if (nodes_cost[i][j] != 0) {
					E(i, j, nodes_cost[i][j]);
				}
			}
		}
#	undef E

		heap = calloc(sizeof(heap_t), N_NODES + 1);
		heap_len = 0;
 	
		calc_all(nodes + start);
		for (i = 0; i < N_NODES; i++) {
			routing_table[(nodes + start)->name][(nodes + i)->name] = show_path(nodes + start, nodes + i);
		}

		free_edges();
		free(heap);
		free(nodes);
	}
	return 0;
}