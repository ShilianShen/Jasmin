# Tempo

```mermaid
erDiagram
    Trig {
        TrigType type
        TirgFunc func
        TrigPara para
    }
    Elem {
        ElemType type
        ElemInfo info
        int anchor
        Trig trig
        SDL_Texture tex
        SDL_FRect gid
        SDL_FRect src
    }
    Page {
        char[] name
        Elem[] elemSet
    }
    Menu {
        Page[] pageSet
    }
    Trig ||--|| Elem: "Trig is Elem.trig"
    Elem }|--|| Page: "Elem in Page.elemSet"
    Page }|--|| Menu: "Page in Menu.pageSet"
  
```

```mermaid
graph LR
    interface.h --> basic.h --> trig.h --> elem.h --> page.h --> menu.h --> tempo.h
    basic.h -- basic.c --> basic.h
    trig.h -- trig.c --> menu.h
    elem.h -- elem.c --> elem.h
    page.h -- page.c --> page.h
    menu.h -- menu.c --> menu.h
    interface.h --> tempo.h
```

## 介绍

Menu为Jasmin的子模块, 承担Mouse和屏幕交互的主要功能.


| ElemType | TrigType    |
|----------|-------------|
| NULL     | NULL        |
| FILE     | MENU, OTHER |
| TEXT     | MENU, OTHER |
| SLID     | SLID        |
| KNOB     | KNOB        |



## 功能

* 静态图片/文本展示
* 可选, 频繁从toml文件重新渲染
* 按钮事件
* 界面跳转

