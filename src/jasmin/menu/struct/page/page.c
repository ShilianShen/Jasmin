#include "page.h"


bool testPage(const Page* page, const char* string) {
    if (page == NULL) {if (string != NULL) {printf("%s: page not exists.\n", string);} return false;}
    if (page->name == NULL) {if (string != NULL) {printf("%s: name not exists.\n", string);} return false;}
    return true;
}