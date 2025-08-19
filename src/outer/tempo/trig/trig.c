#include "../trig/trig.h"


KeyVal TEMPO_MENU_TRIG_SET[TRIG_NUM_TYPES] = {
    [TRIG_TYPE_PASS] = {"pass", TEMPO_TrigFuncPass},
    [TRIG_TYPE_FORWARD] = {"forward", TEMPO_TrigFuncForward},
    [TRIG_TYPE_BACKWARD] = {"backward", TEMPO_TrigFuncBackward},
    [TRIG_TYPE_CLEAR] = {"clear", TEMPO_TrigFuncClear},
    [TRIG_TYPE_KNOB] = {"knob", NULL},
};


TrigType TRIG_GetTypeFromName(const char* name) {
    for (int i = 0; i < TRIG_NUM_TYPES; i++) {
        if (strcmp(name, TEMPO_MENU_TRIG_SET[i].key) == 0) {
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
    trig->func = TEMPO_MENU_TRIG_SET[trig->type].val;
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

            break;
        }
        default: break;
    }
    return true;
}
Trig* TEMPO_DeleteTrig(Trig* trig) {
    if (trig == NULL) {
        return trig;
    } // Opt Condition

    switch (trig->type) {
        case TRIG_TYPE_FORWARD: {
            if (trig->para.pageName != NULL) {
                free(trig->para.pageName);
                trig->para.pageName = NULL;
            } // Opt Condition
            break;
        }
        default: break;
    }

    trig->func = NULL;
    free(trig);
    trig = NULL;
    return trig;
}
Trig* TEMPO_CreateTrig(const toml_table_t* tomlTrig) {

    if (tomlTrig == NULL) {
        printf("%s: tomlTrig == NULL.\n", __func__);
        return NULL;
    } // Req Condition

    Trig* trig = calloc(1, sizeof(Trig)); // alloc
    if (trig == NULL) {
        printf("%s: trig == NULL.\n", __func__);
        return NULL;
    } // Req Condition

    if (TEMPO_CreateTrig_RK(trig, tomlTrig) == false) {
        printf("%s: TEMPO_CreateTrig_RK failed.\n", __func__);
        trig = TEMPO_DeleteTrig(trig); // free
    } // Req Condition
    return trig;
}