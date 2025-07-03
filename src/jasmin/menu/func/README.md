# Func

```mermaid
graph LR
    interface.h --> func.h
    func.h -- load.c, unload.c --> func.h
    func.h -- init.c, deinit.c --> func.h
    func.h -- draw.c, renew.c --> func.h
```
