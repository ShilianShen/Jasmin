#ifndef PAGE_H
#define PAGE_H


typedef char PageName;
typedef int PageId;
typedef int PageVolume;
const PageVolume PAGE_MAX_VOLUME = 32;


typedef struct {
    PageId id;
    PageName* name;  // malloc
    Elem elems[PAGE_MAX_VOLUME];
} Page;


#endif //PAGE_H
