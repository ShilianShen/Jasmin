# Source

```mermaid
flowchart LR
    third_party --> basic --> jasmin
    basic --> debug --> jasmin
    debug --> perph --> jasmin
    debug --> lotri --> jasmin
    perph --> tempo --> jasmin
    intel --> villa --> jasmin
    debug --> intel --> jasmin
  
    lotri --> villa
    tempo --> villa --> jasmin
```
