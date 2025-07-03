#include "elem.h"


bool testElem(const Elem* elem, const char* string) {
    if (elem == NULL) {printf("%s: elem not exists.\n", string); return false;}
    if (elem->id < 0) {printf("%s: elem id is negative.\n", string); return false;}
    if (elem->string == NULL) {printf("%s: elem[%d].string not exists.\n", string, elem->id); return false;}
    if (elem->texture == NULL) {printf("%s: elem[%d].texture not exists.\n", string, elem->id); return false;}
    return true;
}
char* getElemStateName(const ElemState state) {
    static char* names[] = {"OUTSIDE", "INSIDE", "PRESSED", "RELEASED"};
    return names[state];
}