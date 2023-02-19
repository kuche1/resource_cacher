
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <dirent.h>

#include "resource_cacher.h"
#include "sha-2/sha-256.h"
#include "utils.h"

char *rc_dir_data = NULL;           // `/tmp/cache'
char *rc_dir_data_versioned = NULL; // `/tmp/cache/v1`
char *rc_dir_hash = NULL;           // `/tmp/cache/v1/sha256`
char *rc_dir_redirect = NULL;       // `/tmp/cache/v1/redirect`

int rc_test(void){
    if(rc_init()){
        return 1;
    }

    printf("rc_dir_data == %s\n", rc_dir_data);
    printf("rc_dir_data_versioned == %s\n", rc_dir_data_versioned);
    printf("rc_dir_hash == %s\n", rc_dir_hash);
    printf("rc_dir_redirect == %s\n", rc_dir_redirect);

    {
        char hash_as_str[65];
        char *str = "asdfg";
        if(rc_cache_data_str(strlen(str), str, hash_as_str)){
            printf("ERROR in call to rc_cache_data_str\n");
            goto error;
        }
    }

    {
        char *key = "test-01";
        char *data = "aaasddddffffg";
        if(rc_associate_key_str_to_data_str(key, strlen(data), data)){
            printf("ERROR asdasdasdsadsadsad\n");
            goto error;
        }
    }

    rc_deinit();
    return 0;
error:
    rc_deinit();
    return 1;
}

int rc_init(void){ // TODO check if init was already called
    char *data_dir = getenv(RC_ENV_DATA_DIR); // TODO get from command line and not from env?
    if(data_dir == NULL){
        fprintf(stderr, "ERROR: could not find env variable `%s`\n", RC_ENV_DATA_DIR);
        goto error;
    }
    if(str_copy(&rc_dir_data, data_dir)) goto error;
    if(rc_ass_fol_exs(rc_dir_data)) goto error;

    if(str_copy(&rc_dir_data_versioned, rc_dir_data)) goto error;
    if(str_append(&rc_dir_data_versioned, "/" RC_DATA_VERSION)) goto error;
    if(rc_create_folder_if_not_already(rc_dir_data_versioned)) goto error;

    if(str_copy(&rc_dir_hash, rc_dir_data_versioned)) goto error;
    if(str_append(&rc_dir_hash, "/sha256")) goto error; // TODO hash alg shouldn't be hardcoded
    if(rc_create_folder_if_not_already(rc_dir_hash)) goto error;

    if(str_copy(&rc_dir_redirect, rc_dir_data_versioned)) goto error;
    if(str_append(&rc_dir_redirect, "/redirect")) goto error; // TODO hash alg shouldn't be hardcoded
    if(rc_create_folder_if_not_already(rc_dir_redirect)) goto error;

    return 0;
error:
    rc_deinit();
    return 1;
}

void rc_deinit(void){
    free(rc_dir_data); rc_dir_data = NULL;
    free(rc_dir_data_versioned); rc_dir_data_versioned = NULL;
    free(rc_dir_hash); rc_dir_hash = NULL;
    free(rc_dir_redirect); rc_dir_redirect = NULL;
}

