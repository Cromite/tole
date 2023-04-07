#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

char **token;
int i = 0;

/**
 * data type
*/
typedef struct type {
    char *key;
    int type;
    int scope;
    int items;
    struct type *object;

    char *string;
    double number;
    int bool;
    
    int length;
    struct type *array;

    char **function;
    int prc;
    struct type *param;
    int id;
} Type;

int scope = 0;
Type built;

/**
 * skip
*/
void skip_expression(char ***token);
void skip_value(char ***token) {
    if ((**token)[0] == '(' || (**token)[0] == '[') {
        (*token) ++;
        skip_expression(token);
        while ((**token)[0] == ',') {
            (*token) ++;
            skip_expression(token);
        }
        (*token) ++;
    }
    else if ((**token)[0] == '"') (*token) += 3;
    else if ((**token)[0] == '+' || (**token)[0] == '-' || (**token)[0] == '!' || (**token)[0] == '~') {
        (*token) ++;
        skip_value(token);
    }
    else if ((**token)[0] >= '0' && (**token)[0] <= '9') (*token) ++;
    else if (
        ((**token)[0] >= 'A' && (**token)[0] <= 'Z') ||
        ((**token)[0] >= 'a' && (**token)[0] <= 'z') || (**token)[0] == '_'
    ) (*token) ++;

    while ((**token)[0] == '[' || (**token)[0] == '.' || (**token)[0] == '(') {
        if ((**token)[0] == '.') (*token) ++;
        skip_value(token);
    }

    if (((**token)[0] == '+' && (**token)[1] == '+') || ((**token)[0] == '-' && (**token)[1] == '-')) (*token) ++;
}
void skip_logical_operation(char ***token) {
    skip_value(token);
    while (
        (**token)[0] == '+' ||
        (**token)[0] == '-' ||
        (**token)[0] == '*' ||
        (**token)[0] == '/' ||
        (**token)[0] == '%' ||
        (**token)[0] == '=' ||
        (**token)[0] == '<' ||
        (**token)[0] == '>' ||
        (**token)[0] == '&' ||
        (**token)[0] == '|' ||
        (**token)[0] == '^'
    ) {
        (*token) ++;
        skip_value(token);
    }
}
void skip_expression(char ***token) {
    skip_logical_operation(token);
    while (
        (**token)[0] == '?' ||
        (**token)[0] == ':'
    ) {
        (*token) ++;
        skip_logical_operation(token);
    }
}
void skip_process(char ***token) {

    if ((**token)[0] == '{') {
        (*token) ++;
        while ((**token)[0] != '}') {
            skip_process(token);
        }
        (*token) ++;
    }

    else if ((
        (**token)[0] == 'i' &&
        (**token)[1] == 'n' &&
        (**token)[2] == 'i' &&
        (**token)[3] == 't' &&
        (**token)[4] == '\0'
    ) || (
        (**token)[0] == 'e' &&
        (**token)[1] == 'x' &&
        (**token)[2] == 'i' &&
        (**token)[3] == 't' &&
        (**token)[4] == '\0'
    ) || (
        (**token)[0] == 'd' &&
        (**token)[1] == 'e' &&
        (**token)[2] == 'l' &&
        (**token)[3] == 'e' &&
        (**token)[4] == 't' &&
        (**token)[5] == 'e' &&
        (**token)[6] == '\0'
    ) || (
        (**token)[0] == 'b' &&
        (**token)[1] == 'r' &&
        (**token)[2] == 'e' &&
        (**token)[3] == 'a' &&
        (**token)[4] == 'k' &&
        (**token)[5] == '\0'
    ) || (
        (**token)[0] == 'c' &&
        (**token)[1] == 'o' &&
        (**token)[2] == 'n' &&
        (**token)[3] == 't' &&
        (**token)[4] == 'i' &&
        (**token)[5] == 'n' &&
        (**token)[6] == 'u' &&
        (**token)[7] == 'e' &&
        (**token)[8] == '\0'
    )) (*token) ++;

    else if ((
        (**token)[0] == 'f' &&
        (**token)[1] == 'o' &&
        (**token)[2] == 'r' &&
        (**token)[3] == '\0'
    ) || (
        (**token)[0] == 'r' &&
        (**token)[1] == 'e' &&
        (**token)[2] == 't' &&
        (**token)[3] == 'u' &&
        (**token)[4] == 'r' &&
        (**token)[5] == 'n' &&
        (**token)[6] == '\0'
    ) || (
        (**token)[0] == 's' &&
        (**token)[1] == 'w' &&
        (**token)[2] == 'i' &&
        (**token)[3] == 't' &&
        (**token)[4] == 'c' &&
        (**token)[5] == 'h' &&
        (**token)[6] == '\0'
    )) {
        (*token) ++;
        skip_expression(token);
    }
    
    else if ((
        (**token)[0] == 'e' &&
        (**token)[1] == 'l' &&
        (**token)[2] == 's' &&
        (**token)[3] == 'e' &&
        (**token)[4] == '\0'
    ) || (
        (**token)[0] == 'd' &&
        (**token)[1] == 'e' &&
        (**token)[2] == 'f' &&
        (**token)[3] == 'a' &&
        (**token)[4] == 'u' &&
        (**token)[5] == 'l' &&
        (**token)[6] == 't' &&
        (**token)[7] == '\0'
    ) || (
        (**token)[0] == 'd' &&
        (**token)[1] == 'o' &&
        (**token)[2] == '\0'
    )) {
        (*token) ++;
        skip_process(token);
    }

    else if ((
        (**token)[0] == 'w' &&
        (**token)[1] == 'h' &&
        (**token)[2] == 'i' &&
        (**token)[3] == 'l' &&
        (**token)[4] == 'e' &&
        (**token)[5] == '\0'
    ) || (
        (**token)[0] == 'i' &&
        (**token)[1] == 'f' &&
        (**token)[2] == '\0'
    ) || (
        (**token)[0] == 'i' &&
        (**token)[1] == 'n' &&
        (**token)[2] == '\0'
    ) || (
        (**token)[0] == 'c' &&
        (**token)[1] == 'a' &&
        (**token)[2] == 's' &&
        (**token)[3] == 'e' &&
        (**token)[4] == '\0'
    )) {
        (*token) ++;
        skip_expression(token);
        skip_process(token);
    }

    else {
        skip_expression(token);
        while ((**token)[0] == ',') {
            (*token) ++;
            skip_expression(token);
        }
    }

}

/**
 * process
*/
Type expression(char ***token, Type *defined);
Type process(char ***token, Type *defined);

