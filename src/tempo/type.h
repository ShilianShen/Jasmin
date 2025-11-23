#ifndef TEMPO_TYPE_H
#define TEMPO_TYPE_H


#include "tempo.h"
#include "type/text.h"
#include "type/file.h"
#include "type/slid.h"
#include "type/bool.h"


typedef enum {
    TEMPO_TYPE_NULL,
    TEMPO_TYPE_FILE,
    TEMPO_TYPE_TEXT,
    TEMPO_TYPE_SLID,
    TEMPO_TYPE_BOOL,
    TEMPO_NUM_TYPES,
} TypeId;
typedef union {
    TypeFileInfo file;
    TypeTextInfo text;
    TypeSlidInfo slid;
    TypeBoolInfo bool_;
} TypeInfo;
typedef struct Type Type;
struct Type {
    TypeInfo info;
    TypeId id;
    SDL_Texture* texture;
};
typedef struct {
    const char* name;
    bool (*create)(void*, const cJSON*);
    bool (*renew)(const void*, SDL_Texture**);
    void (*delete)(void*);
    Trig trig;
} TypeFunc;
extern const TypeFunc TYPE_INFO_DETAIL[TEMPO_NUM_TYPES];
TypeId TEMPO_GetTypeFromString(const char* string);
Trig TEMPO_GetTypeTrig(const Type* type);
SDL_Texture* TEMPO_GetTypeTexture(const Type* type);
Type* TEMPO_CreateType(const cJSON* type_json);
Type* TEMPO_DeleteType(Type* type);
bool TEMPO_RenewType(Type* type);


#endif //TEMPO_TYPE_H