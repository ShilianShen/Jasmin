#ifndef TRIG_H
#define TRIG_H


union TrigPara {
    char* pageName;
};
enum TrigType {
    TRIG_TYPE_PASS,
    TRIG_TYPE_FORWARD,
    TRIG_TYPE_BACKWARD,
    TRIG_TYPE_CLEAR,
    TRIG_TYPE_KNOB,
    TRIG_NUM_TYPES
};
typedef enum TrigType TrigType;
typedef union TrigPara TrigPara;
typedef void (*TrigFunc)(TrigPara);
struct Trig {
    TrigType type;
    TrigFunc func;
    TrigPara para;
};
typedef struct Trig Trig;


#endif //TRIG_H