/**
 * just value, literals
*/
Type raw_value(char ***token, Type *defined) {
    Type res;
    res.type = 0;
    res.key = NULL;
    res.items = 0;
    res.object = NULL;

    Type *value = &res;

    // grouping operator
    if ((**token)[0] == '(') {
        value->param = NULL;
        value->prc = 0;
        while ((**token)[0] != ')') {
            (*token) ++;
            value->prc ++;
            value->param = realloc(value->param, sizeof(Type) * value->prc);
            value->param[value->prc - 1] = expression(token, defined);
            //printf("%s, %d\n",value->array[0].key, value->array[0].type);
        }
        (*token) ++;        
        if ((**token)[0] == '=' && (**token)[1] == '>') {
            (*token) ++;
            value->type = 5;
            value->id = 0;
            value->function = *token;
            skip_process(token);
        } else {
            *value = value->param[value->prc - 1];
        }
    }

    // object literal
    else if ((**token)[0] == '[') {
        value->array = NULL;
        value->type = 4;
        value->length = 0;
        while ((**token)[0] != ']') {
            (*token) ++;
            value->length ++;
            value->array = realloc(value->array, sizeof(Type) * value->length);
            value->array[value->length - 1] = expression(token, defined);
            //printf("%s, %d\n",value->object[item].key, value->object[item].type);
        }
        (*token) ++;
    }

    // NOT operator
    else if ((**token)[0] == '!') {
        (*token) ++;
        Type val = raw_value(token, defined);
        if (val.type == 3) {
            value->type = 3;
            value->bool = !val.bool;
        }
        else value->type = -2;
    }
    else if ((**token)[0] == '~') {
        (*token) ++;
        Type val = raw_value(token, defined);
        if (val.type == 2) {
            value->type = 2;
            value->number = ~(int)val.number;
        }
        else value->type = -2;
    }

    // boolean literals
    else if (
        (**token)[0] == 't' &&
        (**token)[1] == 'r' &&
        (**token)[2] == 'u' &&
        (**token)[3] == 'e' &&
        (**token)[4] == '\0'
    ) {
        value->type = 3;
        value->bool = 1;
        (*token) ++;
    }
    else if (
        (**token)[0] == 'f' &&
        (**token)[1] == 'a' &&
        (**token)[2] == 'l' &&
        (**token)[3] == 's' &&
        (**token)[4] == 'e' &&
        (**token)[5] == '\0'
    ) {
        value->type = 3;
        value->bool = 0;
        (*token) ++;
    }

    // void literal
    else if (
        (**token)[0] == 'v' &&
        (**token)[1] == 'o' &&
        (**token)[2] == 'i' &&
        (**token)[3] == 'd' &&
        (**token)[4] == '\0'
    ) {
        value->type = 0;
        (*token) ++;
    }

    // error literal
    else if (
        (**token)[0] == 'e' &&
        (**token)[1] == 'r' &&
        (**token)[2] == 'r' &&
        (**token)[3] == 'o' &&
        (**token)[4] == 'r' &&
        (**token)[5] == '\0'
    ) {
        value->type = -2;
        (*token) ++;
    }

    // undefined literal
    else if (
        (**token)[0] == 'u' &&
        (**token)[1] == 'n' &&
        (**token)[2] == 'd' &&
        (**token)[3] == 'e' &&
        (**token)[4] == 'f' &&
        (**token)[5] == 'i' &&
        (**token)[6] == 'n' &&
        (**token)[7] == 'e' &&
        (**token)[8] == 'd' &&
        (**token)[9] == '\0'
    ) {
        value->type = -1;
        (*token) ++;
    }

    // string literal
    else if ((**token)[0] == '"') {
        (*token) ++;
        value->type = 1;
        value->string = strdup(**token);
        value->object = built.object[1].object;
        value->items = built.object[1].items;
        (*token) += 2;
    }

    // numeric literal
    else if ((**token)[0] == '+') {
        (*token) ++;
        Type val = raw_value(token, defined);
        if (val.type == 2) {
            value->number = val.number;
            value->type = 2;
        }
        else value->type = -2;
    }
    else if ((**token)[0] == '-') {
        (*token) ++;
        Type val = raw_value(token, defined);
        if (val.type == 2) {
            value->number = - val.number;
            value->type = 2;
        }
        else value->type = -2;
    }
    else if ((**token)[0] >= '0' && (**token)[0] <= '9') {
        switch ((**token)[1]) {
            case 'b': value->number = (double)strtol(**token + 2, NULL, 2); break;
            case 'o': value->number = (double)strtol(**token + 2, NULL, 8); break;
            case 'x': value->number = (double)strtol(**token + 2, NULL, 16); break;
            default: value->number = strtod(**token, NULL);
        }
        value->type = 2;
        (*token) ++;
    }

    else if (
        (**token)[0] == 's' &&
        (**token)[1] == 'e' &&
        (**token)[2] == 'l' &&
        (**token)[3] == 'f' &&
        (**token)[4] == '\0'
    ) {
        value = defined;
        (*token) ++;
    }

    // identifer
    else if (
        ((**token)[0] >= 'A' && (**token)[0] <= 'Z') ||
        ((**token)[0] >= 'a' && (**token)[0] <= 'z') || (**token)[0] == '_'
    ) {
        value->type = -1;
        value->key = (**token);
        for (int v = built.items - 1; v >= 0; v--) {
            if (strcmp(**token, built.object[v].key) == 0) {
                value = &built.object[v];
                break;
            }
        }
        (*token) ++;
    }

    Type self = *value;
    while ((**token)[0] == '[' || (**token)[0] == '.' || (**token)[0] == '(') {
        if ((**token)[0] == '[') {
            (*token) ++;
            Type val = expression(token, defined);
            if ((**token)[0] != ']' || val.type != 2 || value->type != 4) {
                while ((**token)[0] != ']') {
                    (*token) ++;
                    skip_expression(token);
                }
                (*token) ++;
            } else {
                if ((int)val.number >= value->length) {
                    value->array = realloc(value->array, sizeof(Type) * ((int)val.number + 1));
                    value->length = ((int)val.number + 1);
                }
                value = &value->array[(int)val.number];
                (*token) ++;
            }
        }
        else if ((**token)[0] == '@') {
            (*token) ++;
            Type res;
            switch (value->type) {
                case 7: {
                    int id = value->items - 1;
                    while (id >= 0) {
                        if (strcmp(**token, value->object[id].key) == 0) {
                            value = &value->object[id];
                            break;
                        }
                        id --;
                    }
                    if (id == -1) value->type = -1;
                }
                break;
                case 1:
                    if (
                        (**token)[0] == 'u' && 
                        (**token)[1] == 'p' && 
                        (**token)[2] == 'p' && 
                        (**token)[3] == 'e' && 
                        (**token)[4] == 'r' && 
                        (**token)[5] == '\0'
                    ){
                        int i = strlen(value->string);
                        res.string = malloc(sizeof(char) * i);
                        for (int l = 0; l < i; l ++) {
                            res.string[l] = value->string[l] >= 'a' && value->string[l] <= 'z' ? value->string[l] - 32 : value->string[l];
                        }
                        res.type = 1;
                    }
                    else if (
                        (**token)[0] == 'l' && 
                        (**token)[1] == 'o' && 
                        (**token)[2] == 'w' && 
                        (**token)[3] == 'e' && 
                        (**token)[4] == 'r' && 
                        (**token)[5] == '\0'
                    ){
                        int i = strlen(value->string);
                        res.string = malloc(sizeof(char) * i);
                        for (int l = 0; l < i; l ++) {
                            res.string[l] = value->string[l] >= 'A' && value->string[l] <= 'Z' ? value->string[l] + 32 : value->string[l];
                        }
                        res.type = 1;
                    }
                    else res.type = -1;
                break;
            }
            value = &res;
            (*token) ++;
        }
        else if ((**token)[0] == '.') {
            self = *value;   // here
            (*token) ++;
            int id = value->items - 1;
            while (id >= 0) {
                if (value->object[id].key != NULL)
                if (strcmp(**token, value->object[id].key) == 0) {
                    value = &value->object[id];
                    break;
                }
                id --;
            }
            if (id == -1) {
                value->items ++;
                value->object = realloc(value->object, sizeof(Type) * value->items);
                value->object[value->items - 1].key = **token;
                value = &value->object[value->items - 1];
            }
            (*token) ++;
        }
        else if ((**token)[0] == '(') {
            Type *param = NULL;
            int item = 0;
            while ((**token)[0] != ')') {
                (*token) ++;
                item ++;
                param = realloc(param, sizeof(Type) * item);
                param[item - 1] = expression(token, defined);
            }
            (*token) ++;
            if ((**token)[0] == '=' && (**token)[1] == '>') {
                (*token) ++;
                value->type = 5;
                value->id = 0;
                value->function = *token;
                value->param = param;
                value->prc = item;
                skip_process(token);
            } else {
                Type res;
                if (value->type == 5) {
                    if (value->id == 0) {
                        Type this = *value;
                        int it = 0, hit = 0;
                        while (it < this.prc) {
                            if (this.param[it].key != NULL) {
                                built.items ++;
                                built.object = realloc(built.object, sizeof(Type) * built.items);
                                built.object[built.items - 1] = param[it];
                                built.object[built.items - 1].key = this.param[it].key;
                                built.object[built.items - 1].scope = scope;
                                hit ++;
                            }
                            it ++;
                        }
                        res = process(&this.function, &self);
                        if (res.type == -5) res.type = res.scope;
                        built.items -= hit;
                        built.object = realloc(built.object, sizeof(Type) * built.items);
                    }
                    else switch (value->id) {
                        case 1: { // print
                            if (item == 1 && param[0].type == 1) {
                                puts(param[0].string);
                                res.type = 0;
                            }
                            else res.type = -2;
                            break;
                        }
                        case 2: { // str 
                            if (item == 1) {
                                res.string = malloc(sizeof(char) * BUFSIZ);
                                switch (param[0].type) {
                                    case -2: strcpy(res.string, "error"); break;
                                    case -1: strcpy(res.string, "undefined"); break;
                                    case 1: strcpy(res.string, param[0].string); break;
                                    case 2: sprintf(res.string, "%.16g", param[0].number); break;
                                    case 3: strcpy(res.string, param[0].bool ? "true" : "false"); break;
                                    case 4: strcpy(res.string, "[]"); break;
                                    case 5: strcpy(res.string, "{}"); break;
                                    default: res.string[0] = '\0';
                                }
                                res.string = realloc(res.string, sizeof(char) * strlen(res.string));
                                res.type = 1;
                            }
                            else res.type = -2;
                            break;
                        }
                        case 3: { // scan
                            if (item == 1 && (param[0].type == 0 || param[0].type == 1)) {
                                if (param[0].type == 1) printf("%s", param[0].string);
                                res.string = malloc(sizeof(char) * BUFSIZ);
                                fgets(res.string, BUFSIZ, stdin);
                                int len = strlen(res.string);
                                res.string[len - 1] = '\0';
                                res.string = realloc(res.string, sizeof(char) * len);
                                res.type = 1;
                            }
                            else res.type = -2;
                            break;
                        }
                        case 4: { // tod
                            if (item == 1) {
                                switch (param[0].type) {
                                    case 1: res.number = atof(param[0].string); break;
                                    case 2: res.number = param[0].number; break;
                                    case 3: res.number = param[0].bool; break;
                                    default: res.number = 0;
                                }
                                res.type = 2;
                            }
                            else res.type = -2;
                            break;
                        }
                        case 101: { // length
                            if (item == 1 && param[0].type == 0) {
                                res.number = strlen(self.string);
                                res.type = 2;
                            }
                            else res.type = -2;
                            break;
                        }
                        case 102: { // find
                            if (item == 1 && param[0].type == 1) {
                                char* index = strstr(self.string, param[0].string);
                                res.number = index != NULL ? index - self.string : -1;
                                res.type = 2;
                            }
                            else res.type = -2;
                            break;
                        }
                        case 103: { // replace
                            if (item == 2 && param[0].type == 1 && param[1].type == 1) {
                                res.string = strdup(self.string);
                                char *what_pos = strstr(res.string, param[0].string);
                                if (what_pos != NULL) {
                                    size_t what_len = strlen(param[0].string), with_len = strlen(param[1].string);
                                    char *remain = what_pos + what_len;
                                    memmove(what_pos + with_len, remain, strlen(remain) + 1);
                                    memcpy(what_pos, param[1].string, with_len);
                                }
                                res.type = 1;
                                res.object = built.object[1].object;
                                res.items = built.object[1].items;
                            }
                            else res.type = -2;
                            break;
                        }
                        case 104: { // replace_all
                            if (item == 2 && param[0].type == 1 && param[1].type == 1) {
                                res.string = strdup(self.string);
                                char *what_pos = strstr(res.string, param[0].string);
                                size_t what_len = strlen(param[0].string), with_len = strlen(param[1].string);
                                while ((what_pos = strstr(what_pos, param[0].string)) != NULL) {
                                    char *remain = what_pos + what_len;
                                    memmove(what_pos + with_len, remain, strlen(remain) + 1);
                                    memcpy(what_pos, param[1].string, with_len);
                                }
                                res.type = 1;
                                res.object = built.object[1].object;
                                res.items = built.object[1].items;
                            }
                            else res.type = -2;
                            break;
                        }
                        case 105: { // split
                            if (item == 1 && param[0].type == 1) {
                                res.array = NULL;
                                res.length = 0;
                                res.type = 4;
                                char *string = strdup(self.string);
                                char *token = strtok(string, param[0].string);
                                while (token != NULL) {
                                    res.length ++;
                                    res.array = realloc(res.array, sizeof(Type) * res.length);
                                    res.array[res.length - 1].type = 1;
                                    res.array[res.length - 1].object = built.object[1].object;
                                    res.array[res.length - 1].items = built.object[1].items;
                                    res.array[res.length - 1].string = token;
                                    token = strtok(NULL, param[0].string);
                                }
                            }
                            else res.type = -2;
                            break;
                        }
                        case 106: { // starts_with
                            if (item == 1 && param[0].type == 1) {
                                res.bool = strncmp(self.string, param[0].string, strlen(param[0].string)) == 0;
                                res.type = 3;
                            }
                            else res.type = -2;
                            break;
                        }
                        case 107: { // ends_with
                            if (item == 1 && param[0].type == 1) {
                                size_t str_len = strlen(self.string);
                                size_t suffix_len = strlen(param[0].string);
                                res.bool = str_len < suffix_len ? 0 : strcmp(self.string + str_len - suffix_len, param[0].string) == 0;
                                res.type = 3;
                            }
                            else res.type = -2;
                            break;
                        }
                        case 108: { // reverse
                            if (item == 1 && param[0].type == 0) {
                                int len = strlen(self.string);
                                res.string = malloc(sizeof(char) * len);
                                for (int l = 0; l < len; l ++) res.string[l] = self.string[len - l - 1];
                                res.type = 1;
                                res.object = built.object[1].object;
                                res.items = built.object[1].items;
                            }
                            else res.type = -2;
                            break;
                        }
                        case 601: { // read
                            if (item == 1 && param[0].type == 1) {
                                FILE *fp = fopen(param[0].string, "rb");
                                if (fp == NULL) res.type = -2;
                                else {
                                    fseek(fp, 0, SEEK_END);
                                    long size = ftell(fp);
                                    res.string = (char*)malloc(size + 1);
                                    fseek(fp, 0, SEEK_SET);
                                    fread(res.string, size, 1, fp);
                                    fclose(fp);
                                    res.type = 1;
                                    res.object = built.object[1].object;
                                    res.items = built.object[1].items;
                                }
                            }
                            else res.type = -2;
                            break;
                        }
                        case 602: { // write
                            if (item == 2 && param[0].type == 1 && param[1].type == 1) {
                                FILE *fp = fopen(param[0].string, "wb");
                                fwrite(param[1].string, strlen(param[1].string), 1, fp);
                                fclose(fp);
                                res.type = 0;
                            }
                            else res.type = -2;
                            break;
                        }
                        case 603: { // exist
                            if (item == 1 && param[0].type == 1) {
                                res.bool = fopen(param[0].string, "rb") != NULL;
                                res.type = 3;
                            }
                            else res.type = -2;
                            break;
                        }
                    }
                } else res.type = -2; 
                value = &res;
            }
        }
    }

    // assignment
    if ((**token)[0] == '=' && (**token)[1] == '\0') {
        (*token) ++;
        Type val = expression(token, defined);
        if (value->type == -1) {
            built.items ++;
            built.object = realloc(built.object, sizeof(Type) * built.items);
            built.object[built.items - 1].scope = scope;
            built.object[built.items - 1].key = value->key;
            value = &built.object[built.items - 1];
        }
        value->type = val.type;
        value->object = val.object;
        value->items = val.items;
        switch (value->type) {
            case 1: value->string = val.string; break;
            case 2: value->number = val.number; break;
            case 3: value->bool = val.bool; break;
            case 5:
            value->id = val.id;
            value->function = val.function;
            value->param = val.param;
            value->prc = val.prc;
            break;
            case 4:
            value->array = val.array;
            value->length = val.length;
            break;
        }
    }

    // increment
    else if ((**token)[0] == '+' && (**token)[1] == '+') {
        (*token) ++;
        if (value->type == 2) value->number ++;
        else value->type = -2;
    }

    // decrement
    else if ((**token)[0] == '-' && (**token)[1] == '-') {
        (*token) ++;
        if (value->type == 2) value->number --;
        else value->type = -2;
    }

    else if ((**token)[0] == '+' && (**token)[1] == '=') {
        (*token) ++;
        Type val = expression(token, defined);
        if (value->type == 1 && val.type == 1) {
            value->string = realloc(value->string, sizeof(char) * (strlen(value->string) + strlen(val.string)));
            strcat(value->string, val.string);
        }
        else if (value->type == 2 && val.type == 2) value->number += val.number;
        else value->type = -2;
    }
    else if ((**token)[0] == '-' && (**token)[1] == '=') {
        (*token) ++;
        Type val = expression(token, defined);
        if (value->type == 2 && val.type == 2) value->number -= val.number;
        else value->type = -2;
    }
    else if ((**token)[0] == '*' && (**token)[1] == '=') {
        (*token) ++;
        Type val = expression(token, defined);
        if (value->type == 2 && val.type == 2) value->number *= val.number;
        else value->type = -2;
    }
    else if ((**token)[0] == '/' && (**token)[1] == '=') {
        (*token) ++;
        Type val = expression(token, defined);
        if (value->type == 2 && val.type == 2) value->number /= val.number;
        else value->type = -2;
    }
    else if ((**token)[0] == '%' && (**token)[1] == '=') {
        (*token) ++;
        Type val = expression(token, defined);
        if (value->type == 2 && val.type == 2) value->number = value->number - (int)(value->number / val.number) * val.number;
        else value->type = -2;
    }
    else if ((**token)[0] == '&' && (**token)[1] == '=') {
        (*token) ++;
        Type val = expression(token, defined);
        if (value->type == 2 && val.type == 2) value->number = (int)value->number & (int)val.number;
        else value->type = -2;
    }
    else if ((**token)[0] == '|' && (**token)[1] == '=') {
        (*token) ++;
        Type val = expression(token, defined);
        if (value->type == 2 && val.type == 2) value->number = (int)value->number | (int)val.number;
        else value->type = -2;
    }
    else if ((**token)[0] == '^' && (**token)[1] == '=') {
        (*token) ++;
        Type val = expression(token, defined);
        if (value->type == 2 && val.type == 2) value->number = (int)value->number ^ (int)val.number;
        else value->type = -2;
    }
    else if ((**token)[0] == '*' && (**token)[1] == '*' && (**token)[2] == '=') {
        (*token) ++;
        Type val = expression(token, defined);
        if (value->type == 2 && val.type == 2) value->number = pow(value->number, val.number);
        else value->type = -2;
    }
    else if ((**token)[0] == '&' && (**token)[1] == '&' && (**token)[2] == '=') {
        (*token) ++;
        Type val = expression(token, defined);
        if (value->type == 3 && val.type == 3) value->bool = value->bool && val.bool;
        else value->type = -2;
    }
    else if ((**token)[0] == '|' && (**token)[1] == '|' && (**token)[2] == '=') {
        (*token) ++;
        Type val = expression(token, defined);
        if (value->type == 3 && val.type == 3) value->bool = value->bool || val.bool;
        else value->type = -2;
    }
    else if ((**token)[0] == '<' && (**token)[1] == '<' && (**token)[2] == '=') {
        (*token) ++;
        Type val = expression(token, defined);
        if (value->type == 2 && val.type == 2) value->number = (int)value->number << (int)val.number;
        else value->type = -2;
    }
    else if ((**token)[0] == '>' && (**token)[1] == '>' && (**token)[2] == '=') {
        (*token) ++;
        Type val = expression(token, defined);
        if (value->type == 2 && val.type == 2) value->number = (int)value->number >> (int)val.number;
        else value->type = -2;
    }

    return *value;
}

