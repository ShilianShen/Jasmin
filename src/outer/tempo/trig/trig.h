#ifndef JASMIN_MENU_STRUCT_TRIG_H
#define JASMIN_MENU_STRUCT_TRIG_H


#include "../basic/basic.h"


/*
 * Trig是Menu中调用函数的手段, 涉及的数据类型有Trig, TrigName, TrigFunc, TrigPara和变量trig_set.
 * TrigFunc函数和TrigPara外来参数两者一起描述调用函数的具体细节, 即实际发生trigFunc(trigPara).
 * TrigName应该是TrigFunc的文本解释.
 * TrigName和TrigFunc共同构成Trig.
 * 所有的Trig构成有限的trig_set.
 * trig_set是检索范围, TrigName和TrigFunc是检索关键字.
 * TrigName和TrigFunc被定义双射, 即可以用TrigName在trig_set内查找到TrigFunc或相反, 同样是Trig内的唯一检索手段.
 *
 * 流: toml -> (TrigName, TrigPara) -(trig_set)-> (TrigFunc, TrigPara) -> TrigFunc(TrigPara)
 *
 * Q: 为什么用TrigFunc和TrigPara两个对象来描述而不是只用一个对象?
 * A: 如果用一个对象来描述, 那么在能forward到n个page时, 需要储存的空间是n*sizeof(forward), 但是分成两个参数就不需要了.
 */


// datatype
typedef void (*TrigFunc)(const char*);
typedef struct Trig2 {
    char* name;
    const TrigFunc func;
} Trig2;
typedef struct Trig {
    TrigFunc func;
    char* para;
} Trig;


void TRIG_FUNC_Pass(const char*);
void TRIG_FUNC_Forward(const char*);
void TRIG_FUNC_Backward(const char*);
void TRIG_FUNC_Clear(const char*);


extern Trig2 trig_set[];


TrigFunc TRIG_FindFuncFromName(const char* name);
char* TRIG_FindNameFromFunc(TrigFunc func);


#endif //JASMIN_MENU_STRUCT_TRIG_H
