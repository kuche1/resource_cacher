
#ifndef H_STRUTILS
#define H_STRUTILS

// TODO there must be a better way
#define SU_WARN_UNUSED __attribute__((warn_unused_result))

// this seems like the best solution, but I want gcc to give an error if I forget to check for null
// typedef char* str_or_null;

// typedef struct{
//     int null;
//     char *str;
// }str_or_null;

int SU_WARN_UNUSED str_copy(char **to, char *from);

int SU_WARN_UNUSED str_append(char **str_to_append_to, char *data_to_append);

#endif // #ifndef H_STRUTILS
