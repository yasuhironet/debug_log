#include <stdio.h>
#include <stdarg.h>

#include "debug_log.h"

int
main (int argc, char **argv)
{
  DEBUG_OUTPUT_SET (STDOUT);
  DEBUG_OUTPUT_FILE_SET ("debug_log.log");

  //DEBUG_SET(MTCP, SEND);
  DEBUG_UNSET(MTCP, RECV);
  DEBUG_SET(MTCP, SEQNUM);

  debug_log ("test test, argc: %d", argc);
  DEBUG_LOG(MTCP, RECV, "argc: %d, %s: %s",
            argc, "argv", (argc > 1 ? argv[1] : "null"));
  DEBUG_MTCP_LOG (SEQNUM,  "argc: %d, %s[%d]: %s",
            argc, "seqnum: argv", 2, (argc > 2 ? argv[2] : "null"));

  DEBUG_OUTPUT_FILE_UNSET ();
}