/**
 * arithmetic operators
 * `+` `-` `*` `**` `/` `%` `<<` `>>`
*/
Type arithmetic_operation(char ***token, Type *defined) {
    
    Type res;
    Type value = raw_value(token, defined);

    res.type = value.type;
    res.key = value.key;
    res.object = value.object;
    res.items = value.items;
    switch (res.type) {
        case 1:
            res.string = strdup(value.string);
            while ((**token)[0] == '+') {
                (*token) ++;
                Type val = raw_value(token, defined);
                if (val.type == 1) {
                    res.string = realloc(res.string, sizeof(char) * (strlen(res.string) + strlen(val.string)));
                    strcat(res.string, val.string);
                }
                else {
                    res.type = -2;
                    break;
                }
            }
        break;
        case 2:
            res.number = value.number;
            while ((**token)[0] == '+' || (**token)[0] == '-' || (**token)[0] == '*' || (**token)[0] == '/' || (**token)[0] == '%' || (**token)[1] == '<' || (**token)[1] == '>') {
                if ((**token)[0] == '+') {
                    (*token) ++;
                    Type val = arithmetic_operation(token, defined);
                    if (val.type == 2) res.number = res.number + val.number;
                    else {
                        res.type = -2;
                        break;
                    }
                }
                else if ((**token)[0] == '-') {
                    (*token) ++;
                    Type val = arithmetic_operation(token, defined);
                    if (val.type == 2) res.number = res.number - val.number;
                    else {
                        res.type = -2;
                        break;
                    }
                }
                else if ((**token)[0] == '*') {
                    if ((**token)[1] == '*') {
                        (*token) ++;
                        Type val = raw_value(token, defined);
                        if (val.type == 2) res.number = pow(res.number, val.number);
                        else {
                            res.type = -2;
                            break;
                        }
                    } else {
                        (*token) ++;
                        Type val = raw_value(token, defined);
                        if (val.type == 2) res.number = res.number * val.number;
                        else {
                            res.type = -2;
                            break;
                        }
                    }
                }
                else if ((**token)[0] == '/') {
                    (*token) ++;
                    Type val = raw_value(token, defined);
                    if (val.type == 2) res.number = res.number / val.number;
                    else {
                        res.type = -2;
                        break;
                    }
                }
                else if ((**token)[0] == '%') {
                    (*token) ++;
                    Type val = raw_value(token, defined);
                    if (val.type == 2) res.number = res.number - (int)(res.number / val.number) * val.number;
                    else {
                        res.type = -2;
                        break;
                    }
                }
                else if ((**token)[1] == '<') {
                    (*token) ++;
                    Type val = arithmetic_operation(token, defined);
                    if (val.type == 2) res.number = (int)res.number << (int)val.number;
                    else {
                        res.type = -2;
                        break;
                    }
                }
                else if ((**token)[1] == '>') {
                    (*token) ++;
                    Type val = arithmetic_operation(token, defined);
                    if (val.type == 2) res.number = (int)res.number >> (int)val.number;
                    else {
                        res.type = -2;
                        break;
                    }
                }
            }
        break;
        case 3: res.bool = value.bool; break;
        case 5: res.id = value.id; res.function = value.function; 
            res.param = value.param;
            res.prc = value.prc;
        break;
        case 4: res.array = value.array; res.length = value.length; break;
    }

    while ((**token)[0] == '+' || (**token)[0] == '-' || (**token)[0] == '*' || (**token)[0] == '/' || (**token)[0] == '%' || (**token)[1] == '<' || (**token)[1] == '>') {
        res.type = -2;
        (*token) ++;
        raw_value(token, defined);
    }

    return res;
}

