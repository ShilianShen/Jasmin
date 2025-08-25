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


| Return Type | Function Name      | Parameters |
| ----------- | ------------------ | ---------- |
| MyStruct*   | XX_CreateMyStruct  | ...        |
| void        | XX_DestroyMyStruct | MyStruct*  |

* init & deinit: Initialization (called only once before deinit) and deinitialization (can be called repeatedly after unload). These typically do not require file parameters.
* load & unload: Loading (called only once after init and before unload) and unloading (can be called repeatedly). These usually involve file-related parameters.
* renew: Refresh (can be called repeatedly after load and before unload). Refreshes data.
* draw: Drawing (can be called repeatedly after load and before unload). Renders data and must not involve any non-temporary dynamic memory allocation.

## Para_CK: Parameter Checking

The types of condition checks are:

* Pre Condition: Conditions required to call the function. The condition checks are written inside the function. "Failing to meet preconditions" is not considered an "error".
* Req Condition: Conditions required to call the function. "Failing to meet required conditions" is considered an "error" and will output an error message.
* Opt Condition: Detailed conditions of the function. "Failing to meet optional conditions" is not considered an "error", and default values will be filled in instead.
