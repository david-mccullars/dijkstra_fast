#include <prioritized_item_list.h>

PrioritizedItemList make_prioritized_item_list(int capacity) {
  int i;

  PrioritizedItemList list = malloc(sizeof(PrioritizedItemListStruct));
  list->priorities = malloc(capacity * sizeof(PrioritizedItem));
  list->indices = malloc(capacity * sizeof(int));
  list->capacity = capacity;
  list->last = capacity - 1;
  for (i = 0; i < capacity; i++) {
    list->priorities[i] = malloc(sizeof(PrioritizedItemStruct));
    list->priorities[i]->item = i;
    list->priorities[i]->priority = INT_MAX;
    list->indices[i] = i;
  }
  return list;
}

bool empty_prioritized_item_list(PrioritizedItemList list) {
  return list->last < 0;
}

bool in_prioritized_item_list(PrioritizedItemList list, int item) {
  return list->indices[item] <= list->last;
}

void swap_prioritized_items(PrioritizedItemList list, int i, int j) {
  PrioritizedItem tmp = list->priorities[i];
  list->priorities[i] = list->priorities[j];
  list->priorities[j] = tmp;
  list->indices[list->priorities[i]->item] = i;
  list->indices[list->priorities[j]->item] = j;
}

void reprioritize_right(PrioritizedItemList list, int i) {
int orig_i = i;
  int wi, wj_left, wj_right;
  int j_left, j_right;
  wi = list->priorities[i]->priority;

  while (true) {
    j_left = (i << 1) + 1;
    j_right = j_left + 1;

    wj_left = j_left <= list->last ? list->priorities[j_left]->priority : INT_MAX;
    wj_right = j_right <= list->last ? list->priorities[j_right]->priority : INT_MAX;

    if (wj_right < wi && wj_right < wj_left) {
      swap_prioritized_items(list, i, j_right);
      i = j_right;

    } else if (wj_left < wi) {
      swap_prioritized_items(list, i, j_left);
      i = j_left;

    } else {
      return;
    } 
  }
}

void reprioritize_left(PrioritizedItemList list, int i) {
  int wi, wj;
  int j;
  wi = list->priorities[i]->priority;

  while (i > 0) {
    j = (i - 1) >> 1;
    wj = list->priorities[j]->priority;

    if (wj > wi) {
      swap_prioritized_items(list, i, j);
      i = j;

    } else {
      return;
    }
  }
}

int next_prioritized_item(PrioritizedItemList list) {
  PrioritizedItem item;

  if (empty_prioritized_item_list(list)) {
    return -1;
  }

  item = list->priorities[0];
  swap_prioritized_items(list, 0, list->last);
  list->last--;
  reprioritize_right(list, 0);

  return item->item;
}

void update_prioritized_item(PrioritizedItemList list, int item, int priority) {
  int i = list->indices[item];
  list->priorities[i]->priority = priority;

  if (i <= list->last) {
    reprioritize_left(list, i);
  }
}

int get_priority(PrioritizedItemList list, int item) {
  int index = list->indices[item];
  return list->priorities[index]->priority;
}

// For debugging only
void print_prioritized_item_list(PrioritizedItemList list) {
  for (int i = 0; i <= list->last; i++) {
    if (i > 0) printf(", ");
    printf("%d (%d)", list->priorities[i]->item, list->priorities[i]->priority);
  }
  printf("\n");
}

void free_prioritized_item_list(PrioritizedItemList list) {
  for (int i = 0; i < list->capacity; i++) {
    free(list->priorities[i]);
  }

  free(list->priorities);
  free(list->indices);
  free(list);
}
