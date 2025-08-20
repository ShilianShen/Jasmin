#include "../trig/trig.h"


KeyVal TEMPO_MENU_TRIG_SET[] = {
    {"pass", TEMPO_TrigFuncPass},
    {"forward", TEMPO_TrigFuncForward},
    {"backward", TEMPO_TrigFuncBackward},
    {"clear", TEMPO_TrigFuncClear},
    {"knob", NULL},
    {NULL, NULL}
};


// CREATE & DELETE =====================================================================================================
static bool TEMPO_CreateTrig_RK(Trig* trig, const toml_table_t* tomlTrig) {
    memset(trig, 0, sizeof(Trig));

    const toml_datum_t tomlFuncName = toml_string_in(tomlTrig, "func");
    if (tomlFuncName.ok == false) {
        printf("%s: tomlFuncName.ok == false.\n", __func__);
        return false;
    } // Req Condition

    trig->func = BASIC_GetValByKey(TEMPO_MENU_TRIG_SET, tomlFuncName.u.s);
    if (trig->func == NULL) {
        printf("%s: func == NULL.\n", __func__);
        return false;
    } // Req Condition

    if (toml_string_in(tomlTrig, "para").ok) {
        const toml_datum_t tomlPara = toml_string_in(tomlTrig, "para");
        if (tomlPara.ok == false) {
            printf("%s: tomlPara.ok == false.\n", __func__);
            return false;
        } // Req Condition

        trig->para = strdup(tomlPara.u.s);
        if (trig->para == NULL) {
            printf("%s: para == NULL.\n", __func__);
            return false;
        } // Req Condition
    }

    return true;
}
Trig* TEMPO_DeleteTrig(Trig* trig) {
    if (trig == NULL) {
        return trig;
    } // Opt Condition

    if (trig->para != NULL) {
        free(trig->para);
        trig->para = NULL;
    } // Opt Condition

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