#Snape

###Contents
* [Acyclic visitor](#acyclic-visitor)


##Acyclic visitor
An implementation of the acyclic visitor pattern without RTTI

###Usage

```
// class hierarchy

struct entity : snape::visitable<entity>
{
public:

    SNAPE_VISITABLE();

    // something
};

struct geometry : entity
{
public:

    SNAPE_VISITABLE();

    // something      
};

struct model : entity
{
public:

    SNAPE_VISITABLE();

    // something      
};

// visitor
template<typename _Visitor>
using visitor_entities = snape::visitor<_Visitor, snape::visitor_traits<entity>>;

struct visitor : visitor_entities<visitor>
{
public:

    SNAPE_VISIT(entity, geometry, model);

public:

    void visit(const entity & obj)
    {
        // something
    }

    void visit(const geometry & obj)
    {
        // something
    }

    void visit(const model & obj)
    {
        // something
    }
};

// usage
std::vector<entity*> entities =
{
    new entity,
    new model,
    new entity,
    new geometry,
    new entity,
    new model
};

visitor test_visitor;
for(const auto & obj : entities)
    test_visitor(*obj);

```

###Performance

Iterate of three million items

| Pattern | Performance(ms) |
| :--- | :--- |
| Cyclic visitor | 11.3 |
| Acyclic visitor(RTTI) | 220.4 |
| Snape's acyclic visitor | 18.2 |

-----------------
###Author
Alexey Yegorov.
If you have an idea, feel free to get in touch with me yegorov.alex@gmail.com
