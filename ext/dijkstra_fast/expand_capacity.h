#ifndef EXPAND_CAPACITY_H
#define EXPAND_CAPACITY_H

#define expand_capacity(TYPE, ITEMS, CAPACITY, INITIAL_CAPACITY, DESC) {  \
  unsigned long prev_capacity = CAPACITY;                                 \
  TYPE *prev_items = ITEMS;                                               \
                                                                          \
  if (CAPACITY == UINT_MAX) raise_max_capacity_reached_error(DESC);       \
  CAPACITY = prev_items == NULL ? INITIAL_CAPACITY : prev_capacity * 2;   \
  if (CAPACITY <= prev_capacity) CAPACITY = UINT_MAX;                     \
  ITEMS = calloc(CAPACITY, sizeof(TYPE));                                 \
                                                                          \
  if (prev_items != NULL) {                                               \
    memcpy(ITEMS, prev_items, prev_capacity * sizeof(TYPE));              \
    free(prev_items);                                                     \
  }                                                                       \
}

#endif
