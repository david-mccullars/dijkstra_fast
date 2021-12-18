#include <ruby.h>
#include <dijkstra_graph.h>
#include <prioritized_item_list.h>

const size_t VERTEX_LIST_SIZE = sizeof(VertexListStruct);
const size_t EDGE_LIST_SIZE = sizeof(EdgeListStruct);
const size_t VERTEX_SIZE = sizeof(VertexStruct);
const size_t EDGE_SIZE = sizeof(EdgeStruct);
const size_t GRAPH_SIZE = sizeof(GraphStruct);

static const rb_data_type_t graph_typed_data = {
  "Dijkstra/Graph",
  { 0, free_graph, },
  0, 0,
  RUBY_TYPED_FREE_IMMEDIATELY,
};

//////////////////////////////////////////////////////////////////////////////////////

void Init_dijkstra_graph() {
  VALUE DijkstraFastModule, GraphClass;

  DijkstraFastModule = rb_const_get(rb_cObject, rb_intern("DijkstraFast"));
  GraphClass = rb_const_get(DijkstraFastModule, rb_intern("Graph"));

  rb_define_alloc_func(GraphClass, dijkstra_graph_allocate);
  rb_define_private_method(GraphClass, "_add_edge", dijkstra_graph_add_edge, 3);
  rb_define_private_method(GraphClass, "_shortest_path", dijkstra_graph_shortest_path, 3);
}

VALUE dijkstra_graph_allocate(VALUE self) {
  Graph g = malloc(GRAPH_SIZE);

  // Grab a reference to the hash type used by a generic Ruby {}
  // which accepts any key and any value.  We'll need this type to create
  // a st_table in which to put arbitrary VALUE keys.  This hash type
  // should be a static constant and thus should be safe to utilize without
  // fear of garbage collection.
  const struct st_hash_type *objhash = rb_hash_tbl(rb_hash_new(), "dijkstra.c", 1)->type;

  g->vertex_count = 0;
  g->vertices = NULL;
  g->vertex_lookup = st_init_table_with_size(objhash, 0);

  return TypedData_Wrap_Struct(self, &graph_typed_data, g);
}

VALUE dijkstra_graph_add_edge(VALUE self, VALUE source_label, VALUE dest_label, VALUE distance) {
  Graph g;

  TypedData_Get_Struct(self, GraphStruct, &graph_typed_data, g);
  add_edge_with_labels(g, source_label, dest_label, NUM2INT(distance));
  return Qnil;
}

VALUE dijkstra_graph_shortest_path(VALUE self, VALUE source_label, VALUE dest_label, VALUE best_path) {
  Graph g;
  Vertex source, dest;

  TypedData_Get_Struct(self, GraphStruct, &graph_typed_data, g);
  source = lookup_vertex(g, source_label, false);
  dest = lookup_vertex(g, dest_label, false);

  if (source == NULL || dest == NULL) {
    return Qnil;
  } else {
    return INT2NUM(shortest_path(g, source, dest, best_path));
  }
}

//////////////////////////////////////////////////////////////////////////////////////

void free_graph(void *data) {
  Graph g = (Graph)data;

  struct EdgeListStruct **vertices;

  free_vertex_list(g->vertices, free_vertex);
  free(g->vertex_lookup);
  free(g);
}

void free_vertex(Vertex n) {
  free_edge_list(n->edges, free_edge);
  free(n);
}

void free_vertex_list(VertexList vertices, void (*free_item)(Vertex)) {
  VertexList tmp;
  while (vertices != NULL) {
    tmp = vertices;
    vertices = vertices->next;
    if (free_item) {
      free_item(tmp->vertex);
    }
    free(tmp);
  }
}

void free_edge(Edge e) {
  // Assume source and destination vertices were allocated elsewhere and will be free'd elsewhere
  free(e);
}

