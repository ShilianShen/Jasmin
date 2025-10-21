#include "action.h"
#include "entity.h"
#include "intel_arr.h"


Action actionSet[NUM_ACTIONS] = {
    [ACTION_NULL] = {.name = "unknown", .type = ACTION_TYPE_ONE_WAY},
    [ACTION_IS] = {.name = "is", .type = ACTION_TYPE_TWO_WAY},
    [ACTION_BELONG] = {.name = "belong", .type = ACTION_TYPE_ONE_WAY},
    [ACTION_WILL] = {.name = "will", .type = ACTION_TYPE_ONE_WAY},
    [ACTION_CAN] = {.name = "can", .type = ACTION_TYPE_ONE_WAY},
};


bool INTEL_InitAction() {
    return true;
}
void INTEL_ExitAction() {
}