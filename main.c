#include <stdio.h>
#include <stdarg.h>

#include "debug_log.h"

void a ();
void b ();
void c ();
void d ();

void
a ()
{
  DEBUG_LOG (MTCP, PROCESS, "begin.");
  b ();
  DEBUG_LOG (MTCP, PROCESS, "end.");
}

void
b ()
{
  DEBUG_MTCP_LOG (PROCESS, "begin.");
  c ();
  d ();
  DEBUG_MTCP_LOG (PROCESS, "end.");
}

void
c ()
{
  DEBUG_MTCP_LOG (PROCESS, "begin.");
  DEBUG_MTCP_LOG (PROCESS, "end.");
}

void
d ()
{
  DEBUG_MTCP_LOG (PROCESS, "begin.");
  DEBUG_MTCP_LOG (PROCESS, "end.");
}

int
main (int argc, char **argv)
{
  debug_log_init (argv[0]);

  DEBUG_OUTPUT_SET (STDOUT);
  DEBUG_OUTPUT_FILE_SET ("debug_log.log");

  DEBUG_SET(MTCP, SEQNUM);
  DEBUG_SET(MTCP, PROCESS);
  DEBUG_SET(MTCP, ACK);
  DEBUG_UNSET(MTCP, RECV);
  //DEBUG_SET(MTCP, SEND);

  debug_log ("test test, argc: %d", argc);
  DEBUG_LOG(MTCP, RECV, "argc: %d, %s: %s",
            argc, "argv", (argc > 1 ? argv[1] : "null"));
  DEBUG_MTCP_LOG (SEQNUM,  "argc: %d, %s[%d]: %s",
            argc, "seqnum: argv", 2, (argc > 2 ? argv[2] : "null"));

  a ();

  DEBUG_OUTPUT_FILE_UNSET ();
}