void free_edge_list(EdgeList edges, void (*free_item)(Edge)) {
  EdgeList tmp;
  while (edges != NULL) {
    tmp = edges;
    edges = edges->next;
    if (free_item) {
      free_item(tmp->edge);
    }
    free(tmp);
  }
}

//////////////////////////////////////////////////////////////////////////////////////

Vertex add_vertex(Graph g, VALUE label) {
  VertexList tmp = malloc(VERTEX_LIST_SIZE);

  tmp->vertex = malloc(VERTEX_SIZE);
  tmp->vertex->id = g->vertices != NULL ? g->vertices->vertex->id + 1 : 0; // Auto-incrementing id
  tmp->vertex->label = label;
  tmp->vertex->edges = NULL;

  tmp->next = g->vertices;
  g->vertices = tmp;
  g->vertex_count += 1;

  return tmp->vertex;
}

VertexList add_vertex_to_list(VertexList list, VALUE label) {
  VertexList tmp = malloc(VERTEX_LIST_SIZE);

  tmp->vertex = malloc(VERTEX_SIZE);
  tmp->vertex->label = label;
  tmp->vertex->edges = NULL;

  tmp->next = list;
  return tmp;
}

Edge add_edge(Vertex source, Vertex dest, int distance) {
  EdgeList tmp = malloc(EDGE_LIST_SIZE);

  tmp->edge = malloc(EDGE_SIZE);
  tmp->edge->source = source;
  tmp->edge->dest = dest;
  tmp->edge->distance = distance;

  tmp->next = source->edges;
  source->edges = tmp;

  return tmp->edge;
}

Edge add_edge_with_labels(Graph g, VALUE source_label, VALUE dest_label, int distance) {
  Vertex source, dest;

  source = lookup_vertex(g, source_label, true);
  dest = lookup_vertex(g, dest_label, true);

  return add_edge(source, dest, distance);
}

Vertex lookup_vertex(Graph g, VALUE label, bool create_if_missing) {
  Vertex n = NULL;

  if (!st_lookup(g->vertex_lookup, (st_data_t)label, (st_data_t *)&n)) {
    if (!create_if_missing) return NULL;
    n = add_vertex(g, label);
    st_add_direct(g->vertex_lookup, (st_data_t)label, (st_data_t)n);
  }
  return n;
}

//////////////////////////////////////////////////////////////////////////////////////

int shortest_path(Graph g, Vertex source, Vertex dest, VALUE best_path) {
  Vertex *items, *prevs;
  PrioritizedItemList list;

  int d, du, dv;
  Vertex u, v;
  VertexList vl;
  EdgeList el;
  bool reached = source == dest;

  items = malloc(g->vertex_count * sizeof(Vertex));
  prevs = malloc(g->vertex_count * sizeof(Vertex));
  list = make_prioritized_item_list(g->vertex_count);

  for (vl = g->vertices; vl != NULL; vl = vl->next) {
    v = vl->vertex;
    items[v->id] = v;
    prevs[v->id] = NULL;
  }

  update_prioritized_item(list, source->id, 0);

  while (!empty_prioritized_item_list(list)) {
    u = items[next_prioritized_item(list)];
    du = get_priority(list, u->id);
    for (el = u->edges; el != NULL; el = el->next) {
      v = el->edge->dest;
      dv = get_priority(list, v->id);
      d = du + el->edge->distance;
      if (d < 0) d = INT_MAX; // Wrapped around

      if (in_prioritized_item_list(list, v->id) && d < dv) {
        update_prioritized_item(list, v->id, d);
        prevs[v->id] = u;
        reached = reached || v == dest;
      }
    }
  }

  if (reached) {
    for (v = dest; v != NULL; v = prevs[v->id]) {
      rb_ary_unshift(best_path, v->label); 
    }
    d = get_priority(list, dest->id);
  } else {
    d = -1;
  }

  free(items);
  free(prevs);
  free_prioritized_item_list(list);

  return d;
}
