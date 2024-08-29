#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 256
#define MAX_KEY_LENGTH 256
#define MAX_VALUE_LENGTH 256
#define INITIAL_CAPACITY 10

typedef struct {
    const char *key;
    const char *value;
} WLSN_MAP_OBJ;

typedef struct {
    WLSN_MAP_OBJ *data;
    size_t size;
    size_t capacity;
} WLSN_CNFG;

void WLSN_Trim_Whitespace(char *str);

void WLSN_Add_Pair(WLSN_CNFG *config, const char *key, const char *value);

void WLSN_Parse_Input(const char *input, WLSN_CNFG *config, int is_font_file);

const char* WLSN_Get_Value(WLSN_CNFG config, const char *key);
void WLSN_Set_Value(WLSN_CNFG *config, const char *key, const char* value);

void WLSN_Print_Config(WLSN_CNFG config);

void WLSN_Release(WLSN_CNFG config);

WLSN_CNFG WLSN_Load_From_File(const char *filename, int is_font_file) ;

WLSN_CNFG WLSN_Load_From_String(const char *config_str, int is_font_file);

void WLSN_Dump(WLSN_CNFG config,const char *path);

#ifdef WILSON_IMP
#undef WILSON_IMP
void WLSN_Trim_Whitespace(char *str) {
    char *end;

    // Trim leading space
    while (isspace((unsigned char)*str)) str++;

    // Trim trailing space
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;

    // Null-terminate
    *(end + 1) = '\0';
}

void WLSN_Add_Pair(WLSN_CNFG *config, const char *key, const char *value) {
    if (config->size >= config->capacity) {
        config->capacity *= 2;
        config->data = realloc(config->data, config->capacity * sizeof(WLSN_MAP_OBJ));
        if (config->data == NULL) {
            perror("Failed to reallocate memory");
            exit(EXIT_FAILURE);
        }
    }
    config->data[config->size].key = strdup(key);
    config->data[config->size].value = strdup(value);
    if (config->data[config->size].key == NULL || config->data[config->size].value == NULL) {
        perror("Failed to duplicate string");
        exit(EXIT_FAILURE);
    }
    config->size++;
}

void WLSN_Parse_Input(const char *input, WLSN_CNFG *config, int is_font_file) {
    char *input_copy = strdup(input);
    if (input_copy == NULL) {
        perror("Failed to duplicate input string");
        exit(EXIT_FAILURE);
    }
    char *line = strtok(input_copy, "\n");

    while (line != NULL) {
        char key[MAX_KEY_LENGTH] = {0};
        char value[MAX_VALUE_LENGTH] = {0};

        // For font files, parse key-value pairs in format "key = x,y"
        if (is_font_file) {
            int x, y;
            if (sscanf(line, " %255[^=]=%d,%d", key, &x, &y) == 3) {
                snprintf(value, MAX_VALUE_LENGTH, "%d,%d", x, y);
                WLSN_Trim_Whitespace(key);
                WLSN_Add_Pair(config, key, value);
            } else {
                fprintf(stderr, "Warning: Font file line format error: %s\n", line);
            }
        } else {
            // For normal config files, handle quoted keys and values
            char *key_start = strchr(line, '"');
            char *key_end = key_start ? strchr(key_start + 1, '"') : NULL;
            char *value_start = key_end ? strchr(key_end + 1, '"') : NULL;
            char *value_end = value_start ? strchr(value_start + 1, '"') : NULL;

            if (key_start && key_end && value_start && value_end) {
                *key_end = '\0';
                *value_end = '\0';
                strncpy(key, key_start + 1, sizeof(key) - 1);
                strncpy(value, value_start + 1, sizeof(value) - 1);
                WLSN_Trim_Whitespace(key);
                WLSN_Trim_Whitespace(value);
                WLSN_Add_Pair(config, key, value);
            } else {
                // Handle non-quoted values or other formats
                char *equals_sign = strchr(line, '=');
                if (equals_sign) {
                    *equals_sign = '\0';
                    strncpy(key, line, sizeof(key) - 1);
                    strncpy(value, equals_sign + 1, sizeof(value) - 1);
                    WLSN_Trim_Whitespace(key);
                    WLSN_Trim_Whitespace(value);

                    // Remove potential quotes around key and value
                    if (key[0] == '"') {
                        memmove(key, key + 1, strlen(key));
                    }
                    char *end_key = strchr(key, '"');
                    if (end_key) *end_key = '\0';

                    if (value[0] == '"') {
                        memmove(value, value + 1, strlen(value));
                    }
                    char *end_value = strchr(value, '"');
                    if (end_value) *end_value = '\0';

                    // Handle cases where the key or value might be empty
                    if (strlen(key) == 0 || strlen(value) == 0) {
                        fprintf(stderr, "Warning: Empty key or value in line: %s\n", line);
                        line = strtok(NULL, "\n");
                        continue;
                    }

                    WLSN_Add_Pair(config, key, value);
                } else {
                    fprintf(stderr, "Warning: Line format error: %s\n", line);
                }
            }
        }

        line = strtok(NULL, "\n");
    }

    free(input_copy);
}

