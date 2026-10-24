#include "object.hpp"
#include "../debugger/debugger.hpp"
const unsigned int Object::IDBinder::OccupyID(Object & Target){
    LOG("Assigning ID", TRACE);
    if(deleted_ids.empty()){
        Target._id = std::make_unique<unsigned int>(pool_ids.size());
        pool_ids.insert(&Target);
        return *(Target._id);
    }else{
        Target._id = std::make_unique<unsigned int>(*deleted_ids.begin());
        pool_ids.insert(&Target);
        deleted_ids.erase(deleted_ids.begin());
        return *(Target._id);
         
    }
    
}
void Object::IDBinder::RemoveID(unsigned int id_to_del){
    LOG("Removing ID", TRACE);
    std::unordered_set<Object *>::iterator it;
    for(it = pool_ids.begin(); it != pool_ids.end(); it++){
        if((*(*(*it))._id) == id_to_del){
            pool_ids.erase(it);
            (*(*it))._id = nullptr;
            return;
        }
    }
    LOG("ID to delete not found", ERROR);
    return;
}
const unsigned int Object::ID(){
    if(_id == 0){ 
        return IDBinder::OccupyID(*this);
    }else{
        return *_id;
    }
    
}
Object::~Object(){};