/**
 * relational operators
 * `==` `!=` `<` `>` `<=` `>=`
*/
Type relational_operation(char ***token, Type *defined) {

    Type res;
    Type value = arithmetic_operation(token, defined);

    res.type = value.type;
    res.key = value.key;
    res.object = value.object;
    res.items = value.items;
    switch (res.type) {
        case 0:
            while ((**token)[1] == '=' && ((**token)[0] == '=' || (**token)[0] == '!')) {
                res.type = 3;
                if ((**token)[0] == '=') {
                    (*token) ++;
                    Type val = arithmetic_operation(token, defined);
                    res.bool = val.type == 0 ? 1 : 0;
                }
                else {
                    (*token) ++;
                    Type val = arithmetic_operation(token, defined);
                    res.bool = val.type == 0 ? 0 : 1;
                }
            }
        break;
        case 1:
            res.string = value.string;
            while ((**token)[1] == '=' && ((**token)[0] == '=' || (**token)[0] == '!')) {
                res.type = 3;
                if ((**token)[0] == '=') {
                    (*token) ++;
                    Type val = arithmetic_operation(token, defined);
                    res.bool = val.type == 1 ? strcmp(res.string, val.string) == 0 : 0;
                }
                else {
                    (*token) ++;
                    Type val = arithmetic_operation(token, defined);
                    res.bool = val.type == 1 ? strcmp(res.string, val.string) != 0 : 0;
                }
            }
        break;
        case 2:
            res.number = value.number;
            while ((**token)[1] == '=' || (**token)[0] == '<' || (**token)[0] == '>') {
                res.type = 3;
                if ((**token)[0] == '=') {
                    (*token) ++;
                    Type val = arithmetic_operation(token, defined);
                    res.bool = val.type == 2 ? res.number == val.number : 0;
                } 
                else if ((**token)[0] == '!') {
                    (*token) ++;
                    Type val = arithmetic_operation(token, defined);
                    res.bool = val.type == 2 ? res.number != val.number : 0;
                }
                else if ((**token)[0] == '<') {
                    if ((**token)[1] == '=') {
                        (*token) ++;
                        Type val = arithmetic_operation(token, defined);
                        if (val.type == 2) res.bool = res.number <= val.number;
                        else {
                            res.type = -2;
                            break;
                        }
                    } else {
                        (*token) ++;
                        Type val = arithmetic_operation(token, defined);
                        if (val.type == 2) res.bool = res.number < val.number;
                        else {
                            res.type = -2;
                            break;
                        }
                    }
                }
                else if ((**token)[0] == '>') {
                    if ((**token)[1] == '=') {
                        (*token) ++;
                        Type val = arithmetic_operation(token, defined);
                        if (val.type == 2) res.bool = res.number >= val.number;
                        else {
                            res.type = -2;
                            break;
                        }
                    } else {
                        (*token) ++;
                        Type val = arithmetic_operation(token, defined);
                        if (val.type == 2) res.bool = res.number > val.number;
                        else {
                            res.type = -2;
                            break;
                        }
                    }
                }
            }
        break;
        case 3:
            res.bool = value.bool;
            while ((**token)[1] == '=' && ((**token)[0] == '=' || (**token)[0] == '!')) {
                res.type = 3;
                if ((**token)[0] == '=') {
                    (*token) ++;
                    Type val = arithmetic_operation(token, defined);
                    res.bool = val.type == 3 ? res.bool == val.bool : 0;
                }
                else {
                    (*token) ++;
                    Type val = arithmetic_operation(token, defined);
                    res.bool = val.type == 3 ? res.bool != val.bool : 0;
                }
            }
        break;
        case 4:
            res.array = value.array;
            res.length = value.length;
            while ((**token)[1] == '=' && ((**token)[0] == '=' || (**token)[0] == '!')) {
                res.type = 3;
                if ((**token)[0] == '=') {
                    (*token) ++;
                    Type val = arithmetic_operation(token, defined);
                    res.bool = val.type == 4 ? 1 : 0;
                }
                else {
                    (*token) ++;
                    Type val = arithmetic_operation(token, defined);
                    res.bool = val.type == 4 ? 0 : 1;
                }
            }
        break;
        case 5:
            res.id = value.id;
            res.function = value.function;
            res.param = value.param;
            res.prc = value.prc;
            while ((**token)[1] == '=' && ((**token)[0] == '=' || (**token)[0] == '!')) {
                res.type = 3;
                if ((**token)[0] == '=') {
                    (*token) ++;
                    Type val = arithmetic_operation(token, defined);
                    res.bool = val.type == 5 ? 1 : 0;
                }
                else {
                    (*token) ++;
                    Type val = arithmetic_operation(token, defined);
                    res.bool = val.type == 5 ? 0 : 1;
                }
            }
        break;
        case -1:
            while ((**token)[1] == '=' && ((**token)[0] == '=' || (**token)[0] == '!')) {
                res.type = 3;
                if ((**token)[0] == '=') {
                    (*token) ++;
                    Type val = arithmetic_operation(token, defined);
                    res.bool = val.type == -1 ? 1 : 0;
                }
                else {
                    (*token) ++;
                    Type val = arithmetic_operation(token, defined);
                    res.bool = val.type == -1 ? 0 : 1;
                }
            }
        break;
        case -2:
            while ((**token)[1] == '=' && ((**token)[0] == '=' || (**token)[0] == '!')) {
                res.type = 3;
                if ((**token)[0] == '=') {
                    (*token) ++;
                    Type val = arithmetic_operation(token, defined);
                    res.bool = val.type == -2 ? 1 : 0;
                }
                else {
                    (*token) ++;
                    Type val = arithmetic_operation(token, defined);
                    res.bool = val.type == -2 ? 0 : 1;
                }
            }
        break;
    }

    while ((**token)[1] == '=' || (**token)[0] == '<' || (**token)[0] == '>') {
        res.type = -2;
        (*token) ++;
        raw_value(token, defined);
    }

    return res;
}

/**
 * logical operators
 * `&&` `||` `&` `|` `^`
*/
Type logical_operation(char ***token, Type *defined) {
    
    Type res;
    Type value = relational_operation(token, defined);

    res.type = value.type;
    res.key = value.key;
    res.object = value.object;
    res.items = value.items;
    switch (res.type) {
        case 3:
            res.bool = value.bool;
            while ((**token)[1] == '&' || (**token)[1] == '|') {
                if ((**token)[1] == '&') {
                    (*token) ++;
                    Type val = relational_operation(token, defined);
                    if (val.type == 3) res.bool = res.bool && val.bool;
                    else {
                        res.type = -2;
                        break;
                    }
                } else {
                    (*token) ++;
                    Type val = relational_operation(token, defined);
                    if (val.type == 3) res.bool = res.bool || val.bool;
                    else {
                        res.type = -2;
                        break;
                    }
                }
            }
        break;
        case 2:
            res.number = value.number;
            while (((**token)[0] == '&' || (**token)[0] == '|' || (**token)[0] == '^') && (**token)[1] == '\0') {
                if ((**token)[0] == '&') {
                    (*token) ++;
                    Type val = relational_operation(token, defined);
                    if (val.type == 2) res.number = (int)res.number & (int)val.number;
                    else {
                        res.type = -2;
                        break;
                    }
                } else if ((**token)[0] == '|') {
                    (*token) ++;
                    Type val = relational_operation(token, defined);
                    if (val.type == 2) res.number = (int)res.number | (int)val.number;
                    else {
                        res.type = -2;
                        break;
                    }
                } else {
                    (*token) ++;
                    Type val = relational_operation(token, defined);
                    if (val.type == 2) res.number = (int)res.number ^ (int)val.number;
                    else {
                        res.type = -2;
                        break;
                    }
                }
            }
        break;
        case 1: res.string = value.string; break;
        case 5: res.id = value.id; res.function = value.function; 
            res.param = value.param;
            res.prc = value.prc;
        break;
        case 4: res.array = value.array; res.length = value.length; break;
    }

    while ((**token)[0] == '&' || (**token)[0] == '|' || (**token)[0] == '^') {
        res.type = -2;
        (*token) ++;
        raw_value(token, defined);
    }
    
    return res;
}

/**
 * expression
*/
Type expression(char ***token, Type *defined) {

    Type res = logical_operation(token, defined);

    while ((**token)[0] == '?' || (**token)[0] == ':') {
        switch ((**token)[0]) {
            case '?':
                (*token) ++;
                if (res.type == 3) {
                    if (res.bool == 1) res = logical_operation(token, defined);
                    else {
                        res.type = 0;
                        skip_logical_operation(token);
                    }
                } else res.type = -2;
            break;
            case ':':
                (*token) ++;
                if (res.type == 0 || res.type == -1 || res.type == -2) res = logical_operation(token, defined);
                else skip_logical_operation(token);
        }
    }

    return res;
}

