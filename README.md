# README

## CODING STYLE

标识符:

* 「变长数据类型」(通常和物理变量相关): lower_snake_case
* 「定长数据类型」(通常和逻辑变量相关): camelCase

函数签名:

* createSomthing: 根据参数创建sth, sth通常是声明了数量不为一的结构体.
* destroySomthing: 和create对应的函数, 释放sth及其属性, sth通常是声明了数量不为一的结构体.
* loadSomething: 根据参数修改sth的东西, 通常比renew开销大, sth通常是声明了数量为一的结构体实例.
* killSomething: 和load对应的函数, 释放sth的属性, sth通常是声明了数量为一的结构体实例.
* renewSomething: 在循环中反复调用
* drawSomethinh: 在renew后显示的函数, 在循环中反复调用

## MENU

### ELEM

### PAGE

### MENU
