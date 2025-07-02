# Math

```mermaid
graph LR
    interface.h --> vec.h --> mat.h --> math.h
  
    mat.h --mat.c--> mat.h
    vec.h --vec.c--> vec.h
    math.h --math.c--> math.h
```
