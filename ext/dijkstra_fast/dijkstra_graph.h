#ifndef DIJKSTRA_GRAPH_H
#define DIJKSTRA_GRAPH_H

#include <ruby.h>

struct VertexListStruct;
typedef struct VertexListStruct* VertexList;

typedef struct VertexListStruct {
  struct VertexStruct *vertex;
  struct VertexListStruct *next;
} VertexListStruct;

//////////////////////////////////////////////////////////////////////////////////////

struct EdgeListStruct;
typedef struct EdgeListStruct* EdgeList;

typedef struct EdgeListStruct {
  struct EdgeStruct *edge;
  struct EdgeListStruct *next;
} EdgeListStruct;

//////////////////////////////////////////////////////////////////////////////////////

struct VertexStruct;
typedef struct VertexStruct* Vertex;

typedef struct VertexStruct {
  EdgeList edges;
  int id;
  VALUE label;
} VertexStruct;

//////////////////////////////////////////////////////////////////////////////////////

struct EdgeStruct;
typedef struct EdgeStruct* Edge;

typedef struct EdgeStruct {
  Vertex source;
  Vertex dest;
  int distance;
} EdgeStruct;

//////////////////////////////////////////////////////////////////////////////////////

struct GraphStruct;
typedef struct GraphStruct* Graph;

typedef struct GraphStruct {
  unsigned long vertex_count;
  VertexList vertices;
  st_table *vertex_lookup;
} GraphStruct;

//////////////////////////////////////////////////////////////////////////////////////

void free_graph(void *data);
void free_vertex(Vertex n);
void free_vertex_list(VertexList vertices, void (*free_item)(Vertex));
void free_edge(Edge e);
void free_edge_list(EdgeList edges, void (*free_item)(Edge));

//////////////////////////////////////////////////////////////////////////////////////

Vertex add_vertex(Graph g, VALUE label);
Edge add_edge(Vertex source, Vertex dest, int distance);
Edge add_edge_with_labels(Graph g, VALUE source_label, VALUE dest_label, int distance);
Vertex lookup_vertex(Graph g, VALUE label, bool create_if_missing);

//////////////////////////////////////////////////////////////////////////////////////

int shortest_path(Graph g, Vertex source, Vertex dest, VALUE best_path);

//////////////////////////////////////////////////////////////////////////////////////

void Init_dijkstra_graph();
VALUE dijkstra_graph_allocate(VALUE self);
VALUE dijkstra_graph_add_edge(VALUE self, VALUE source_label, VALUE dest_label, VALUE distance);
VALUE dijkstra_graph_shortest_path(VALUE self, VALUE source_label, VALUE dest_label, VALUE best_path);

#endif
