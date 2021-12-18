#ifndef PRIORITIZED_ITEM_LIST_H
#define PRIORITIZED_ITEM_LIST_H

#include <ruby.h>

struct PrioritizedItemStruct;
typedef struct PrioritizedItemStruct* PrioritizedItem;

typedef struct PrioritizedItemStruct {
  int item;
  int priority;
} PrioritizedItemStruct;

//////////////////////////////////////////////////////////////////////////////////////

struct PrioritizedItemListStruct;
typedef struct PrioritizedItemListStruct* PrioritizedItemList;

typedef struct PrioritizedItemListStruct {
  PrioritizedItem *priorities;
  int *indices;
  int capacity;
  int last;
} PrioritizedItemListStruct;

//////////////////////////////////////////////////////////////////////////////////////

PrioritizedItemList make_prioritized_item_list(int capacity);
bool empty_prioritized_item_list(PrioritizedItemList list);
bool in_prioritized_item_list(PrioritizedItemList list, int item);
int next_prioritized_item(PrioritizedItemList list);
void update_prioritized_item(PrioritizedItemList list, int item, int priority);
void print_prioritized_item_list(PrioritizedItemList list);
int get_priority(PrioritizedItemList list, int item);
void free_prioritized_item_list(PrioritizedItemList list);

//////////////////////////////////////////////////////////////////////////////////////

#endif
