#pragma once
#include <string>
#include<unordered_set>
#include<set>
#include<memory>
enum class type{
    Script,
    GameObject,
};
class Object{
    public:
    virtual ~Object() = 0;
    virtual type GetType() = 0;
    const unsigned int ID();
    Object(){ name = "Object"+std::to_string(ID()); };
    std::string name;
    protected:
    std::unique_ptr<const unsigned int>_id;
    private:
    friend struct IDBinder;
    struct IDBinder{
    inline static std::unordered_set<Object *> pool_ids;
    inline static std::set<unsigned int> deleted_ids;
    static const unsigned int OccupyID(Object & Target);
    static void RemoveID(unsigned int);
    };
};

