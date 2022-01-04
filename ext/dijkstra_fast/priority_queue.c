#include <ruby.h>
#include <priority_queue.h>

const size_t PRIORITY_QUEUE_SIZE = sizeof(PriorityQueueStruct);

//////////////////////////////////////////////////////////////////////////////////////

void swap_prioritized_items(PrioritizedItem *items, unsigned long i, unsigned long j) {
  PrioritizedItem tmp = items[i];
  items[i] = items[j];
  items[j] = tmp;
}

void reprioritize_right(PrioritizedItem *items, unsigned long i, unsigned long size) {
  PRIORITY i_priority, j_priority_left, j_priority_right;
  unsigned long j_left, j_right;
  i_priority = items[i]->priority;

  while (true) {
    j_left = (i << 1) + 1;
    j_right = j_left + 1;

    j_priority_left = j_left < size ? items[j_left]->priority : PRIORITY_MAX;
    j_priority_right = j_right < size ? items[j_right]->priority : PRIORITY_MAX;

    if (j_priority_right < i_priority && j_priority_right < j_priority_left) {
      swap_prioritized_items(items, i, j_right);
      i = j_right;

    } else if (j_priority_left < i_priority) {
      swap_prioritized_items(items, i, j_left);
      i = j_left;

    } else {
      return;
    } 
  }
}

void reprioritize_left(PrioritizedItem *items, unsigned long i) {
  PRIORITY i_priority, j_priority;
  unsigned long j;
  i_priority = items[i]->priority;

  while (i > 0) {
    j = (i - 1) >> 1;
    j_priority = items[j]->priority;

    if (j_priority > i_priority) {
      swap_prioritized_items(items, i, j);
      i = j;

    } else {
      return;
    }
  }
}

//////////////////////////////////////////////////////////////////////////////////////

PriorityQueue make_priority_queue() {
  PriorityQueue pq = malloc(PRIORITY_QUEUE_SIZE);
  pq->size = 0;
  pq->capacity = 0;
  pq->items = NULL;
  expand_priority_queue(pq);
  return pq;
}

void free_priority_queue(PriorityQueue pq) {
  for (unsigned long i = 0; i < pq->size; i++) free(pq->items[i]);
  free(pq->items);
  free(pq);
}

bool priority_queue_is_empty(PriorityQueue pq) {
  return pq->size == 0 ? true : false;
}

void priority_queue_push(PriorityQueue pq, ITEM object, PRIORITY priority) {
  PrioritizedItem item = malloc(sizeof(PrioritizedItem));
  item->item = object;
  item->priority = priority;

  if (pq->size == pq->capacity) expand_priority_queue(pq);
  pq->items[pq->size] = item;
  reprioritize_left(pq->items, pq->size);
  pq->size++;
}

ITEM priority_queue_pop(PriorityQueue pq) {
  if (pq->size == 0) return 0;

  swap_prioritized_items(pq->items, 0, pq->size - 1);
  pq->size--;
  reprioritize_right(pq->items, 0, pq->size);

  ITEM item = pq->items[pq->size]->item;
  free(pq->items[pq->size]);

  return item;
}

// Safely sum two priorities without overflow
PRIORITY priority_sum(PRIORITY p1, PRIORITY p2) {
  PRIORITY sum = p1 + p2;
  return sum < p1 || sum < p2 ? PRIORITY_MAX : sum;
}
