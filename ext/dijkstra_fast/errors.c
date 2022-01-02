#include <ruby.h>
#include <errors.h>

void raise_max_capacity_reached_error(const char *expandable) {
  rb_raise(rb_eRuntimeError, "Maximum capacity reached for %s", expandable);
}

void raise_no_path_error() {
  VALUE DijkstraFastModule = rb_const_get(rb_cObject, rb_intern("DijkstraFast"));
  VALUE NoPathExistsError = rb_const_get(DijkstraFastModule, rb_intern("NoPathExistsError"));
  rb_raise(NoPathExistsError, "No path exists from source to destination");
}
