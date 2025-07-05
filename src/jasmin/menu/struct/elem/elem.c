#include "elem.h"


void ELEM_TurnOn(Elem* elem) {
    // Req Condition
    if (elem == NULL) {
        printf("%s: elem not exists.\n", __func__);
        return;
    }

    //
    elem->on = true;
}
void ELEM_TurnOff(Elem* elem) {
    // Req Condition
    if (elem == NULL) {
        printf("%s: elem not exists.\n", __func__);
        return;
    }

    //
    elem->on = false;
}
bool ELEM_IfReady(const Elem* elem) {
    // Req Condition
    if (elem == NULL) {
        printf("%s: elem not exists.\n", __func__);
        return false;
    }
    if (elem->on == false) {
        printf("%s: elem is off.\n", __func__);
        return false;
    }
    if (elem->id < 0) {
        printf("%s: elem.id is illegal.\n", __func__);
        return false;
    }
    if (elem->string == NULL) {
        printf("%s: elem[%d].string not exists.\n", __func__, elem->id);
        return false;
    }
    if (elem->texture == NULL) {
        printf("%s: elem[%d].texture not exists.\n", __func__, elem->id);
        return false;
    }

    //
    return true;
}
char* ELEM_GetStateName(const ElemState state) {
    // Req Condition
    if (state >= NUM_ELEM_STATES) {
        printf("%s: state is illegal.\n", __func__);
        return NULL;
    }

    //
    static char* names[] = {"OUTSIDE", "INSIDE", "PRESSED", "RELEASED"};
    return names[state];
}