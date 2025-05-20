# README

> # 过早的优化是万恶之源

## CODING STYLE

标识符:

* 「变长数据类型」(通常和物理变量相关): lower_snake_case
* 「定长数据类型」(通常和逻辑变量相关): camelCase

函数签名:

* bool testSth: 检查是否正常经过了load
* void initSth: 最优先调用, 建议只调用一次, 理论上可以在kill之后再调用, 但是应该不会有任何改变.
* void loadSth: 可以调用多次, 在kill后调用
* void killSth: 可以调用多次
* void renewSth: 可以调用多次, 但是在一次循环中应该只调用一次
* void drawSth: 可以调用多次, 但是在一次循环中应该只调用一次

函数流:

0. init
1. load
2. renew
3. draw如果不退出, 回到renew; 如果频繁更新kill后回到load; 否则到下一步
4. kill

loadStruct分两种情况:

1. loadStruct开头的函数有且仅有loadStruct()
2. loadStruct开头的函数有loadStructAttributes(), loadStructOthers(), loadStruct(),其中:
   * loadStructAttributes: 载入指定属性
   * loadStructOthers: 可选, 载入其他属性
   * loadStruct: 为以上函数的集合, 不可在其中擅自load.

简单来说就是要么「一个函数完成」要么就是「分多个函数完成」.

传入参数检查:

条件检查分为「先决条件PreCondition」「必要条件ReqCondition」「非必要条件OptCondition」「自动修正条件AtoCondition」

先决条件: 调用函数的条件, 但是被写到了函数内部, 「不满足先决条件」不被视作「错误」

必要条件: 执行函数的条件, 「不满足先决条件」被视作「错误」, 会打印错误.

非必要条件: 执行函数的细节条件, 「不满足先决条件」不被视作「错误」, 而且会由默认值填充.

* 自定义的函数需要检查参数
* 如果主要作用对象不存在不用报错
* 对于函数嵌套的情况, 如果非必要, 只在嵌套末端检查即可

## MENU

### ELEM

#### ElemState 元素状态

#### TrigFunc 元素诱发函数


| func     | datatype |
| -------- | -------- |
| pass     | void     |
| forward  | string   |
| backward | string   |

### PAGE

### MENU
