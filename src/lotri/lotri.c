#include "lotri.h"


bool LOTRI_Init() {
    return true;
}
bool LOTRI_Renew() {
    for (int i = 0; i < MAX_MODEL_BUFFER; i++) {
        modelBuffer[i] = NULL;
    }
    modelBufferHead = 0;
    LOTRI_RenewCamera();
    return true;
}
bool LOTRI_Draw() {
    LOTRI_DrawCamera();
    LOTRI_DrawModelBuffer();
    return true;
}
void LOTRI_Exit() {
}