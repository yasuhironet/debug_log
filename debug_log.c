#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdlib.h>
#include <syslog.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>

#include "debug_log.h"
#include "debug_category.h"

uint64_t debug_config[DEBUG_CATEGORY_MAX];
uint64_t debug_output;

pid_t pid = 0;

/* syslog */
char *ident = "debug_log";
int option = LOG_CONS|LOG_NDELAY|LOG_PID;
int level = LOG_INFO;
int facility = LOG_USER;

/* file */
char *debug_log_filename;
FILE *debug_log_file;

static int
snprintf_time (char *buf, int size)
{
  char timebuf[32];
  time_t clock;
  struct tm *tm;
  time (&clock);
  tm = localtime (&clock);
  strftime (timebuf, sizeof (timebuf), "%Y/%m/%d %H:%M:%S", tm);
  return snprintf (buf, size, "%s", timebuf);
}

int
debug_vlog_level (int level_spec, const char *format, va_list *args)
{
  int ret = 0;
  char timebuf[32];
  snprintf_time (timebuf, sizeof (timebuf));
  int priority;

  if (FLAG_CHECK (debug_output, DEBUG_OUTPUT_STDOUT))
    {
      ret = fprintf (stdout, "%s ", timebuf);
      ret += fprintf (stdout, "%s[%d]: ", ident, pid);
      ret += vfprintf (stdout, format, args[DEBUG_INDEX_STDOUT]);
      ret += fprintf (stdout, "\n");
      fflush (stdout);
    }

  if (FLAG_CHECK (debug_output, DEBUG_OUTPUT_STDERR))
    {
      ret = fprintf (stderr, "%s ", timebuf);
      ret += fprintf (stderr, "%s[%d]: ", ident, pid);
      ret += vfprintf (stderr, format, args[DEBUG_INDEX_STDERR]);
      ret += fprintf (stderr, "\n");
      fflush (stderr);
    }

  if (FLAG_CHECK (debug_output, DEBUG_OUTPUT_SYSLOG))
    {
      priority = level_spec | facility;
      vsyslog (priority, format, args[DEBUG_INDEX_SYSLOG]);
    }

  if (FLAG_CHECK (debug_output, DEBUG_OUTPUT_FILE))
    {
      ret = fprintf (debug_log_file, "%s ", timebuf);
      ret += fprintf (debug_log_file, "%s[%d]: ", ident, pid);
      ret += vfprintf (debug_log_file, format, args[DEBUG_INDEX_FILE]);
      ret += fprintf (debug_log_file, "\n");
      fflush (debug_log_file);
    }

  return ret;
}

int
debug_vlog (const char *format, va_list *args)
{
  /* using the default level. */
  return debug_vlog_level (level, format, args);
}

int
debug_log (const char *format, ...)
{
  va_list args[DEBUG_INDEX_MAX];
  int index;
  int ret;

  for (index = 0; index < DEBUG_INDEX_MAX; index++)
    va_start (args[index], format);

  ret = debug_vlog (format, args);

  for (index = 0; index < DEBUG_INDEX_MAX; index++)
    va_end (args[index]);

  return ret;
}

int
debug_warn (const char *format, ...)
{
  va_list args[DEBUG_INDEX_MAX];
  int index;
  int ret;

  for (index = 0; index < DEBUG_INDEX_MAX; index++)
    va_start (args[index], format);

  ret = debug_vlog_level (LOG_WARNING, format, args);

  for (index = 0; index < DEBUG_INDEX_MAX; index++)
    va_end (args[index]);

  return ret;
}

int
debug_error (const char *format, ...)
{
  va_list args[DEBUG_INDEX_MAX];
  int index;
  int ret;

  for (index = 0; index < DEBUG_INDEX_MAX; index++)
    va_start (args[index], format);

  ret = debug_vlog_level (LOG_ERR, format, args);

  for (index = 0; index < DEBUG_INDEX_MAX; index++)
    va_end (args[index]);

  return ret;
}

void
debug_log_open_syslog ()
{
  FLAG_SET (debug_output, DEBUG_OUTPUT_SYSLOG);
  openlog (ident, option, facility);
}

void
debug_log_close_syslog ()
{
  FLAG_UNSET (debug_output, DEBUG_OUTPUT_SYSLOG);
  closelog ();
}

void
debug_log_open_file (char *filename)
{
  if (! filename)
    return;
  if (! strlen (filename))
    return;

  if (debug_log_filename)
    free (debug_log_filename);
  debug_log_filename = strdup (filename);
  if (! debug_log_filename)
    {
      DEBUG_WARN ("filename strdup failed: %s: %s",
                   filename, strerror (errno));
      if (debug_log_file)
        {
          fclose (debug_log_file);
          debug_log_file = NULL;
        }
      FLAG_UNSET (debug_output, DEBUG_OUTPUT_FILE);
      return;
    }
  if (debug_log_file)
    fclose (debug_log_file);
  debug_log_file = fopen (debug_log_filename, "a");
  if (! debug_log_file)
    {
      DEBUG_WARN ("cannot open file: %s: %s",
                   debug_log_filename, strerror (errno));
      free (debug_log_filename);
      debug_log_filename = NULL;
      return;
    }
  FLAG_SET (debug_output, DEBUG_OUTPUT_FILE);
}

void
debug_log_close_file ()
{
  FLAG_UNSET (debug_output, DEBUG_OUTPUT_FILE);
  if (debug_log_filename)
    free (debug_log_filename);
  debug_log_filename = NULL;
  if (debug_log_file)
    fclose (debug_log_file);
  debug_log_file = NULL;
}

void
debug_log_rotate_file ()
{
  if (! FLAG_CHECK (debug_output, DEBUG_OUTPUT_FILE))
    return;

  if (debug_log_file)
    fclose (debug_log_file);
  if (debug_log_filename)
    debug_log_file = fopen (debug_log_filename, "a");
  else
    debug_log_file = NULL;
}

void
debug_log_init (char *progname)
{
  char *p;
  if (progname)
    {
      p = rindex (progname, '/');
      if (p)
        p++;
      if (p && strlen (p))
        ident = p;
      else
        ident = progname;
    }
  pid = getpid ();

  DEBUG_SET (DEFAULT, LOGGING);
  DEBUG_SET (DEFAULT, BACKTRACE);
}
