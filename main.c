#include <stdio.h>
#include <stdarg.h>

#include "debug_log.h"

int
main (int argc, char **argv)
{
  FLAG_SET (debug_output, DEBUG_OUTPUT_STDOUT);
  debug_log_open_file ("debug_log.log");

  debug_log ("test test, argc: %d", argc);

  debug_log_close_file ();
}

