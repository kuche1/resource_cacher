
#ifndef H_STRUTILS
#define H_STRUTILS

// TODO there must be a better way
#define UT_WARN_UNUSED __attribute__((warn_unused_result))

// this seems like the best solution, but I want gcc to give an error if I forget to check for null
// typedef char* str_or_null;

// typedef struct{
//     int null;
//     char *str;
// }str_or_null;

int UT_WARN_UNUSED str_copy(char **to, char *from);
int UT_WARN_UNUSED str_append(char **str_to_append_to, char *data_to_append);

int UT_WARN_UNUSED create_folder(char *path);
int UT_WARN_UNUSED create_folder_if_not_already(char *path);
int UT_WARN_UNUSED assert_folder_exists(char *path);
int UT_WARN_UNUSED folder_exists(char *path);

#endif // #ifndef H_STRUTILS
