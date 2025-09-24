#include "func.h"

#define MAX_MATERIALS 128


int parse_mtl_file(const char* path, MTLMaterial* materials, int max_materials) {
    FILE* file = fopen(path, "r");
    if (!file) {
        fprintf(stderr, "Failed to open MTL file: %s\n", path);
        return 0;
    }

    char line[512];
    int count = 0;
    MTLMaterial* current = NULL;

    while (fgets(line, sizeof(line), file)) {
        char* ptr = line;
        while (isspace(*ptr)) ptr++;

        if (strncmp(ptr, "newmtl ", 7) == 0) {
            if (count >= max_materials) break;
            current = &materials[count++];
            memset(current, 0, sizeof(MTLMaterial));
            sscanf(ptr + 7, "%63s", current->name);
        } else if (current && strncmp(ptr, "map_Kd ", 7) == 0) {
            sscanf(ptr + 7, "%255s", current->map_Kd);
        } else if (current && strncmp(ptr, "map_Bump ", 9) == 0) {
            sscanf(ptr + 9, "%255s", current->map_Bump);
        } else if (current && strncmp(ptr, "map_Ks ", 7) == 0) {
            sscanf(ptr + 7, "%255s", current->map_Ks);
        }
    }

    fclose(file);
    return count;  // 返回材质数量
}


cJSON* getJson(const char* path) {
    FILE* file = fopen(path, "r");
    if (!file) {
        perror("无法打开文件");
        return NULL;
    }

    // 获取文件大小
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // 分配内存
    char* buffer = (char*)malloc(size + 1);
    if (!buffer) {
        fclose(file);
        return NULL;
    }

    // 读取文件内容
    const size_t read_size = fread(buffer, 1, size, file);
    buffer[read_size] = '\0';
    fclose(file);

    // 解析 JSON
    cJSON* json = cJSON_Parse(buffer);
    free(buffer);

    return json;
}
bool cJSON_ExistKey(const cJSON* object, const char* key) {
    if (object == NULL || key == NULL) return false; // Req Condition
    const cJSON* val = cJSON_GetObjectItem(object, key);
    if (val == NULL) return false; // Req Condition
    return true;
}
bool cJSON_Load(const cJSON* object, const char* key, const JSM_DataType type, void* target) {
    // 这个函数的任务是: object[key]暴露给target, 不涉及alloc
    if (object == NULL || key == NULL || target == NULL) return false; // Req Condition
    const cJSON* val = cJSON_GetObjectItem(object, key);
    if (val == NULL) return false; // Req Condition
    switch (type) {
        case JSM_INT: {
            if (cJSON_IsNumber(val)) {
                *(int*)target = val->valueint;
                return true;
            }
            return false;
        }
        case JSM_FLOAT: {
            if (cJSON_IsNumber(val)) {
                *(float*)target = (float)val->valuedouble;
                return true;
            }
            return false;
        }
        case JSM_BOOL: {
            if (cJSON_IsBool(val)) {
                *(bool*)target = (bool)val->valueint;
                return true;
            }
            return false;
        }
        case JSM_FRECT: {
            if (cJSON_IsArray(val) == false || cJSON_GetArraySize(val) != 4) {
                return false;
            }
            const cJSON* rect_json[4];
            for (int i = 0; i < 4; i++) {
                rect_json[i] = cJSON_GetArrayItem(val, i);
                if (rect_json[i] == NULL || cJSON_IsNumber(rect_json[i]) == false) {
                    return false;
                }
            }
            const SDL_FRect rect = {
                (float)rect_json[0]->valuedouble,
                (float)rect_json[1]->valuedouble,
                (float)rect_json[2]->valuedouble,
                (float)rect_json[3]->valuedouble,
            };
            *(SDL_FRect*)target = rect;
            return true;
        }
        case JSM_RECT: {
            if (cJSON_IsArray(val) == false || cJSON_GetArraySize(val) != 4) {
                return false;
            }
            const cJSON* rect_json[4];
            for (int i = 0; i < 4; i++) {
                rect_json[i] = cJSON_GetArrayItem(val, i);
                if (rect_json[i] == NULL || cJSON_IsNumber(rect_json[i]) == false) {
                    return false;
                }
            }
            const SDL_Rect rect = {
                rect_json[0]->valueint,
                rect_json[1]->valueint,
                rect_json[2]->valueint,
                rect_json[3]->valueint,
            };
            *(SDL_Rect*)target = rect;
            return true;
        }
        case JSM_STRING: {
            if (cJSON_IsString(val) == false) {
                return false;
            }
            *(char**)target = val->valuestring;
            return true;
        }
        case JSM_COLOR:  {
            if (cJSON_IsArray(val) == false || cJSON_GetArraySize(val) != 4) {
                return false;
            }
            const cJSON* color_json[4];
            for (int i = 0; i < 4; i++) {
                color_json[i] = cJSON_GetArrayItem(val, i);
                if (color_json[i] == NULL || cJSON_IsNumber(color_json[i]) == false) {
                    return false;
                }
            }
            const SDL_Color color = {
                (int)color_json[0]->valuedouble,
                (int)color_json[1]->valuedouble,
                (int)color_json[2]->valuedouble,
                (int)color_json[3]->valuedouble,
            };
            *(SDL_Color*)target = color;
            return true;
        }
        default: return false;
    }
}


float clip(const float min, const float value, const float max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}
float loop(const float min, const float value, const float max) {
    const float period = max - min;
    float r = fmodf(value - min, period);  // 先移到以 a 为起点的坐标系
    if (r < 0) r += period;             // 保证落在 [0, period)
    return min + r;                       // 再移回原坐标系
}
void** allocate2DArray(size_t w, size_t h, size_t elementSize) {
    void** array = (void**)malloc(w * sizeof(void*));
    if (array == NULL) {
        return NULL;
    }

    for (size_t i = 0; i < w; i++) {
        array[i] = malloc(h * elementSize);
        if (array[i] == NULL) {
            // 分配失败，释放之前已分配的内存
            for (size_t j = 0; j < i; j++) {
                free(array[j]);
            }
            free(array);
            return NULL;
        }
    }

    return array;
}
void free2DArray(void** array, size_t w) {
    if (array == NULL) return;

    for (size_t i = 0; i < w; i++) {
        free(array[i]);
    }
    free(array);
}
