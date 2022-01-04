#ifndef ERRORS_H
#define ERRORS_H

_Noreturn void raise_max_capacity_reached_error(const char *expandable);
_Noreturn void raise_no_path_error();

#endif
