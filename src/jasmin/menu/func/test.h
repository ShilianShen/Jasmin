#ifndef TEST_H
#define TEST_H


bool testElem(const Elem* elem, const char* string) {
    if (elem == NULL) {printf("%s: elem not exists.\n", string); return false;}
    if (elem->id < 0) {printf("%s: elem id is negative.\n", string); return false;}
    if (elem->string == NULL) {printf("%s: elem[%d].string not exists.\n", string, elem->id); return false;}
    if (elem->texture == NULL) {printf("%s: elem[%d].texture not exists.\n", string, elem->id); return false;}
    return true;
}
bool testPage(const Page* page, const char* string) {
    if (page == NULL) {if (string != NULL) {printf("%s: page not exists.\n", string);} return false;}
    if (page->name == NULL) {if (string != NULL) {printf("%s: name not exists.\n", string);} return false;}
    return true;
}


#endif //TEST_H