/**
 * process
 * 
 * -6 exit
 * -5 return
 * -4 break
 * -3 continue
 * -2 error
 * -1 undefined
 * 0 - 7
*/
Type process(char ***token, Type *defined) {

    Type res;
    res.type = 0;

    if ((**token)[0] == '{') {
        (*token) ++;
        scope ++;
        do res = process(token, defined); while (res.type >= -2 && (**token)[0] != '}');
        while ((**token)[0] != '}') skip_process(token);
        if (res.type >= -2) res.type = 0;
        (*token) ++;
        scope --;
        while (built.object[built.items - 1].scope > scope) {
            built.items --;
            built.object = realloc(built.object, sizeof(Type) * built.items);
        }
    }

    else if (
        (**token)[0] == 'e' &&
        (**token)[1] == 'x' &&
        (**token)[2] == 'i' &&
        (**token)[3] == 't' &&
        (**token)[4] == '\0'
    ) {
        (*token) ++;
        res.type = -6;
    }


    else if (
        (**token)[0] == 'r' &&
        (**token)[1] == 'e' &&
        (**token)[2] == 't' &&
        (**token)[3] == 'u' &&
        (**token)[4] == 'r' &&
        (**token)[5] == 'n' &&
        (**token)[6] == '\0'
    ) {
        (*token) ++;
        res = expression(token, defined);
        res.scope = res.type;
        res.type = -5;
    }

    else if (
        (**token)[0] == 'b' &&
        (**token)[1] == 'r' &&
        (**token)[2] == 'e' &&
        (**token)[3] == 'a' &&
        (**token)[4] == 'k' &&
        (**token)[5] == '\0'
    ) {
        (*token) ++;
        res.type = -4;
    }

    else if (
        (**token)[0] == 'c' &&
        (**token)[1] == 'o' &&
        (**token)[2] == 'n' &&
        (**token)[3] == 't' &&
        (**token)[4] == 'i' &&
        (**token)[5] == 'n' &&
        (**token)[6] == 'u' &&
        (**token)[7] == 'e' &&
        (**token)[8] == '\0'
    ) {
        (*token) ++;
        res.type = -3;
    }

    else if (
        (**token)[0] == 'd' &&
        (**token)[1] == 'e' &&
        (**token)[2] == 'l' &&
        (**token)[3] == 'e' &&
        (**token)[4] == 't' &&
        (**token)[5] == 'e' &&
        (**token)[6] == '\0'
    ) {
        (*token) ++;
        for (int item = built.items - 1; item >= 0 ; item --) {
            if (strcmp(**token, built.object[item].key) == 0) {
                while (item - 1 < built.items) {
                    built.object[item] = built.object[item + 1];
                    item ++;
                }
                built.items --;
                built.object = realloc(built.object, sizeof(Type) * built.items);
                break;
            }
        }
    }

    else if (
        (**token)[0] == 'i' &&
        (**token)[1] == 'n' &&
        (**token)[2] == 'i' &&
        (**token)[3] == 't' &&
        (**token)[4] == '\0'
    ) {
        (*token) ++;
        built.items ++;
        built.object = realloc(built.object, sizeof(Type) * built.items);
        built.object[built.items - 1].key = (**token);
        built.object[built.items - 1].scope = scope;
        built.object[built.items - 1].type = 0;
        built.object[built.items - 1].object = NULL;
        built.object[built.items - 1].items = 0;
    }

    else if ((
        (**token)[0] == 'e' &&
        (**token)[1] == 'l' &&
        (**token)[2] == 's' &&
        (**token)[3] == 'e' &&
        (**token)[4] == '\0'
    ) || (
        (**token)[0] == 'd' &&
        (**token)[1] == 'e' &&
        (**token)[2] == 'f' &&
        (**token)[3] == 'a' &&
        (**token)[4] == 'u' &&
        (**token)[5] == 'l' &&
        (**token)[6] == 't' &&
        (**token)[7] == '\0'
    )) {
        (*token) ++;
        skip_process(token);
    }

    else if (
        (**token)[0] == 'i' &&
        (**token)[1] == 'f' &&
        (**token)[2] == '\0'
    ) {
        (*token) ++;
        Type condition = expression(token, defined);
        while ((**token)[0] == ',') {
            (*token) ++;
            condition = expression(token, defined);
        }
        if (condition.type == 3 && condition.bool == 1) res = process(token, defined);
        else {
            skip_process(token);
            if (
                (**token)[0] == 'e' &&
                (**token)[1] == 'l' &&
                (**token)[2] == 's' &&
                (**token)[3] == 'e' &&
                (**token)[4] == '\0'
            ) {
                (*token) ++;
                res = process(token, defined);
            }
        }
    }

    else if (
        (**token)[0] == 'd' &&
        (**token)[1] == 'o' &&
        (**token)[2] == '\0'
    ) {
        (*token) ++;
        char **stptr = &(**token);
        while (1) {
            res = process(token, defined);
            if (
                (**token)[0] == 'w' &&
                (**token)[1] == 'h' &&
                (**token)[2] == 'i' &&
                (**token)[3] == 'l' &&
                (**token)[4] == 'e' &&
                (**token)[5] == '\0'
            ) {
                (*token) ++;
                Type condition = expression(token, defined);
                while ((**token)[0] == ',') {
                    (*token) ++;
                    condition = expression(token, defined);
                }
                if (condition.type == 3 && condition.bool == 1 && res.type >= -3) (*token) = &(*stptr);
                else {
                    if (res.type == -4) res.type = 0;
                    break;
                }
            } else break;
            res.type = 0;
        }
    }

    else if (
        (**token)[0] == 'w' &&
        (**token)[1] == 'h' &&
        (**token)[2] == 'i' &&
        (**token)[3] == 'l' &&
        (**token)[4] == 'e' &&
        (**token)[5] == '\0'
    ) {
        (*token) ++;
        char **stptr = &(**token);
        while (1) {
            Type condition = expression(token, defined);
            while ((**token)[0] == ',') {
                (*token) ++;
                condition = expression(token, defined);
            }

            if (condition.type == 3 && condition.bool == 1) {
                res = process(token, defined);
                //printf("@ %d %s\n", res.type, **token);
                if (res.type >= -3) (*token) = &(*stptr);
                else {
                    if (res.type == -4) res.type = 0;
                    break;
                }
            }
            else {
                skip_process(token);
                res.type = 0;
                break;
            }
        }
    }

    else if (
        (**token)[0] == 'f' &&
        (**token)[1] == 'o' &&
        (**token)[2] == 'r' &&
        (**token)[3] == '\0'
    ) {
        (*token) ++;
        char *index = (**token);
        (*token) ++;
        if (
            (**token)[0] == 'i' &&
            (**token)[1] == 'n' &&
            (**token)[2] == '\0'
        ) {
            (*token) ++;
            Type in = expression(token, defined);
            built.items ++;
            built.object = realloc(built.object, sizeof(Type) * built.items);
            char **stptr = &(**token);

            if (in.type == 4) {
                int l = 0;
                while (1) {
                    if (in.length != l) {
                        built.object[built.items - 1].key = index;
                        built.object[built.items - 1].type = in.array[l].type;
                        built.object[built.items - 1].object = in.array[l].object;
                        built.object[built.items - 1].items = in.array[l].items;
                        switch (in.array[l].type) {
                            case 1: built.object[built.items - 1].string = in.array[l].string; break;
                            case 2: built.object[built.items - 1].number = in.array[l].number; break;
                            case 3: built.object[built.items - 1].bool = in.array[l].bool; break;
                            case 5:
                            built.object[built.items - 1].id = in.array[l].id;
                            built.object[built.items - 1].function = &(*in.array[l].function);
                            built.object[built.items - 1].param = in.array[l].param;
                            built.object[built.items - 1].prc = in.array[l].prc;
                            break;
                            case 4:
                            built.object[built.items - 1].array = in.array[l].array;
                            built.object[built.items - 1].length = in.array[l].length;
                            break;
                        }
                        res = process(token, defined);
                        if (res.type >= -3) (*token) = &(*stptr);
                        else {
                            if (res.type == -4) res.type = 0;
                            break;
                        }
                        l ++;
                    } else {
                        skip_process(token);
                        res.type = 0;
                        break;
                    }
                }
            }

            built.items --;
            built.object = realloc(built.object, sizeof(Type) * built.items);

        } else res = process(token, defined);
    }

    else if (
        (**token)[0] == 'c' &&
        (**token)[1] == 'a' &&
        (**token)[2] == 's' &&
        (**token)[3] == 'e' &&
        (**token)[4] == '\0'
    ) {
        (*token) ++;
        skip_expression(token);
        skip_process(token);
    }

    else if (
        (**token)[0] == 's' &&
        (**token)[1] == 'w' &&
        (**token)[2] == 'i' &&
        (**token)[3] == 't' &&
        (**token)[4] == 'c' &&
        (**token)[5] == 'h' &&
        (**token)[6] == '\0'
    ) {
        (*token) ++;
        Type model = expression(token, defined);
        while ((**token)[0] == ',') {
            (*token) ++;
            model = expression(token, defined);
        }

        while (
            (**token)[0] == 'c' &&
            (**token)[1] == 'a' &&
            (**token)[2] == 's' &&
            (**token)[3] == 'e' &&
            (**token)[4] == '\0'
        ) {
            (*token) ++;
            Type condition = expression(token, defined);
            while ((**token)[0] == ',') {
                (*token) ++;
                condition = expression(token, defined);
            }

            if (condition.type == model.type) {
                if (model.type == 1) {
                    if (strcmp(condition.string, model.string) == 0) {
                        res = process(token, defined);
                        break;
                    }
                }
                else if (model.type == 2) {
                    if (condition.number == model.number) {
                        res = process(token, defined);
                        break;
                    }
                }
                else if (model.type == 3) {
                    if (condition.bool == model.bool) {
                        res = process(token, defined);
                        break;
                    }
                }
                else {
                    res = process(token, defined);
                    break;
                }
            }

            skip_process(token);
            if (
                (**token)[0] == 'd' &&
                (**token)[1] == 'e' &&
                (**token)[2] == 'f' &&
                (**token)[3] == 'a' &&
                (**token)[4] == 'u' &&
                (**token)[5] == 'l' &&
                (**token)[6] == 't' &&
                (**token)[7] == '\0'
            ) {
                (*token) ++;
                res = process(token, defined);
            }
        }
    }

    else {
        res = expression(token, defined);
        while ((**token)[0] == ',') {
            (*token) ++;
            res = expression(token, defined);
        }
    }

    return res;
}









static inline void next(int len) {
    token[i] = realloc(token[i], sizeof(char) * len);
    i ++;
    token = realloc(token, sizeof(char *) * (i + 1));
    token[i] = malloc(sizeof(char) * BUFSIZ);
}

