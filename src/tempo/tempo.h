#ifndef JASMIN_TEMPO_H
#define JASMIN_TEMPO_H


#include "../basic/basic.h"
#include "../perph/perph.h"


/**
 * A Json interface.
 */
#define TEMPO_JSON "../config/tempo.json"


/**
 * A To-Implement interface.
 * Will be used in 'tempo.json' to map 'func' to a TrigFunc.
 */
extern const Table TEMPO_OUTER_TRIG_FUNC_TABLE;


/**
 * A To-Implement interface.
 * Will be used in 'tempo.json' to map 'para'/'key' to a void*.
 */
extern const Table TEMPO_PTR_TABLE;
struct TEMPO_TypeFunc {
    bool (*create)(void*, const cJSON*);
    SDL_Texture* (*texture)(void*);
    bool (*trig)(void*, SDL_FRect);
    void (*delete)(void*);
};
typedef struct TEMPO_TypeFunc TEMPO_TypeFunc;
extern const Table TEMPO_TYPE_FUNC_TABLE;


/**
 * To-Implement interface type.
 * Add it to TEMPO_PTR_TABLE, so it can be found by 'tempo.json'.
 */
typedef struct {
    TrigPara info; /** It should point at the manually implemented structure. */
    SDL_Texture* (*texture)(TrigPara); /** A function to renew & return the texture, which will be drawn. */
    bool (*trig)(TrigPara, SDL_FRect); /** A function to set Trig, be called when needed, the dst_rect will be given. */
} TEMPO_TypeManu;


bool TEMPO_Init();
void TEMPO_Exit();
bool TEMPO_Renew();
bool TEMPO_Draw();


#endif //JASMIN_TEMPO_H