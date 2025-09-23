#include "func.h"

#define MAX_MATERIALS 128


bool SLD_GetPointInRect(const SDL_FPoint point, const SDL_FRect rect) {
    return rect.x <= point.x && point.x < rect.x + rect.w && rect.y <= point.y && point.y < rect.y + rect.h;
}
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


bool BASIC_Renew() {
    SDL_GetWindowSize(window, &logical_w, &logical_h);
    SDL_GetWindowSizeInPixels(window, &windowWidth, &windowHeight);
    scale_x = (float)windowWidth / (float)logical_w;
    scale_y = (float)windowHeight / (float)logical_h;
    windowRect = (SDL_FRect){0, 0, (float)windowWidth * scale_x, (float)windowHeight * scale_y};
    return true;
}
bool BASIC_Draw() {
    return true;
}
SDL_Texture* TXT_LoadTexture(SDL_Renderer* renderer, TTF_Font* font, const char* text, const SDL_Color color) {
    // N-Condition
    if (renderer == NULL) {printf("%s: renderer[%p] not exists.\n", __func__, renderer); return NULL;}
    if (font == NULL) {printf("%s: font[%p] not exists.\n", __func__, font); return NULL;}
    if (text == NULL) {printf("%s: text[%p] not exists.\n", __func__, text); return NULL;}
    if (strlen(text) == 0) {return NULL;} // Opt

    // getSurface N-Condition
    SDL_Surface* surface = TTF_RenderText_Blended(font, text, 0, color);
    if (surface == NULL) {printf("%s: fail to create surface from \"%s\".\n", __func__, text); return NULL;}

    // getTexture N-Condition
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_DestroySurface(surface);
    if (texture == NULL) {printf("%s: fail to create texture from \"%s\".\n", __func__, text); return NULL;}

    return texture;
}
SDL_Texture* TXT_LoadTextureWithLines(
    SDL_Renderer* renderer,
    TTF_Font* font,
    const char* scr_text,
    const SDL_Color colorText,
    const SDL_Color colorBack,
    const char aligned
    ) {

    if (strlen(scr_text) == 0) return NULL; // Opt Condition
    int size_text = (int)strlen(scr_text) + 1;
    char text[size_text];
    strcpy(text, scr_text);

    int num_lines = 1;
    for (int i = 0; text[i] != '\0'; i++) {
        if (text[i] == '\n') {
            num_lines++;
        }
    }

    int line_offsets[num_lines];
    line_offsets[0] = 0;
    for (int i = 0, j = 1; text[i] != '\0'; i++) {
        // i: index of char, j: index of line
        if (text[i] == '\n') {
            line_offsets[j] = i + 1;
            text[i] = '\0';
            j++;
        }
    }

    SDL_Texture* sub_textures[num_lines];
    for (int i = 0; i < num_lines; i++) {
        const char* sub_text = text + line_offsets[i];
        if (strlen(sub_text) == 0) continue;
        sub_textures[i] = TXT_LoadTexture(renderer, font, sub_text, colorText);  // alloc
        if (sub_textures[i] == NULL) {
            printf("%s: fail to create texture from \"%s\".\n", __func__, sub_text);
        }
    }

    // getMainTextureSize
    const float line_height = (float)TTF_GetFontHeight(font);
    const int main_texture_height = num_lines * TTF_GetFontHeight(font);
    float line_widths[num_lines];
    int main_texture_width = 0;
    for (int i = 0; i < num_lines; i++) {
        SDL_GetTextureSize(sub_textures[i], &line_widths[i], NULL);
        if ((float)main_texture_width < line_widths[i]) {main_texture_width = (int)line_widths[i];}
    }

    // getTexture N-Condition
    SDL_Texture* texture = SDL_CreateTexture(
        renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
        main_texture_width, main_texture_height
        );
    if (texture == NULL) {printf("%s: fail to create texture from \"%s\".\n", __func__, text); return NULL;}
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND_PREMULTIPLIED);

    // clearTexture
    SDL_SetRenderTarget(renderer, texture);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    // updateTexture
    SDL_SetRenderColor(renderer, colorBack);
    for (int i = 0; i < num_lines; i++) {
        SDL_FRect dst_rect = {0, line_height * (float)i, line_widths[i], line_height};
        switch (aligned) {
            case 'r': case 'R': {dst_rect.x = (float)main_texture_width - dst_rect.w; break;}
            case 'c': case 'C': {dst_rect.x = ((float)main_texture_width - dst_rect.w) / 2; break;}
            case 'l': case 'L': default: {break;}
        }
        SDL_RenderFillRect(renderer, &dst_rect);
        SDL_RenderTexture(renderer, sub_textures[i], NULL, &dst_rect);
    }

    // end
    for (int i = 0; i < num_lines; i++) {SDL_DestroyTexture(sub_textures[i]);}
    SDL_SetRenderTarget(renderer, NULL);

    return texture;
}
SDL_FColor SDL_GetFColorFromColor(const SDL_Color color) {
    return (SDL_FColor){(float)color.r / 255, (float)color.g / 255, (float)color.b / 255, (float)color.a / 255};
}
bool SDL_SetRenderColor(SDL_Renderer* renderer, const SDL_Color color) {
    return SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}
