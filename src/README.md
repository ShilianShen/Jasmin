# Src
```mermaid
flowchart
    third_party --> basic --> jasmin
    basic --> debug --> jasmin
    debug --> device --> jasmin
    device --> lotri --> jasmin
    device --> tempo --> jasmin
    lotri --> villa --> jasmin
    tempo --> villa --> jasmin
```