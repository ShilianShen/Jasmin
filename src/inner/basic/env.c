#include "env.h"


bool Init(const int len, const Env arr[len]) {
    for (int i = 0; i < len; i++) {
        if (arr[i].init != NULL && arr[i].init() == false) {
            printf("%s: fail in %s.\n", __func__, arr[i].name);
            return false;
        }
    }
    return true;
}
bool Renew(const int len, const Env arr[len]) {
    for (int i = 0; i < len; i++) {
        if (arr[i].renew != NULL && arr[i].renew() == false) {
            printf("%s: fail in %s.\n", __func__, arr[i].name);
            return false;
        }
    }
    return true;
}
bool Draw(const int len, const Env arr[len]) {

    for (int i = len - 1; i >= 0; i--) {
        if (arr[i].draw != NULL && arr[i].draw() == false) {
            printf("%s: fail in %s.\n", __func__, arr[i].name);
            return false;
        }
    }

    return true;
}
void Exit(const int len, const Env arr[len]) {
    for (int i = len - 1; i >= 0; i--) {
        if (arr[i].exit != NULL) {
            printf("%s: exiting: %s.\n", __func__, arr[i].name);
            arr[i].exit();
        }
    }
}