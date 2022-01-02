#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include <ruby.h>
#include <errors.h>
#include <expand_capacity.h>

typedef unsigned long ITEM;
typedef unsigned int PRIORITY;
static const PRIORITY PRIORITY_MAX = UINT_MAX;
static const ITEM NULL_ITEM = 0;

struct PrioritizedItemStruct;
typedef struct PrioritizedItemStruct* PrioritizedItem;

typedef struct PrioritizedItemStruct {
  ITEM item;
  PRIORITY priority;
} PrioritizedItemStruct;

//////////////////////////////////////////////////////////////////////////////////////

struct PriorityQueueStruct;
typedef struct PriorityQueueStruct* PriorityQueue;

typedef struct PriorityQueueStruct {
  PrioritizedItem *items;
  unsigned long capacity;
  unsigned long size;
} PriorityQueueStruct;

//////////////////////////////////////////////////////////////////////////////////////

PriorityQueue make_priority_queue();
#define expand_priority_queue(pq) expand_capacity(PrioritizedItem, pq->items, pq->capacity, 256, "Priority Queue")
void free_priority_queue(PriorityQueue pq);
bool priority_queue_is_empty(PriorityQueue pq);
void priority_queue_push(PriorityQueue pq, ITEM object, PRIORITY priority);
ITEM priority_queue_pop(PriorityQueue pq);
PRIORITY priority_sum(PRIORITY p1, PRIORITY p2);

#endif
