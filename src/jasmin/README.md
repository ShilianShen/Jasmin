# Jasmin

## 简介 Intro

Jasmin是基于SDL3的, OolongImPress自用的GUI.

## 功能 Function

* debug能力
* menu二维菜单的交互
* 3d模型的渲染

## 依赖 Dependency

```mermaid
graph
    SDL3 --> basic_interface.h
    toml --> basic_interface.h

    subgraph BASIC
        basic_interface.h --> setting --> ease --> debug --> device --> basic.h
    end

    subgraph MENU
        basic.h --> menu_interface.h --> menu_struct_interface.h
  
        subgraph MENU_STRUCT
            menu_struct_interface.h --> trig.h --> elem
            elem --> page --> menu' --> trig.c --> menu_struct.h
        end

        menu_struct.h --> menu_func_interface.h
  
        subgraph MENU_FUNC
            menu_func_interface.h --> draw --> menu_func.h
            menu_func_interface.h --> init --> menu_func.h
            menu_func_interface.h --> kill --> menu_func.h
            menu_func_interface.h --> load --> menu_func.h
            menu_func_interface.h --> renew --> menu_func.h
        end
        menu_func.h --> menu.h
    end
  
    style BASIC fill: black
    style MENU fill: black

    style basic_interface.h fill: darkslategray
    style menu_struct_interface.h fill: darkslategray
    style menu_func_interface.h fill: darkslategray
    style menu_interface.h fill: darkslategray
  
    style basic.h fill: peru
    style menu_struct.h fill: peru
    style menu_func.h fill: peru
    style menu.h fill: peru

    basic.h --> menu.h --> jasmin
    basic.h --> lo_tri --> jasmin
```
