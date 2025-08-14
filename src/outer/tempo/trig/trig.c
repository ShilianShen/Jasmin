#include "../trig/trig.h"


struct {const char* name; TrigFunc func;} TRIG_INFO_SET[TRIG_NUM_TYPES] = {
    [TRIG_TYPE_PASS] = {"pass", TEMPO_TrigFuncPass},
    [TRIG_TYPE_FORWARD] = {"forward", TEMPO_TrigFuncForward},
    [TRIG_TYPE_BACKWARD] = {"backward", TEMPO_TrigFuncBackward},
    [TRIG_TYPE_CLEAR] = {"clear", TEMPO_TrigFuncClear},
    [TRIG_TYPE_KNOB] = {"knob", NULL},
};


// GET & SET ===========================================================================================================
TrigFunc TRIG_GetFuncFromName(const char* name) {
    // Req Condition
    if (name == NULL) {printf("%s: name not exists.\n", __func__); return NULL;}

    for (int i = 0; i < TRIG_NUM_TYPES; i++) {
        if (strcmp(name, TRIG_INFO_SET[i].name) == 0) {
            return TRIG_INFO_SET[i].func;
        }
    }
    return NULL;
}
const char* TRIG_GetNameFromFunc(const TrigFunc func) {
    // Req Condition
    if (func == NULL) {printf("%s: func not exists.\n", __func__); return NULL;}

    for (int i = 0; i < TRIG_NUM_TYPES; i++) {
        if (TRIG_INFO_SET[i].func == func) {
            return TRIG_INFO_SET[i].name;
        }
    }
    return NULL;
}
TrigType TRIG_GetTypeFromName(const char* name) {
    for (int i = 0; i < TRIG_NUM_TYPES; i++) {
        if (strcmp(name, TRIG_INFO_SET[i].name) == 0) {
            return i;
        }
    }
    return 0;
}


// CREATE & DELETE =====================================================================================================
static bool TEMPO_CreateTrig_RK(Trig* trig, const toml_table_t* tomlTrig) {
    const toml_datum_t tomlFuncName = toml_string_in(tomlTrig, "func");
    if (tomlFuncName.ok == false) {
        printf("%s: tomlFuncName.ok == false.\n", __func__);
        return false;
    } // Req Condition

    trig->type = TRIG_GetTypeFromName(tomlFuncName.u.s);
    trig->func = TRIG_INFO_SET[trig->type].func;
    if (trig->func == NULL) {
        printf("%s: func == NULL.\n", __func__);
        return false;
    } // Req Condition

    switch (trig->type) {
        case TRIG_TYPE_PASS: {break;}
        case TRIG_TYPE_FORWARD: {
            const toml_datum_t tomlPara = toml_string_in(tomlTrig, "para");
            if (tomlPara.ok == false) {
                printf("%s: tomlPara.ok == false.\n", __func__);
                return false;
            } // Req Condition

            trig->para.pageName = strdup(tomlPara.u.s);
            if (trig->para.pageName == NULL) {
                printf("%s: para == NULL.\n", __func__);
                return false;
            } // Req Condition

            return true;
        }
        default: {break;}
    }
    return true;
}
Trig* TEMPO_DeleteTrig(Trig* trig) {
    if (trig != NULL) {
        switch (trig->type) {
            case TRIG_TYPE_FORWARD: {
                if (trig->para.pageName != NULL) {
                    free(trig->para.pageName);
                    trig->para.pageName = NULL;
                }
            }
            default: {break;}
        }
        trig->func = NULL;
    }
    free(trig);
    return NULL;
}
Trig* TEMPO_CreateTrig(const toml_table_t* tomlTrig) {
    if (tomlTrig == NULL) {
        printf("%s: tomlTrig == NULL.\n", __func__);
        return NULL;
    } // Req Condition
    Trig* trig = calloc(1, sizeof(Trig));
    if (trig == NULL) {
        printf("%s: trig == NULL.\n", __func__);
        return NULL;
    } // Req Condition
    if (TEMPO_CreateTrig_RK(trig, tomlTrig) == false) {
        printf("%s: TEMPO_CreateTrig_RK failed.\n", __func__);
        trig = TEMPO_DeleteTrig(trig);
    } // Req Condition
    return trig;
}