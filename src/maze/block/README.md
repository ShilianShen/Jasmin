# block.png

虽然PNG有RGBA, 实际上只会用到RGB

色集, 用于区分对象的颜色, 有Block色集, 和Gene色集, 两套色集独立存在, 不能混用

## 色集

block.png会使用两套色集, 在边缘使用Block色集, 在非边缘使用Gene色集.

Block色集:

* MAZE16
* white: 终点
* black: 墙
* other: 墙

Gene色集:

* MAZE16, RGB(You)是空地, 其它都是墙
* white: 空地
* black: 墙
* other: 墙
