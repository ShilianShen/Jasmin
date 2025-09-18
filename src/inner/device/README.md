# DEVICE

```mermaid
flowchart
    _device.h --> mouse.h --> device.h --> device.c
    _device.h --> keyboard.h --> device.h
    mouse.h --> mouse.c
    keyboard.h --> keyboard.c
```
