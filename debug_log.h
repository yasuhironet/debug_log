#ifndef __DEBUG_LOG_H__
#define __DEBUG_LOG_H__

#include <stdint.h> //uint64_t
#include <linux/limits.h> //PATH_MAX

#ifndef FLAG_SET
#define FLAG_CHECK(V,F) ((V) & (F))
#define FLAG_SET(V,F) ((V) |= (F))
#define FLAG_UNSET(V,F) ((V) &= ~(F))
#define FLAG_CLEAR(V,F) ((V) &= ~(F))
#define FLAG_RESET(V) ((V) = 0)
#define FLAG_ZERO(V) ((V) = 0)
#endif /*FLAG_SET*/

/* output index */
#define DEBUG_INDEX_STDOUT  0
#define DEBUG_IDNEX_STDERR  1
#define DEBUG_INDEX_SYSLOG  2
#define DEBUG_INDEX_FILE    3
#define DEBUG_INDEX_MAX     4

/* output flag */
#define DEBUG_OUTPUT_STDOUT  (1ULL << DEBUG_INDEX_STDOUT)
#define DEBUG_OUTPUT_STDERR  (1ULL << DEBUG_INDEX_STDERR)
#define DEBUG_OUTPUT_SYSLOG  (1ULL << DEBUG_INDEX_SYSLOG)
#define DEBUG_OUTPUT_FILE    (1ULL << DEBUG_INDEX_FILE)

/* syslog */
extern char *ident;
extern int option;
extern int level;
extern int facility;

/* file */
extern char *debug_log_filename;
extern FILE *debug_log_file;

/* category */
#define DEBUG_DEFAULT      0
#define DEBUG_CATEGORY_MAX 1

/* config, output */
extern uint64_t debug_config[];
extern uint64_t debug_output;

int debug_vlog (const char *format, va_list *args);
int debug_log (const char *format, ...);

void debug_log_open_syslog ();
void debug_log_close_syslog ();

void debug_log_open_file (char *filename);
void debug_log_close_file ();
void debug_log_rotate_file ();

#endif /*__DEBUG_LOG_H__*/
