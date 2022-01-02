#ifndef NATIVE_H
#define NATIVE_H

#include <ruby.h>
#include <errors.h>
#include <expand_capacity.h>
#include <priority_queue.h>

struct ItemDataStruct;
typedef struct ItemDataStruct* ItemData;

typedef struct ItemDataStruct {
  ITEM previous;
  PRIORITY distance;
  bool visited;
} ItemDataStruct;

//////////////////////////////////////////////////////////////////////////////////////

struct NativeStruct;
typedef struct NativeStruct* Native;

typedef struct NativeStruct {
  PriorityQueue pq;
  ItemData *item_data;
  unsigned long item_data_capacity;
  ITEM u;         // current working item
  ITEM completed; // Number of items completed (for progress)
} NativeStruct;

//////////////////////////////////////////////////////////////////////////////////////

Native make_native();
#define expand_item_data(n) expand_capacity(ItemData, n->item_data, n->item_data_capacity, 256, "Dijkstra items")
void free_native(void *data);

ItemData make_item_data();
void free_item_data(ItemData id);

//////////////////////////////////////////////////////////////////////////////////////

void Init_native_shortest_path();
VALUE native_allocate(VALUE self);
VALUE native_shortest_path(VALUE self, VALUE source, VALUE dest, VALUE progress);
VALUE with_connection(VALUE v, VALUE context, int argc, VALUE extra[]);
VALUE to_result(Native n, ITEM source, ITEM dest);

//////////////////////////////////////////////////////////////////////////////////////

PRIORITY get_distance(Native n, ITEM u);
void update_distance(Native n, ITEM u, PRIORITY distance);
bool has_visited(Native n, ITEM v);
void mark_visited(Native n, ITEM v);
ITEM get_previous(Native n, ITEM v);
void update_previous(Native n, ITEM v, ITEM u);

#endif
