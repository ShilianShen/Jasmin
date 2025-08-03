```mermaid
stateDiagram
    info --> elem0: malloc
    elem0 --> elem: load
    info --> NULL: false
    elem0 --> info: free
```