bool SDL_GetColorEqual(const SDL_Color x, const SDL_Color y) {
    return x.r == y.r && x.g == y.g && x.b == y.b && x.a == y.a;
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

char* SDL_GetStrColor(const SDL_Color color) {
    static char string[] = "[255, 255, 255, 255]";
    static size_t len = 0;
    if (len == 0) {
        len = sizeof(string);
    }
    snprintf(string, len, "[%3d, %3d, %3d, %3d]", color.r, color.g, color.b, color.a);
    return string;
}
char* SDL_GetStrFRect(const SDL_FRect rect) {
    static char string[32];
    snprintf(string, 31, "[%.2f, %.2f, %.2f, %.2f]", rect.x, rect.y, rect.w, rect.h);
    return string;
}
bool SDL_GetSurfaceColor(SDL_Surface* surface, const int x, const int y, SDL_Color* color) {
    if (surface == NULL) {
        printf("%s: surface is null.\n", __func__);
        return false;
    }
    SDL_ReadSurfacePixel(surface, x, y, &color->r, &color->g, &color->b, &color->a);
    return true;
}

bool SDL_LoadDstRectAligned(
    SDL_FRect *dst_rect,
    SDL_Texture *texture,
    const SDL_FRect *src_rect,
    const SDL_FRect *gid_rect,
    const SDL_FRect *bck_rect,
    int anchor
) {
    // Req Condition
    if (dst_rect == NULL) {printf("%s: dst_rect not exists.\n", __func__); return false;}

    // load src (Opt Condition)
    SDL_FRect src = {0, 0, 0, 0};
    if (texture == NULL && src_rect == NULL) {
        src.w = (float)windowWidth;
        src.h = (float)windowHeight;
    }
    else if (src_rect != NULL) {
        src.w = src_rect->w;
        src.h = src_rect->h;
    }
    else {SDL_GetTextureSize(texture, &src.w, &src.h);}

    // load gid (Opt Condition)
    SDL_FRect gid = {0, 0, 1, 1};
    if (gid_rect != NULL) {gid = *gid_rect;}

    // load bck (Opt Condition)
    SDL_FRect bck = {0, 0, (float)windowWidth, (float)windowHeight};
    if (bck_rect != NULL) {bck = *bck_rect;}

    // load dst_rect -> w, h
    dst_rect->w = src.w * gid.w;
    dst_rect->h = src.h * gid.h;

    // load dst_rect -> x, y
    anchor += 40;
    const int x = anchor % 9;
    const int y = anchor / 9;
    float cx = 0, cy = 0, dx = 0, dy = 0;
    switch (x / 3) {
        case 0: cx = bck.x            ; break;
        case 1: cx = bck.x + bck.w / 2; break;
        case 2: cx = bck.x + bck.w    ; break;
        default: break;
    }
    switch (y / 3) {
        case 0: cy = bck.y            ; break;
        case 1: cy = bck.y + bck.h / 2; break;
        case 2: cy = bck.y + bck.h    ; break;
        default: break;
    }
    switch (x % 3) {
        case 0: dx = -dst_rect->w    ; break;
        case 1: dx = -dst_rect->w / 2; break;
        case 2: dx = 0               ; break;
        default: break;
    }
    switch (y % 3) {
        case 0: dy = -dst_rect->h    ; break;
        case 1: dy = -dst_rect->h / 2; break;
        case 2: dy = 0               ; break;
        default: break;
    }
    dst_rect->x = cx + dx + gid.x;
    dst_rect->y = cy + dy + gid.y;
    return true;
}
bool SDL_RenderTextureAligned(
    SDL_Renderer* renderer,
    SDL_Texture* texture,
    const SDL_FRect* src_rect,
    const SDL_FRect* gid_rect,
    const SDL_FRect* bck_rect,
    const int anchor
    ) {
    // Req Condition
    if (renderer == NULL) {printf("%s: renderer not exists.\n", __func__); return false;}
    if (texture == NULL) {printf("%s: texture  not exists.\n", __func__); return false;}

    //
    SDL_FRect dst_rect;
    SDL_LoadDstRectAligned(&dst_rect, texture, src_rect, gid_rect, bck_rect, anchor);
    return SDL_RenderTexture(renderer, texture, src_rect, &dst_rect);
}