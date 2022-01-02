#include <ruby.h>
#include <native.h>

static const rb_data_type_t native_typed_data = {
  "DijkstraFast/Native",
  { 0, free_native, },
  0, 0,
  RUBY_TYPED_FREE_IMMEDIATELY,
};

//////////////////////////////////////////////////////////////////////////////////////

void Init_native_shortest_path() {
  VALUE DijkstraFastModule = rb_const_get(rb_cObject, rb_intern("DijkstraFast"));
  VALUE NativeClass = rb_const_get(DijkstraFastModule, rb_intern("Native"));

  rb_define_alloc_func(NativeClass, native_allocate);
  rb_define_private_method(NativeClass, "_shortest_path", native_shortest_path, 3);
}

VALUE native_allocate(VALUE self) {
  Native n = make_native();
  return TypedData_Wrap_Struct(self, &native_typed_data, n);
}

VALUE native_shortest_path(VALUE self, VALUE source_obj, VALUE dest_obj, VALUE progress) {
  VALUE method_args[1];
  Native n;

  TypedData_Get_Struct(self, NativeStruct, &native_typed_data, n);

  ITEM source = NUM2INT(source_obj);
  ITEM dest = NUM2INT(dest_obj);

  priority_queue_push(n->pq, source, 0);
  update_distance(n, source, 0);

  while (!(priority_queue_is_empty(n->pq))) {
    n->u = priority_queue_pop(n->pq);

    if (n->u == dest) break; // We're done!!!
    if (has_visited(n, n->u)) continue;

    mark_visited(n, n->u);
    method_args[0] = INT2NUM(n->u);
    rb_block_call(self, rb_intern("connections"), 1, method_args, RUBY_METHOD_FUNC(with_connection), (VALUE)n);

    if (progress != Qnil) {
      n->completed++;
      rb_funcall(progress, rb_intern("call"), 2, INT2NUM(n->completed), INT2NUM(n->completed + n->pq->size));
    }
  }

  return to_result(n, source, dest);
}

VALUE with_connection(VALUE v_obj, VALUE context, int argc, VALUE extra[]) {
  Native n = (Native)context;
  ITEM u = n->u;
  ITEM v = NUM2INT(v_obj);

  if (has_visited(n, v)) return Qnil;

  PRIORITY distance = argc > 1 ? NUM2INT(extra[1]) : 1;
  PRIORITY alt_distance = priority_sum(get_distance(n, u), distance);

  if (alt_distance >= get_distance(n, v)) return Qnil;

  priority_queue_push(n->pq, v, alt_distance);
  update_distance(n, v, alt_distance);
  update_previous(n, v, u);

  return Qnil;
}

//////////////////////////////////////////////////////////////////////////////////////

VALUE to_result(Native n, ITEM source, ITEM dest) {
  PRIORITY distance = get_distance(n, dest);
  VALUE path = rb_ary_new();

  if (distance == PRIORITY_MAX) raise_no_path_error();

  while (dest != NULL_ITEM) {
    rb_ary_unshift(path, INT2NUM(dest));
    dest = dest == source ? NULL_ITEM : get_previous(n, dest);
  }

  return rb_ary_new3(2, INT2NUM(distance), path);
}
