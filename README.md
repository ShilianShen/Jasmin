# Jasmin by OolongImPress

![Logo](doc/icon/icon_64.png)

## INTRO

Jasmin is a 2D&3D rendering library based on C and SDL3, currently for internal use by OolongImPress.

> ATTENTION: This doc is still under construction!

## MODULE

We divide the modules into 2 kinds: OpenModule & ClosedModule.

* OM: The all paras, funcs, structs, enums, unions in this module are callable.
* CM: The most paras, funcs, structs, enums, unions in this module are uncallable, except those declared in head file.

> OM-Example
> ```mermaid
> flowchart LR
>   in.h --> _module.h
>   _module.h --> detail_1.h --> detail_1.c
>   _module.h --> detail_2.h --> detail_2.c
>   detail_1.h --> module.h --> out.h
>   detail_2.h --> module.h
> ```

> CM-Example
> ```mermaid
> flowchart LR
>   in.h --> module.h --> out.h
>   module.h --> detail_1.h --> detail_1.c
>   module.h --> detail_2.h --> detail_2.c
>   detail_1.h --> module.c
>   detail_2.h --> module.c
> ```

## INSTALL & UPGRADE

### macOS

```bash
brew install sdl3 sdl3_ttf sdl3_image
```

```bash
brew upgrade sdl3 sdl3_ttf sdl3_image
```