static inline int is_statment(void) {
    return (
        token[i - 1][0] == 'v' &&
        token[i - 1][1] == 'a' &&
        token[i - 1][2] == 'r' &&
        token[i - 1][3] == '\0'
    ) || (
        token[i - 1][0] == 'f' &&
        token[i - 1][1] == 'u' &&
        token[i - 1][2] == 'n' &&
        token[i - 1][3] == 'c' &&
        token[i - 1][4] == '\0'
    ) || (
        token[i - 1][0] == 'c' &&
        token[i - 1][1] == 'l' &&
        token[i - 1][2] == 'a' &&
        token[i - 1][3] == 's' &&
        token[i - 1][4] == 's' &&
        token[i - 1][5] == '\0'
    ) || (
        token[i - 1][0] == 'r' &&
        token[i - 1][1] == 'e' &&
        token[i - 1][2] == 't' &&
        token[i - 1][3] == 'u' &&
        token[i - 1][4] == 'r' &&
        token[i - 1][5] == 'n' &&
        token[i - 1][6] == '\0'
    ) || (
        token[i - 1][0] == 'b' &&
        token[i - 1][1] == 'r' &&
        token[i - 1][2] == 'e' &&
        token[i - 1][3] == 'a' &&
        token[i - 1][4] == 'k' &&
        token[i - 1][5] == '\0'
    ) || (
        token[i - 1][0] == 'i' &&
        token[i - 1][1] == 'f' &&
        token[i - 1][2] == '\0'
    ) || (
        token[i - 1][0] == 'e' &&
        token[i - 1][1] == 'l' &&
        token[i - 1][2] == 's' &&
        token[i - 1][3] == 'e' &&
        token[i - 1][4] == '\0'
    ) || (
        token[i - 1][0] == 'w' &&
        token[i - 1][1] == 'h' &&
        token[i - 1][2] == 'i' &&
        token[i - 1][3] == 'l' &&
        token[i - 1][4] == 'e' &&
        token[i - 1][5] == '\0'
    ) || (
        token[i - 1][0] == 'd' &&
        token[i - 1][1] == 'o' &&
        token[i - 1][2] == '\0'
    ) || (
        token[i - 1][0] == 's' &&
        token[i - 1][1] == 'w' &&
        token[i - 1][2] == 'i' &&
        token[i - 1][3] == 't' &&
        token[i - 1][4] == 'c' &&
        token[i - 1][5] == 'h' &&
        token[i - 1][6] == '\0'
    ) || (
        token[i - 1][0] == 'c' &&
        token[i - 1][1] == 'a' &&
        token[i - 1][2] == 's' &&
        token[i - 1][3] == 'e' &&
        token[i - 1][4] == '\0'
    ) || (
        token[i - 1][0] == 'd' &&
        token[i - 1][1] == 'e' &&
        token[i - 1][2] == 'f' &&
        token[i - 1][3] == 'a' &&
        token[i - 1][4] == 'u' &&
        token[i - 1][5] == 'l' &&
        token[i - 1][6] == 't' &&
        token[i - 1][7] == '\0'
    ) || (
        token[i - 1][0] == 'd' &&
        token[i - 1][1] == 'e' &&
        token[i - 1][2] == 'l' &&
        token[i - 1][3] == 'e' &&
        token[i - 1][4] == 't' &&
        token[i - 1][5] == 'e' &&
        token[i - 1][6] == '\0'
    ) || (
        token[i - 1][0] == 'f' &&
        token[i - 1][1] == 'o' &&
        token[i - 1][2] == 'r' &&
        token[i - 1][3] == '\0'
    ) || (
        token[i - 1][0] == 'i' &&
        token[i - 1][1] == 'n' &&
        token[i - 1][2] == '\0'
    );
}

static inline int need_id(void) {
    return (
        token[i - 1][0] == 'v' &&
        token[i - 1][1] == 'a' &&
        token[i - 1][2] == 'r' &&
        token[i - 1][3] == '\0'
    ) || (
        token[i - 1][0] == 'f' &&
        token[i - 1][1] == 'o' &&
        token[i - 1][2] == 'r' &&
        token[i - 1][3] == '\0'
    ) || (
        token[i - 1][0] == 'c' &&
        token[i - 1][1] == 'l' &&
        token[i - 1][2] == 'a' &&
        token[i - 1][3] == 's' &&
        token[i - 1][4] == 's' &&
        token[i - 1][5] == '\0'
    ) || (
        token[i - 1][0] == 'f' &&
        token[i - 1][1] == 'u' &&
        token[i - 1][2] == 'n' &&
        token[i - 1][3] == 'c' &&
        token[i - 1][4] == '\0'
    ) || (
        token[i - 1][0] == 'd' &&
        token[i - 1][1] == 'e' &&
        token[i - 1][2] == 'l' &&
        token[i - 1][3] == 'e' &&
        token[i - 1][4] == 't' &&
        token[i - 1][5] == 'e' &&
        token[i - 1][6] == '\0'
    ) || (
        token[i - 1][0] == '.'
    ) || (
        token[i - 1][0] == '-' &&
        token[i - 1][1] == '>'
    );
}

static inline int is_data(void) {
    return (
        token[i - 1][0] == 'v' &&
        token[i - 1][1] == 'o' &&
        token[i - 1][2] == 'i' &&
        token[i - 1][3] == 'd' &&
        token[i - 1][4] == '\0'
    ) || (
        token[i - 1][0] == 't' &&
        token[i - 1][1] == 'r' &&
        token[i - 1][2] == 'u' &&
        token[i - 1][3] == 'e' &&
        token[i - 1][4] == '\0'
    ) || (
        token[i - 1][0] == 'f' &&
        token[i - 1][1] == 'a' &&
        token[i - 1][2] == 'l' &&
        token[i - 1][3] == 's' &&
        token[i - 1][4] == 'e' &&
        token[i - 1][5] == '\0'
    ) || (
        token[i - 1][0] == 'u' &&
        token[i - 1][1] == 'n' &&
        token[i - 1][2] == 'd' &&
        token[i - 1][3] == 'e' &&
        token[i - 1][4] == 'f' &&
        token[i - 1][5] == 'i' &&
        token[i - 1][6] == 'n' &&
        token[i - 1][7] == 'e' &&
        token[i - 1][8] == 'd' &&
        token[i - 1][9] == '\0'
    ) || (
        token[i - 1][0] == 'e' &&
        token[i - 1][1] == 'r' &&
        token[i - 1][2] == 'r' &&
        token[i - 1][3] == 'o' &&
        token[i - 1][4] == 'r' &&
        token[i - 1][5] == '\0'
    );
}

static inline int is_operator(void) {
    return (
        token[i - 1][0] == '!' ||
        token[i - 1][0] == '%' ||
        token[i - 1][0] == '&' ||
        token[i - 1][0] == '=' ||
        token[i - 1][0] == '~' ||
        token[i - 1][0] == '^' ||
        token[i - 1][0] == '|' ||
        token[i - 1][0] == ':' ||
        token[i - 1][0] == '*' ||
        token[i - 1][0] == '<' ||
        token[i - 1][0] == '>' ||
        token[i - 1][0] == '.' ||
        token[i - 1][0] == ',' ||
        token[i - 1][0] == '/' ||
        token[i - 1][0] == '?' ||
        token[i - 1][1] == '\0' && (
            token[i - 1][0] == '+' ||
            token[i - 1][0] == '-'
        )
    );
}

