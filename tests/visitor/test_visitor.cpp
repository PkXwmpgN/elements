#include <vector>
#include <memory>
#include <assert.h>

#include <elements/design/visitor.h>

struct entity : eps::design::visitable<entity>
{
public:
    EPS_DESIGN_VISITABLE();

public:
    explicit entity(size_t test)
        : test_(test)
    {}
    virtual ~entity() {}

    size_t test() const { return test_; }

private:

    size_t test_;
};

struct geometry : entity
{
public:
    EPS_DESIGN_VISITABLE();

public:
    using entity::entity;
};

struct model : geometry
{
public:
    EPS_DESIGN_VISITABLE();

public:
    using geometry::geometry;
};

struct test_process_1 : eps::design::visitor<test_process_1, entity>
{
public:
    EPS_DESIGN_VISIT(entity);

public:
    bool visit(entity & obj)
    {
        assert(obj.test() == 0 || obj.test() == 2 || obj.test() == 4);
        return false;
    }
};

struct test_process_2 : eps::design::visitor<test_process_2, entity>
{
public:
    EPS_DESIGN_VISIT(entity, geometry);

public:
    bool visit(entity & obj)
    {
        assert(obj.test() == 0 || obj.test() == 2 || obj.test() == 4);
        return false;
    }

    bool visit(geometry & obj)
    {
        assert(obj.test() == 3);
        return false;
    }
};

struct test_process_3 : eps::design::visitor<test_process_3, entity>
{
public:
    EPS_DESIGN_VISIT(entity, geometry, model);

public:
    bool visit(entity & obj)
    {
        assert(obj.test() == 0 || obj.test() == 2 || obj.test() == 4);
        return false;
    }

    bool visit(geometry & obj)
    {
        assert(obj.test() == 3);
        return false;
    }

    bool visit(model & obj)
    {
        assert(obj.test() == 1 || obj.test() == 5);
        return false;
    }
};

struct test_process_4 : eps::design::visitor<test_process_4, entity>
{
public:
    EPS_DESIGN_VISIT(entity, geometry, model);

public:
    bool visit(entity &)
    {
        ++counter_entities;
        return false;
    }

    bool visit(geometry &)
    {
        ++counter_geometry;
        return false;
    }

    bool visit(model &)
    {
        ++counter_models;
        return false;
    }

    size_t counter_entities = 0;
    size_t counter_geometry = 0;
    size_t counter_models = 0;
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

    test_process_4 test_4;
    for(auto & obj : entities)
        test_4(*obj);

    assert(test_4.counter_entities == 3);
    assert(test_4.counter_geometry == 1);
    assert(test_4.counter_models == 2);
}
