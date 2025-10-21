#ifndef ACTION_H
#define ACTION_H


#include "intel.h"


typedef enum {
    ACTION_NULL,
    ACTION_IS,
    ACTION_BELONG,
    ACTION_WILL,
    ACTION_CAN,
    NUM_ACTIONS,
} ActionId;
typedef enum {ACTION_TYPE_ONE_WAY, ACTION_TYPE_TWO_WAY, NUM_ACTION_TYPES} ActionType;
typedef struct {ActionType type;} Action;


extern Action actionSet[NUM_ACTIONS];
extern const char* ACTION_NAMES[NUM_ACTIONS];


#endif //ACTION_H