#include "action.h"
#include "entity.h"
#include "intel_arr.h"


Action actionSet[NUM_ACTIONS] = {
    [ACTION_NULL] = {.type = ACTION_TYPE_ONE_WAY},
    [ACTION_IS] = {.type = ACTION_TYPE_TWO_WAY},
    [ACTION_BELONG] = {.type = ACTION_TYPE_ONE_WAY},
    [ACTION_WILL] = {.type = ACTION_TYPE_ONE_WAY},
    [ACTION_CAN] = {.type = ACTION_TYPE_ONE_WAY},
};
const char* ACTION_NAMES[NUM_ACTIONS] = {
    [ACTION_NULL] = "unknown",
    [ACTION_IS] = "is",
    [ACTION_BELONG] = "belong",
    [ACTION_WILL] = "will",
    [ACTION_CAN] = "can",
};
