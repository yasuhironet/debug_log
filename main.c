
#include "debug_log.h"

int
main (int argc, char **argv)
{
  debug_log_open_file ("debug_log.log");

  debug_log ("test test, argc: %d\n", argc);

  debug_log_close_file ();
}

