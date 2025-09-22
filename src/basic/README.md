# Basic
```mermaid
graph LR
    subgraph BASIC
        _basic.h --> ease.h --> basic.h
        _basic.h --> env.h --> basic.h
        _basic.h --> func.h --> basic.h
        _basic.h --> setting.h --> basic.h
        _basic.h --> table.h --> basic.h
    end
    third_party --> _basic.h
```
