# Jasmin by OolongImPress

![Logo](./docs/icon/icon_64.png)

## INTRO

Jasmin is a 2D&3D rendering library based on C and SDL3, currently for internal use by OolongImPress.

> ATTENTION: This doc is still under construction!

## INSTALL & UPGRADE

### macOS

```bash
brew install sdl3 sdl3_ttf sdl3_image
```

```bash
brew upgrade sdl3 sdl3_ttf sdl3_image
```

## Special Function Identifiers

The following are special function identifiers and their explanations:


| type   | Return  | Function Name   | Para                   | Explan |
| ------ | ------- | --------------- | ---------------------- | ------ |
| extern | bool    | XX_Init         | void                   |        |
| extern | bool    | XX_Exit         | void                   |        |
| extern | bool    | XX_Renew        | void                   |        |
| extern | bool    | XX_Draw         | void                   |        |
| extern | Struct* | XX_CreateStruct | const cJSON*           |        |
| extern | Struct* | XX_DeleteStruct | Struct*                |        |
| extern | bool    | XX_RenewStruct  | Struct*                |        |
| extern | bool    | XX_DrawStruct   | const Struct*          |        |
| static | bool    | XX_YY_ZZ        | Struct*                |        |
| static | bool    | XX_YY_RK        | Struct*, const cJSON* | Risk   |
| static | bool    | XX_YY_CK        | Struct*                | Check  |

## Para_CK: Parameter Checking

The types of condition checks are:

* Pre Condition: Conditions required to call the function. The condition checks are written inside the function. "Failing to meet preconditions" is not considered an "error".
* Req Condition: Conditions required to call the function. "Failing to meet required conditions" is considered an "error" and will output an error message.
* Opt Condition: Detailed conditions of the function. "Failing to meet optional conditions" is not considered an "error", and default values will be filled in instead.
