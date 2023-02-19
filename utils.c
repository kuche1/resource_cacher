
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>

#include "utils.h"

int str_copy(char **to, char *from){
    int len = strlen(from); // TODO can be optimized away
    char *copy = malloc(sizeof(*from) * (len+1));
    if(copy == NULL){
        return 1;
    }
    memcpy(copy, from, len+1);
    copy[len] = 0;
    *to = copy;
    return 0;
}

// TODO maybe we could be a bit smarter by using a struct instead of a regullar string
// so that we don't have to call realloc a million times
int str_append(char **str_to_append_to, char *data_to_append){
    int len = strlen(*str_to_append_to); // TODO can be optimized away
    int len_data_to_append = strlen(data_to_append); // TODO can be optimized away
    int new_len = len + len_data_to_append;

    char *new_str = realloc(*str_to_append_to, sizeof(**str_to_append_to) * (new_len+1));
    if(new_str == NULL){
        return 1;
    }
    memcpy(&new_str[len], data_to_append, len_data_to_append+1);

    *str_to_append_to = new_str;

    return 0;
}

int create_folder(char *path){
    if(mkdir(path, 0777)){ // TODO perms might be too loose
        fprintf(stderr, "ERROR: could not create directory `%s`\n", path);
        return 1;
    }
    return 0;
}

int create_folder_if_not_already(char *path){
    if(folder_exists(path)){
        return 0;
    }
    if(create_folder(path)){
        return 1;
    }
    return 0;
}

int assert_folder_exists(char *path){
    if(folder_exists(path)){
        return 0;
    }
    printf("ERROR: directory doesn't exist `%s`\n", path);
    return 1;
}

int folder_exists(char *path){
    DIR *dir = opendir(path);
    if(!dir){
        return 0;
    }
    closedir(dir);
    return 1;
}