int readfile(char* file) {

    FILE* fp = file != NULL ? fopen(file, "r") : stdin;

    
    token = realloc(token, sizeof(char *) * (i + 1));
    token[i] = malloc(sizeof(char) * BUFSIZ);

    char *error = NULL;
    int parentheses =  0, pline, pcolumn;
    int square_brackets = 0, sline, scolumn;
    int curly_brackets = 0, cline, ccolumn;

    int len = 0;
    int line = 1, column = 0;
    char c;

    while (1) {

        c = fgetc(fp);
        column ++;
        switch (c) {
            
            case '#':
                do c = fgetc(fp); while (c != '\n' && c != EOF);
            case '\n':
            case EOF:
                if (len != 0) {
                    next(len);
                    len = 0;
                }
                if (c == '\n') {
                    if (file == NULL) {
                        if (i != 0 && parentheses == 0 && square_brackets == 0 && curly_brackets == 0) {
                            if (!is_operator() && !is_statment()) return EXIT_SUCCESS;
                        }
                        printf("> ");
                    } 
                    line ++;
                    column = 0;
                }
                else {
                    if (file == NULL) {
                        free(token);
                        exit(EXIT_SUCCESS);
                    }
                    if (i != 0) {
                        if (parentheses != 0) {
                            error = "Unclosed '('";
                            line = pline;
                            column = pcolumn;
                            break;
                        } else if (square_brackets != 0) {
                            error = "Unclosed '['";
                            line = sline;
                            column = scolumn;
                            break;
                        } else if (curly_brackets != 0) {
                            error = "Unclosed '{'";
                            line = cline;
                            column = ccolumn;
                            break;
                        } else if (is_operator() || is_statment()) {
                            error = "Unexpected end";
                            break;
                        }
                    }
                    return EXIT_SUCCESS;
                }
            break;
            
            case ' ':
            case ';':
                if (len != 0) {
                    next(len);
                    len = 0;
                }
            break;

            case '"':
            case '\'':
                if (len != 0) {
                    next(len);
                    len = 0;
                }
                if (parentheses > curly_brackets || square_brackets > curly_brackets) {
                    if (
                        (token[i - 1][0] >= '0' && token[i - 1][0] <= '9') ||
                        (token[i - 1][0] >= 'A' && token[i - 1][0] <= 'Z') ||
                        (token[i - 1][0] >= 'a' && token[i - 1][0] <= 'z') || token[i - 1][0] == '_' ||
                        token[i - 1][0] == '"' || token[i - 1][0] == ']' || token[i - 1][0] == ')'
                    ) {
                        error = "Missing operator";
                        break;
                    }
                }
                if (i != 0) if (need_id()) {
                    error = "Invalid identifer";
                    break;
                }
                token[i][0] = '"';
                next(1);
                
                int strltrl = column;
                while (1) {
                    char s = fgetc(fp);
                    column ++;

                    if (s == '\n' || s == '\r' || s == EOF) {
                        error = "Unterminated string literal";
                        column = strltrl;
                        break;
                    }
                    if (s == c) {
                        next(len);
                        len = 1;
                        token[i][0] = '"';

                        if (i >= 3) {
                            if (
                                token[i - 3][0] == 'i' &&
                                token[i - 3][1] == 'm' &&
                                token[i - 3][2] == 'p' &&
                                token[i - 3][3] == 'o' &&
                                token[i - 3][4] == 'r' &&
                                token[i - 3][5] == 't' &&
                                token[i - 3][6] == '\0'
                            ) {
                                char path[256];
                                strcpy(path, file);
                                strcpy(strrchr(path, '/') + 1, token[i - 1]);

                                i -= 3;
                                len = 0;
                                token = realloc(token, sizeof(char *) * i);

                                char rlpath[256];
                                if (realpath(path, rlpath) != NULL) {
                                    if (readfile(rlpath)) return EXIT_FAILURE;
                                } else {
                                    token = realloc(token, sizeof(char *) * (i + 1));
                                    token[i] = malloc(sizeof(char) * BUFSIZ);
                                }
                            }
                        }
                        break;
                    }
                    if (s == '\\') {
                        s = fgetc(fp);
                        column ++;
                        switch (s) {
                            case '0': s = '\0'; break;
                            case 'a': s = '\a'; break;
                            case 'b': s = '\b'; break;
                            case 'e': s = '\e'; break;
                            case 'f': s = '\f'; break;
                            case 'n': s = '\n'; break;
                            case 'r': s = '\r'; break;
                            case 't': s = '\t'; break;
                            case 'v': s = '\v'; break;
                            case 'u': {
                                int unicode = 0;
                                for (int n = 4096; n > 0; n /= 16) {
                                    s = fgetc(fp);
                                    column ++;
                                    if (s >= '0' && s <= '9') unicode += (s - 48) * n;
                                    else if (s >= 'a' && s <= 'f') unicode += (s - 87) * n;
                                    else {
                                        error = "Invalid unicode";
                                        break;
                                    }
                                }
                                s = unicode;
                            }
                            break;
                        }
                    }
                    if (error != NULL) break;
                    token[i][len] = s;
                    len ++;
                }
            break;

            case 'A':
            case 'B':
            case 'C':
            case 'D':
            case 'E':
            case 'F':
            case 'G':
            case 'H':
            case 'I':
            case 'J':
            case 'K':
            case 'L':
            case 'M':
            case 'N':
            case 'O':
            case 'P':
            case 'Q':
            case 'R':
            case 'S':
            case 'T':
            case 'U':
            case 'V':
            case 'W':
            case 'X':
            case 'Y':
            case 'Z':
            case 'a':
            case 'b':
            case 'c':
            case 'd':
            case 'e':
            case 'f':
            case 'g':
            case 'h':
            case 'i':
            case 'j':
            case 'k':
            case 'l':
            case 'm':
            case 'n':
            case 'o':
            case 'p':
            case 'q':
            case 'r':
            case 's':
            case 't':
            case 'u':
            case 'v':
            case 'w':
            case 'x':
            case 'y':
            case 'z':
            case '_':
                if (len != 0) {
                    if (len != 1 && token[i][0] == '0' && (token[i][1] == 'b' || token[i][1] == 'B')) {
                        error = "Invalid binary literal";
                        break;
                    }
                    else if (len != 1 && token[i][0] == '0' && (token[i][1] == 'o' || token[i][1] == 'O')) {
                        error = "Invalid octal literal";
                        break;
                    }
                    else if (len != 1 && token[i][0] == '0' && (token[i][1] == 'x' || token[i][1] == 'X')) {
                        if (
                            c != 'a' && c != 'A' &&
                            c != 'b' && c != 'B' &&
                            c != 'c' && c != 'C' &&
                            c != 'd' && c != 'D' &&
                            c != 'e' && c != 'E' &&
                            c != 'f' && c != 'F'
                        ) {
                            error = "Invalid hexadecimal literal";
                            break;
                        }
                    }
                    else if (token[i][0] >= '0' && token[i][0] <= '9') {
                        if (c == 'e' || c == 'E') {
                            for (int e = 0; e < len; e++) {
                                if (token[i][e] == 'e' || token[i][e] == 'E') {
                                    error = "Invalid numeric literal";
                                    break;
                                }
                            }
                        }
                        else if (
                            len != 1 || token[i][0] != '0' || (
                                c != 'b' && c != 'B' &&
                                c != 'o' && c != 'O' &&
                                c != 'x' && c != 'X'
                            )
                        ) {
                            error = "Invalid numeric literal";
                            break;
                        }
                    }
                    else if (
                        (token[i][0] < 'A' || token[i][0] > 'Z') &&
                        (token[i][0] < 'a' || token[i][0] > 'z') && token[i][0] != '_'
                    ) {
                        next(len);
                        len = 0;
                    }
                }
                if (parentheses > curly_brackets || square_brackets > curly_brackets) {
                    if (
                        (token[i - 1][0] >= '0' && token[i - 1][0] <= '9') ||
                        (token[i - 1][0] >= 'A' && token[i - 1][0] <= 'Z') ||
                        (token[i - 1][0] >= 'a' && token[i - 1][0] <= 'z') || token[i - 1][0] == '_' ||
                        token[i - 1][0] == '"' || token[i - 1][0] == ']' || token[i - 1][0] == ')'
                    ) {
                        error = "Missing operator";
                        break;
                    }
                }
                token[i][len] = c;
                len ++;
            break;

            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                if (len != 0) {
                    if (token[i][0] == '0' && (token[i][1] == 'b' || token[i][1] == 'B')) {
                        if (c != '0' && c != '1') {
                            error = "Invalid binary literal";
                            break;
                        }
                    } else if (token[i][0] == '0' && (token[i][1] == 'o' || token[i][1] == 'O')) {
                        if (c == '9' || c == '8') {
                            error = "Invalid binary literal";
                            break;
                        }
                    }
                    if (
                        (token[i][0] < '0' || token[i][0] > '9') &&
                        (token[i][0] < 'A' || token[i][0] > 'Z') &&
                        (token[i][0] < 'a' || token[i][0] > 'z') && token[i][0] != '_'
                    ) {
                        next(len);
                        len = 0;
                    }
                }
                if (i != 0) if (need_id()) {
                    error = "Invalid identifer";
                    break;
                }
                if (parentheses > curly_brackets || square_brackets > curly_brackets) {
                    if (
                        (token[i - 1][0] >= '0' && token[i - 1][0] <= '9') ||
                        (token[i - 1][0] >= 'A' && token[i - 1][0] <= 'Z') ||
                        (token[i - 1][0] >= 'a' && token[i - 1][0] <= 'z') || token[i - 1][0] == '_' ||
                        token[i - 1][0] == '"' || token[i - 1][0] == ']' || token[i - 1][0] == ')'
                    ) {
                        error = "Missing operator";
                        break;
                    }
                }
                token[i][len] = c;
                len ++;
            break;

            case '.':
                if (len != 0) {
                    if (token[i][0] < '0' || token[i][0] > '9') {
                        next(len);
                        len = 0;
                    } else {
                        token[i][len] = c;
                        len ++;
                        break;
                    }
                }
                if (i == 0) {
                    error = "Missing left-hand side expression";
                    break;
                }
                if (parentheses == 0 && square_brackets == 0 && is_statment()) {
                    error = "Missing left-hand side expression";
                    break;
                }
                if (
                    is_operator() ||
                    token[i - 1][0] == '(' ||
                    token[i - 1][0] == '[' ||
                    token[i - 1][0] == '{' ||
                    token[i - 1][0] == '}'
                ) {
                    error = "Invalid left-hand side expression";
                    break;
                }
                token[i][len] = c;
                len ++;
            break;

            case '!':
            case '~':
                if (len != 0) {
                    next(len);
                    len = 0;
                }
                if (i != 0) {
                    if (token[i - 1][0] == '.') {
                        error = "Invalid identifer";
                        break;
                    }
                }
                token[i][len] = c;
                len ++;
            break;

            case ')':
                parentheses -= (square_brackets + curly_brackets + 1);
                if (parentheses < 0) {
                    error = "Unmatched ')'";
                    break;
                }
                if (len != 0) {
                    next(len);
                    len = 0;
                }
                if (is_operator()) {
                    error = "Invalid token ')' in expression";
                    break;
                }
                token[i][len] = c;
                len ++;
            break;

            case '(':
                if (len != 0) {
                    next(len);
                    len = 0;
                }
                if (i != 0) if (need_id()) {
                    error = "Invalid token '(' in expression";
                    break;
                }
                if (parentheses != 0 || square_brackets != 0) {
                    if (
                        (token[i - 1][0] >= '0' && token[i - 1][0] <= '9') ||
                        token[i - 1][0] == '"' || token[i - 1][0] == ']' || token[i - 1][0] == ')'
                    ) {
                        error = "Missing operator";
                        break;
                    }
                }
                if (parentheses == 0) {
                    pline = line;
                    pcolumn = column;
                }
                parentheses += (square_brackets + curly_brackets + 1);
                token[i][len] = c;
                len ++;
            break;

            case ']':
                if (len != 0) {
                    next(len);
                    len = 0;
                }
                if (is_operator()) {
                    error = "Invalid token ']' in expression";
                    break;
                }
                square_brackets -= (parentheses + curly_brackets + 1);
                if (square_brackets < 0) {
                    error = "Unmatched ']'";
                    break;
                }
                token[i][len] = c;
                len ++;
            break;

            case '[':
                if (len != 0) {
                    next(len);
                    len = 0;
                }
                if (i != 0) if (need_id()) {
                    error = "Invalid token '(' in expression";
                    break;
                }
                if (square_brackets == 0) {
                    sline = line;
                    scolumn = column;
                }
                square_brackets += (parentheses + curly_brackets + 1);
                token[i][len] = c;
                len ++;
            break;

            case '}':
                if (len != 0) {
                    next(len);
                    len = 0;
                }
                if (is_operator()) {
                    error = "Invalid token '}' in expression";
                    break;
                }
                curly_brackets -= (parentheses + square_brackets + 1);
                if (curly_brackets < 0) {
                    error = "Unmatched '}'";
                    break;
                }
                token[i][len] = c;
                len ++;
            break;

            case '{':
                if (len != 0) {
                    next(len);
                    len = 0;
                }
                if (i != 0) {
                    if ((token[i - 1][0] != '=' || token[i - 1][1] != '>') && is_operator() || need_id()) {
                        error = "Invalid token '{' in expression";
                        break;
                    }
                }
                if (curly_brackets == 0) {
                    cline = line;
                    ccolumn = column;
                }
                curly_brackets += (parentheses + square_brackets + 1);
                token[i][len] = c;
                len ++;
            break;

            case '>':
                if (len != 0) {
                    if ((token[i][0] != c && token[i][0] != '=') || len != 1) {
                        next(len);
                        len = 0;
                    }
                }
                if (i == 0) {
                    error = "Missing left-hand side expression";
                    break;
                }
                if (token[i][0] == '=' && token[i - 1][0] != ')') {
                    error = "Invalid left-hand side expression in function";
                    column--;
                    break;
                }
                if (
                    (parentheses == 0 && square_brackets == 0 && is_statment()) ||
                    is_operator() ||
                    token[i - 1][0] == '(' ||
                    token[i - 1][0] == '[' ||
                    token[i - 1][0] == '{' ||
                    token[i - 1][0] == '}'
                ) {
                    error = "Invalid left-hand side expression";
                    break;
                }
                token[i][len] = c;
                len ++;
            break;

            case '*':
            case '<':
            case '&':
            case '|':
                if (len != 0) {
                    if (token[i][0] != c || len != 1) {
                        next(len);
                        len = 0;
                    }
                }
                if (i == 0) {
                    error = "Missing left-hand side expression";
                    break;
                }
                if (
                    (parentheses == 0 && square_brackets == 0 && is_statment()) ||
                    is_operator() ||
                    token[i - 1][0] == '(' ||
                    token[i - 1][0] == '[' ||
                    token[i - 1][0] == '{' ||
                    token[i - 1][0] == '}'
                ) {
                    error = "Invalid left-hand side expression";
                    break;
                }
                token[i][len] = c;
                len ++;
            break;

            case '/':
            case '%':
            case '^':
            case ',':
            case '?':
            case ':':
                if (len != 0) {
                    next(len);
                    len = 0;
                }
                if (i == 0) {
                    error = "Missing left-hand side expression";
                    break;
                }
                if (
                    (parentheses == 0 && square_brackets == 0 && is_statment()) ||
                    token[i - 1][0] == '(' ||
                    token[i - 1][0] == '[' ||
                    token[i - 1][0] == '{' ||
                    token[i - 1][0] == '}' ||
                    is_operator()
                ) {
                    error = "Invalid left-hand side expression";
                    break;
                }
                token[i][len] = c;
                len ++;
            break;

            case '+':
            case '-':
                if (len != 0) {
                    if (token[i][0] == c && len == 1) {
                        if (i == 0) {
                            error = "Missing left-hand side expression";
                            break;
                        }
                        if (
                            (parentheses == 0 && square_brackets == 0 && is_statment()) ||
                            token[i - 1][0] == '(' ||
                            token[i - 1][0] == '[' ||
                            token[i - 1][0] == '{' ||
                            token[i - 1][0] == '}' ||
                            (token[i - 1][0] == '+' && token[i - 1][0] == '+') ||
                            (token[i - 1][0] == '-' && token[i - 1][0] == '-') ||
                            is_operator()
                        ) {
                            error = "Invalid left-hand side expression";
                            break;
                        }
                    } else if (token[i][0] < '0' || token[i][0] > '9' || (token[i][len - 1] != 'e' && token[i][len - 1] != 'E')) {
                        next(len);
                        len = 0;
                    }
                }
                if (i != 0) if (need_id()) {
                    error = "Invalid token '(' in expression";
                    break;
                }
                token[i][len] = c;
                len ++;
            break;

            case '=':
                if (len != 0) {
                    if (
                        (len != 1 || (
                            token[i][0] != '+' &&
                            token[i][0] != '-' &&
                            token[i][0] != '*' &&
                            token[i][0] != '/' &&
                            token[i][0] != '%' &&
                            token[i][0] != '&' &&
                            token[i][0] != '|' &&
                            token[i][0] != '^' &&
                            token[i][0] != '=' &&
                            token[i][0] != '!' &&
                            token[i][0] != '<' &&
                            token[i][0] != '>'
                        )) &&
                        (len != 2 || (
                            token[i][0] != '*' &&
                            token[i][0] != '&' &&
                            token[i][0] != '|' &&
                            token[i][0] != '<' &&
                            token[i][0] != '>'
                        ))
                    ) {
                        next(len);
                        len = 0;
                    }
                }
                if (i == 0) {
                    error = "Missing left-hand side expression";
                    break;
                }
                if (
                    (parentheses == 0 && square_brackets == 0 && is_statment()) ||
                    token[i - 1][0] == '(' ||
                    token[i - 1][0] == '[' ||
                    token[i - 1][0] == '{' ||
                    token[i - 1][0] == '}' ||
                    (token[i - 1][0] == '+' && token[i - 1][0] == '+') ||
                    (token[i - 1][0] == '-' && token[i - 1][0] == '-') ||
                    is_operator()
                ) {
                    error = "Invalid left-hand side expression";
                    break;
                }
                token[i][len] = c;
                len ++;
            break;

            case '\e':
                printf("\b\b\b\b    \b\b\b\b\b");
            break;

            default: if (c != '\r' && c != '\t') error = "Invalid character";
        }

        if (error != NULL) {
            if (file != NULL) {
                printf("\033[1m%s\033[0m line %d column %d\n", file, line, column);

                fseek(fp, 0L, SEEK_SET);
                char buffer[BUFSIZ];
                while (line > 0) {
                    fgets(buffer, BUFSIZ, fp);
                    line --;
                }

                if (buffer[strlen(buffer) - 1] == '\n') printf("> %s", buffer);
                else printf("> %s\n", buffer);

                while (column > -1) {
                    printf(" ");
                    column --;
                }
                printf("\033[92m^\033[0m\n\nSyntaxError: %s\n", error);
                fclose(fp);
            } else {
                while (column > -1) {
                    printf(" ");
                    column --;
                }
                printf("\033[92m^\033[0m\n\nSyntaxError: %s\n", error);
            }
            return EXIT_FAILURE;
        }
    }
}


