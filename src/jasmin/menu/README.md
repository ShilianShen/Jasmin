# Menu

```mermaid
graph LR
    ../interface.h --> trig.h --> elem.h --> page.h --> menu.h --> ../menu.h
    trig.h -- trig.c --> menu.h
    elem.h -- elem.c --> elem.h
    page.h -- page.c --> page.h
    menu.h -- menu.c --> menu.h
    ../interface.h --> ../menu.h
```

> trig.c依赖menu.h的部分函数, 这部分函数不属于../menu.h所暴露的函数, 所以不能指向../menu.h.

## 介绍

Menu为Jasmin的子模块, 承担Mouse和屏幕交互的主要功能.

## 功能

* 静态图片/文本展示
* 可选, 频繁从toml文件重新渲染
* 按钮事件
* 界面跳转

## 设计

```mermaid
graph LR
    INIT --> LOAD --> run{RUN?}
    run -- true --> RENEW --> DRAW --> reload{RELOAD?}
    reload -- true --> RELOAD --> run
    reload -- false --> run
    run -- false --> UNLOAD --> DEINIT
```

非频繁重载: init -> load -> renew -> draw -> unload -> deinit

频繁重载:
