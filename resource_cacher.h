
#ifndef H_RESOURCE_CACHER
#define H_RESOURCE_CACHER

// TODO there must be a better way
#define RC_WARN_UNUSED __attribute__((warn_unused_result))

#define RC_ENV_DATA_DIR "RESOURCE_CACHER_DATA_DIR"
#define RC_DATA_VERSION "v3"
#define RC_DATA_HASH_FILE_DATA "data"
#define RC_DATA_HASH_FILE_ACCESSED "accessed"
#define RC_DATA_REDIRECT_FILE_TARGET "target"

int RC_WARN_UNUSED rc_test(void);

int RC_WARN_UNUSED rc_init(void);
void rc_deinit(void);

int RC_WARN_UNUSED rc_cache_data_str(int data_len, char* data, char hash_as_str[65]);
int RC_WARN_UNUSED rc_associate_key_str_to_data_str(char *key, int data_len, char *data);

// helper functions, not meant to be used
// TODO we can put this in strutils and then we can rename it
// and we don't have to make it into it's own library FYI

int RC_WARN_UNUSED rc_create_folder(char *path);
int RC_WARN_UNUSED rc_create_folder_if_not_already(char *path);
int RC_WARN_UNUSED rc_ass_fol_exs(char *path);
int RC_WARN_UNUSED rc_folder_exists(char *path);

#endif // #ifndef H_RESOURCE_CACHER
