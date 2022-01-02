#include <ruby.h>
#include <native.h>

const size_t NATIVE_SIZE = sizeof(NativeStruct);
const size_t ITEM_DATA_SIZE = sizeof(ItemDataStruct);

//////////////////////////////////////////////////////////////////////////////////////

Native make_native() {
  Native n = malloc(NATIVE_SIZE);
  n->pq = make_priority_queue();
  n->item_data_capacity = 0;
  n->item_data = NULL;
  n->u = NULL_ITEM;
  n->completed = 0;
  expand_item_data(n);
  return n;
}

void free_native(void *data) {
  Native n = (Native)data;
  free_priority_queue(n->pq);
  for (unsigned long i = 0; i < n->item_data_capacity; i++) {
    if (n->item_data[i] != NULL) free_item_data(n->item_data[i]);
  }
  free(n->item_data);
  free(n);
}

ItemData make_item_data() {
  ItemData id = malloc(ITEM_DATA_SIZE);
  id->previous = 0;
  id->distance = PRIORITY_MAX;
  id->visited = false;
  return id;
}

void free_item_data(ItemData id) {
  free(id);
}

//////////////////////////////////////////////////////////////////////////////////////

ItemData get_item_data(Native n, ITEM u, bool create_if_not_exists) {
  if (create_if_not_exists) {
    while (n->item_data_capacity <= u) expand_item_data(n);
    if (n->item_data[u] == NULL) n->item_data[u] = make_item_data();
  }
  return n->item_data_capacity <= u ? NULL : n->item_data[u];
}

PRIORITY get_distance(Native n, ITEM u) {
  ItemData uid = get_item_data(n, u, false);
  return uid == NULL ? PRIORITY_MAX : uid->distance;
}

void update_distance(Native n, ITEM u, PRIORITY distance) {
  ItemData uid = get_item_data(n, u, true);
  uid->distance = distance;
}

bool has_visited(Native n, ITEM v) {
  ItemData vid = get_item_data(n, v, false);
  return vid == NULL ? false : vid->visited;
}

void mark_visited(Native n, ITEM v) {
  ItemData vid = get_item_data(n, v, true);
  vid->visited = true;
}

ITEM get_previous(Native n, ITEM v) {
  ItemData vid = get_item_data(n, v, false);
  return vid == NULL ? 0 : vid->previous;
}

void update_previous(Native n, ITEM v, ITEM u) {
  ItemData vid = get_item_data(n, v, true);
  vid->previous = u;
}
