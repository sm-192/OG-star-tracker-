#ifndef COMMAND_STRINGS_H
#define COMMAND_STRINGS_H

/* clang-format off */
// headline
static const char head_line[] = "******************************************\r\n";
static const char head_line_debug[] = "***************Debug Build****************\r\n";
static const char head_line_version[] = "***     " BUILD_VERSION "  " __DATE__ "  " __TIME__ "    ***\r\n";
static const char head_line_tracker[] = "***         Starting Tracker Up        ***\r\n";

// general purpose
static const char malloc_failed[] = "Malloc failed!\r\n";
static const char not_supported[] = "not supported on this device\r\n";
static const char str_true[] = "true";
static const char str_false[] = "false";
static const char cr_nl[] = "\r\n";

const char* const string_table[] = {
    // headline
    head_line,
    head_line_debug,
    head_line_version,
    head_line_tracker,

    // general purpose
    malloc_failed,
    not_supported,
    str_true,
    str_false,
    cr_nl,
};
/* clang-format on */

enum pgm_table_index_t
{
    HEAD_LINE,
    HEAD_LINE_DEBUG,
    HEAD_LINE_VERSION,
    HEAD_LINE_TRACKER,

    MALLOC_FAILED,
    NOT_SUPPORTED,
    STR_TRUE,
    STR_FALSE,
    CR_NL,
};

#endif
