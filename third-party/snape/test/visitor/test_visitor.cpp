#include <vector>
#include <memory>
#include <assert.h>

#include <snape/visitor.h>

struct entity : snape::visitable<entity>
{
public:

    SNAPE_VISITABLE();

public:

    explicit entity(size_t test)
        : test_(test)
    {}

    virtual ~entity()
    {}

    size_t test() const { return test_; }

private:

    size_t test_;
};

struct geometry : entity
{
public:

    SNAPE_VISITABLE();

public:

    using entity::entity;
};

struct model : geometry
{
public:

    SNAPE_VISITABLE();

public:

    using geometry::geometry;
};

template<typename _Visitor>
using test_process = snape::visitor<_Visitor, snape::visitor_traits<entity>>;

struct test_process_1 : test_process<test_process_1>
{
public:

    SNAPE_VISIT(entity);

public:

    void visit(const entity & obj)
    {
        assert(obj.test() == 0 || obj.test() == 2 || obj.test() == 4);
    }
};

struct test_process_2 : test_process<test_process_2>
{
public:

    SNAPE_VISIT(entity, geometry);

public:

    void visit(const entity & obj)
    {
        assert(obj.test() == 0 || obj.test() == 2 || obj.test() == 4);
    }

    void visit(const geometry & obj)
    {
        assert(obj.test() == 3);
    }
};

struct test_process_3 : test_process<test_process_3>
{
public:

    SNAPE_VISIT(entity, geometry, model);

public:

    void visit(const entity & obj)
    {
        assert(obj.test() == 0 || obj.test() == 2 || obj.test() == 4);
    }

    void visit(const geometry & obj)
    {
        assert(obj.test() == 3);
    }

    void visit(const model & obj)
    {
        assert(obj.test() == 1 || obj.test() == 5);
    }
};

struct test_process_counter : test_process<test_process_counter>
{
public:

    SNAPE_VISIT(entity, geometry, model);

public:
    void visit(const entity &)
    {
        ++counter_entities;
    }

    void visit(const geometry &)
    {
        ++counter_geometry;
    }

    void visit(const model &)
    {
        ++counter_models;
    }

    size_t counter_entities = 0;
    size_t counter_geometry = 0;
    size_t counter_models = 0;
};

struct test_process_counter_1 : test_process<test_process_counter_1>
{
public:

    SNAPE_VISIT(entity, geometry, model);

public:

    void visit(const entity &)
    {
        ++counter_entities;
    }

    size_t counter_entities = 0;
};

int main()
{
    std::vector<std::unique_ptr<entity>> entities;

    entities.emplace_back(new entity(0));
    entities.emplace_back(new model(1));
    entities.emplace_back(new entity(2));
    entities.emplace_back(new geometry(3));
    entities.emplace_back(new entity(4));
    entities.emplace_back(new model(5));

    test_process_1 test_1;
    for(auto & obj : entities)
        test_1(*obj);

    test_process_2 test_2;
    for(auto & obj : entities)
        test_2(*obj);

    test_process_3 test_3;
    for(auto & obj : entities)
        test_3(*obj);

    test_process_counter test_counter;
    for(auto & obj : entities)
        test_counter(*obj);

    assert(test_counter.counter_entities == 3);
    assert(test_counter.counter_geometry == 1);
    assert(test_counter.counter_models == 2);

    test_process_counter_1 test_counter_1;
    for(auto & obj : entities)
        test_counter_1(*obj);

    assert(test_counter_1.counter_entities == 6);

    test_process_counter test_constency;

    const entity & ref_entity = entity(1);
    const entity & ref_model = model(2);
    const entity & ref_geometry = geometry(3);

    test_constency(ref_entity);
    test_constency(ref_model);
    test_constency(ref_geometry);

    assert(test_constency.counter_entities == 1);
    assert(test_constency.counter_geometry == 1);
    assert(test_constency.counter_models == 1);
}