int rc_cache_data_str(int data_len, char* data, char hash_as_str[65]){
    // TODO chop `data` if `data_len` is greater than a certain number

    // TODO givi all the data is super inappropriate
    // what if data is 1GB, do we need to waste 1GB RAM?
    // what if data is 1TB, no1 has that much RAM
    //
    // maybe we could use a file descriptor instead
    //
    // maybe we could start caching downloads before they even finish

    char *folder = NULL;
    char *file = NULL;

    struct Sha_256 sha_256;
    uint8_t hash[32];
    sha_256_init(&sha_256, hash);
    sha_256_write(&sha_256, data, data_len);
    sha_256_close(&sha_256);

    long unsigned int i;
    // TODO I hate this and I hope the compiler optimizes it away
    for(i=0; i < sizeof(hash)/sizeof(*hash); ++i){
        sprintf(&hash_as_str[i*2], "%02x", hash[i]); // TODO is it worth using snprintf instead?
    }
    hash_as_str[i*2] = 0; // TODO not needed since `sprintf` copies the \0

    printf("trying to hash: %s\n", data);
    printf("result: %2x %2x %2x %02x\n", hash[0], hash[1], hash[2], 0);
    printf("full hash as str: %s\n", hash_as_str);

    // TODO using a fixed length static variable should improve the performance
    // and make the code prettyer
    // at the cost of adding a path limit (will most likely not be worth it)
    if(str_copy(&folder, rc_dir_hash)) goto error;
    // TODO those 2 calls can be combined (by modifying the str_append fnc)
    if(str_append(&folder, "/")) goto error;
    if(str_append(&folder, hash_as_str)) goto error;

    // TODO add integrity check, a simple `touch all-is-good` might be sufficient
    // simply using the folder name MIGHT be sufficient, but if we are to do so, a check mechanism needs to be implemented
    if(rc_folder_exists(folder)){ // using this super-basic check for now, assuming everything is fine if the folder exists
        goto return_;
    }

    if(rc_create_folder(folder)) goto error;

    if(str_copy(&file, folder)) goto error;
    if(str_append(&file, "/" RC_DATA_HASH_FILE_DATA)) goto error;

    FILE *f = fopen(file, "w");
    if(!f) goto error;
    fwrite(data, sizeof(*data), data_len, f); // TODO check ret?
    fclose(f);

    // TODO can be optimized
    free(file);
    if(str_copy(&file, folder)) goto error;
    if(str_append(&file, "/" RC_DATA_HASH_FILE_ACCESSED)) goto error;

    int32_t times_accesses = 1;
    f = fopen(file, "w"); // TODO might be more appropriate to change to `wb`
    if(!f) goto error;
    fwrite(&times_accesses, sizeof(times_accesses), 1, f); // TODO check ret?
    fclose(f);

    // TODO we can easily create a file for integrity check here, but it'll cost us some performance

return_:
    free(file);
    free(folder);
    return 0;
error:
    free(file);
    free(folder);
    return 1;
}

int rc_associate_key_str_to_data_str(char *key, int data_len, char *data){
    char *folder = NULL;
    if(str_copy(&folder, rc_dir_redirect)) goto error;

    char c;
    char c_sane[3];
    // TODO this seems too inefficient for my liking
    // hopefully the compiler optimizes it away
    // or the realloc implementation is good enough
    while(1){
        c = *key++;
        if(c == 0){
            break;
        }
        sprintf(c_sane, "%02x", c); // TODO is it worth using snprintf instead? // TODO it turns out that sorintf also copies the \0
        if(str_append(&folder, "/")) goto error;
        if(str_append(&folder, c_sane)) goto error;
        if(rc_create_folder_if_not_already(folder)) goto error;
        // TODO maybe we can replace those 3 ^^^ (and any other similar code blocks) with `opendir` ?
    }

    // this really should be called `file`, but I don't to risk any performance or add any additional `free`s
    if(str_append(&folder, "/" RC_DATA_REDIRECT_FILE_TARGET)) goto error;

    // TODO check if REDIRECT already exists, and if it does quit right away
    char hash_as_str[65];
    if(rc_cache_data_str(data_len, data, hash_as_str)) goto error;

    // TODO how are we going to structure the whole cut/reconstruct thing?
    FILE *f = fopen(folder, "w");
    if(!f) goto error;
    fwrite(hash_as_str, sizeof(*hash_as_str), sizeof(hash_as_str)/sizeof(*hash_as_str) - 1, f); // TODO check ret?
    fclose(f);

    free(folder);
    return 0;
error:
    free(folder);
    return 1;
}

///// helper functions

int rc_create_folder(char *path){
    if(mkdir(path, 0777)){ // TODO perms might be too loose
        fprintf(stderr, "ERROR: could not create directory `%s`\n", path);
        return 1;
    }
    return 0;
}

int rc_create_folder_if_not_already(char *path){
    if(rc_folder_exists(path)){
        return 0;
    }
    if(rc_create_folder(path)){
        return 1;
    }
    return 0;
}

int rc_ass_fol_exs(char *path){ // assert_folder_existance
    if(rc_folder_exists(path)){
        return 0;
    }
    printf("ERROR: directory doesn't exist `%s`\n", path);
    return 1;
}

int rc_folder_exists(char *path){
    DIR *dir = opendir(path);
    if(!dir){
        return 0;
    }
    closedir(dir);
    return 1;
}
