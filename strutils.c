
#include<string.h>
#include<stdlib.h>

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
