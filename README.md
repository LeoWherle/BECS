# BECS - Basic Entity Component System

Small project for learning purpuses.
The goal isn't to create a perfect ECS, but to understand the different intricacies of how ECS's work.

The ECS should be small and easy to understand, without external dependencies.

## Global Architecture plan:

```mermaid
erDiagram
    Components-Tables {
        int component-id
        string official-name
        string description
        string table-name
    }
    All-Entities {
        int entity-id
        string label-DEBUG
    }
    Entity-Component-Mapping {
        int entity-id
        int component-id
        int component-data-id
    }
    Component-Position-Table {
        int component-data-id
        int x-y
    }
    Component-Health-Table {
        int component-data-id
        int amount
    }

    All-Assemblages {
        int assemblage-id
        string label-DEBUG
        string official-name
        string description
    }
    Assemblage-Components-Mapping {
        int assemblage-id
        int component-id
    }
    All-Components {
        int component-id
        string name
        string description
        string table-name
    }

    All-Components ||--o{ Components-Tables : "match"
    All-Entities ||--o{ Entity-Component-Mapping : "contains"
    Entity-Component-Mapping ||--o{ All-Components : "includes"
    Entity-Component-Mapping ||--o{ Component-Position-Table : "data"
    Entity-Component-Mapping ||--o{ Component-Health-Table : "data"
    All-Assemblages ||--o{ Assemblage-Components-Mapping : "template"
    Assemblage-Components-Mapping }o--|| All-Components : "component"
```

## Maybe later

It could have some network system to better understand how to manage clients, within an ECS.