int main(int argc, char **argv) {

    built.items = 7;
    built.object = malloc(sizeof(Type) * built.items);

    built.object[0].key = "print";
    built.object[0].type = 5;
    built.object[0].id = 1;
    built.object[6].key = "str";
    built.object[6].type = 5;
    built.object[6].id = 2;
    built.object[2].key = "scan";
    built.object[2].type = 5;
    built.object[2].id = 3;
    built.object[5].key = "tod";
    built.object[5].type = 5;
    built.object[5].id = 4;

    built.object[1].key = "string";
    built.object[1].type = 1;
    built.object[1].string = malloc(sizeof(char));
    built.object[1].string[0] = '\0';
    built.object[1].items = 8;
    built.object[1].object = malloc(sizeof(Type) * built.object[1].items);
    built.object[1].object[0].key = "length";
    built.object[1].object[0].type = 5;
    built.object[1].object[0].id = 101;
    built.object[1].object[1].key = "find";
    built.object[1].object[1].type = 5;
    built.object[1].object[1].id = 102;
    built.object[1].object[2].key = "replace";
    built.object[1].object[2].type = 5;
    built.object[1].object[2].id = 103;
    built.object[1].object[3].key = "replace_all";
    built.object[1].object[3].type = 5;
    built.object[1].object[3].id = 104;
    built.object[1].object[4].key = "split";
    built.object[1].object[4].type = 5;
    built.object[1].object[4].id = 105;
    built.object[1].object[5].key = "starts_with";
    built.object[1].object[5].type = 5;
    built.object[1].object[5].id = 106;
    built.object[1].object[6].key = "ends_with";
    built.object[1].object[6].type = 5;
    built.object[1].object[6].id = 107;
    built.object[1].object[7].key = "reverse";
    built.object[1].object[7].type = 5;
    built.object[1].object[7].id = 108;

    built.object[3].key = "file";
    built.object[3].type = 0;
    built.object[3].items = 3;
    built.object[3].object = malloc(sizeof(Type) * built.object[3].items);
    built.object[3].object[0].key = "read";
    built.object[3].object[0].type = 5;
    built.object[3].object[0].id = 601;
    built.object[3].object[1].key = "write";
    built.object[3].object[1].type = 5;
    built.object[3].object[1].id = 602;
    built.object[3].object[2].key = "exist";
    built.object[3].object[2].type = 5;
    built.object[3].object[2].id = 603;

    built.object[4].key = "math";
    built.object[4].type = 0;
    built.object[4].items = 3;
    built.object[4].object = malloc(sizeof(Type) * built.object[4].items);
    built.object[4].object[0].key = "PI";
    built.object[4].object[0].type = 2;
    built.object[4].object[0].number = M_PI;
    built.object[4].object[1].key = "E";
    built.object[4].object[1].type = 2;
    built.object[4].object[1].number = M_E;
    built.object[4].object[2].key = "sin";
    built.object[4].object[2].type = 5;
    built.object[4].object[2].id = 701;

    Type *defined = &built;

    if (argc > 1) {
        char path[256];
        if (realpath(argv[1], path) == NULL) {
            printf("tole: '%s' not found\n", argv[1]);
            return EXIT_FAILURE;
        }

        if (readfile(path)) {
            free(token);
            return EXIT_FAILURE;
        }
        token[i] = malloc(sizeof(char) * 4);
        token[i] = "exit";
        token = realloc(token, sizeof(char *) * (i + 2));


        Type res;
        do res = process(&token, defined); while (res.type != -6);

        // do {
        //     printf("%s\033[90m|\033[0m", *token);
        //     token ++;
        // } while (
        //     (
        //         (*(token - 1))[0] != 'e' ||
        //         (*(token - 1))[1] != 'x' ||
        //         (*(token - 1))[2] != 'i' ||
        //         (*(token - 1))[3] != 't' ||
        //         (*(token - 1))[4] != '\0'
        //     )
        // );
        // printf("\n");

    } else {

        puts("Tole 1.4.21");
        // struct termios term;
        // tcgetattr(0, &term);
        // term.c_lflag &= ~ICANON;
        // tcsetattr(0, 0, &term);
  

        do {
            printf("> ");
            token = malloc(0);
            i = 0;
            if (readfile(NULL)) {
                free(token);
                return EXIT_FAILURE;
            }
            token[i] = malloc(sizeof(char) * 4);
            token[i] = "exit";
            token = realloc(token, sizeof(char *) * (i + 2));

            Type value;
            Type res;
            do {
                value = res;
                res = process(&token, defined);
            } while (res.type != -6);

            switch (value.type) {
                case -2: printf("\e[90merror\e[0m\n"); break;
                case -1: printf("\e[90mundefined\e[0m\n"); break;
                case 1: printf("\e[90m\"%s\"\e[0m\n", value.string);break;
                case 2: printf("\e[90m%.15g\e[0m\n", value.number);break;
                case 3: printf("\e[90m%s\e[0m\n", value.bool ? "true" : "false");break;
            }

        } while ((*token) == NULL);

    }
    
    return EXIT_SUCCESS;
}