const char* WLSN_Get_Value(WLSN_CNFG config, const char *key) {
    for (size_t i = 0; i < config.size; i++) {
        if (strcmp(config.data[i].key, key) == 0) {
            return config.data[i].value;
        }
    }
    return NULL;
}

void WLSN_Set_Value(WLSN_CNFG *config, const char *key, const char* value) {
    for (size_t i = 0; i < config->size; i++) {
        if (strcmp(config->data[i].key, key) == 0) {
            config->data[i].value = strdup(value);
            return;
        }
    }

    if (config->size >= config->capacity) {
        config->capacity *= 2;
        config->data = realloc(config->data, config->capacity * sizeof(WLSN_MAP_OBJ));
        if (config->data == NULL) {
            perror("WLSN : Set Value : Failed to reallocate memory");
            exit(EXIT_FAILURE);
        }
    }

    config->data[config->size].key = strdup(key);
    config->data[config->size].value = strdup(value);
    config->size++;
}

void WLSN_Print_Config(WLSN_CNFG config) {
    for (size_t i = 0; i < config.size; i++) {
        printf("Key: %s, Value: %s\n", config.data[i].key, config.data[i].value);
    }
}

void WLSN_Release(WLSN_CNFG config) {
    for (size_t i = 0; i < config.size; i++) {
        free((void*)config.data[i].key);
        free((void*)config.data[i].value);
    }
    free(config.data);
}

WLSN_CNFG WLSN_Load_From_File(const char *filename, int is_font_file) {
    WLSN_CNFG config;
    config.size = 0;
    config.capacity = INITIAL_CAPACITY;
    config.data = malloc(config.capacity * sizeof(WLSN_MAP_OBJ));
    if (config.data == NULL) {
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }

    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("WLSN : LOAD : Failed to open file");
        exit(EXIT_FAILURE);
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file)) {
        char key[MAX_KEY_LENGTH];
        char value[MAX_VALUE_LENGTH];
        int x, y;

        if (is_font_file) {
            // For font files: expect format "key = x,y"
            if (sscanf(line, "%s = %d,%d", key, &x, &y) == 3) {
                snprintf(value, MAX_VALUE_LENGTH, "%d,%d", x, y);
            } else {
                fprintf(stderr, "Warning: Font file line format error: %s\n", line);
                continue;  // Skip invalid lines
            }
        } else {
            // For normal config files: expect format "key = value"
            if (sscanf(line, "%255[^=]=%255[^\n]", key, value) == 2) {
                WLSN_Trim_Whitespace(key);
                WLSN_Trim_Whitespace(value);
            } else {
                fprintf(stderr, "Warning: Config file line format error: %s\n", line);
                continue;  // Skip invalid lines
            }
        }

        WLSN_Add_Pair(&config, key, value);
    }

    fclose(file);
    return config;
}

WLSN_CNFG WLSN_Load_From_String(const char *config_str, int is_font_file) {
    WLSN_CNFG config;
    config.size = 0;
    config.capacity = INITIAL_CAPACITY;
    config.data = malloc(config.capacity * sizeof(WLSN_MAP_OBJ));
    if (config.data == NULL) {
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }

    WLSN_Parse_Input(config_str, &config, is_font_file);
    return config;
}

void WLSN_Dump(WLSN_CNFG config, const char *path) {
    FILE *file = fopen(path, "w");
    if (file == NULL) {
        printf("WLSN : DUMP : Error opening file : %s", path);
        return;
    }

    for (size_t i = 0; i < config.size; i++) {
        fprintf(file,"%s=%s\n", config.data[i].key, config.data[i].value);
    }

    fclose(file);
}
#endif