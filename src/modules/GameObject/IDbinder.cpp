#include "GameObject.hpp"
#include "../../core/Debugger/debug.hpp"
template<typename T>
std::vector<unsigned int> IDBinder<T>::IDs = {};
template<typename T>
std::vector<size_t> IDBinder<T>::DeletedIDs = {};
template<typename T>
IDBinder<T>::IDBinder(){
    AssignedID = 0;
    needsToSort = false;
}
template<typename T>
IDBinder<T>::~IDBinder(){
    if(AssignedID != 0){
        for(size_t i = 0; i < IDs.size(); i++){
            if(AssignedID == IDs[i]){
                IDs.erase(IDs.begin()+i);
                DeletedIDs.emplace_back(AssignedID);
                AssignedID = 0;
                if(DeletedIDs.size() > 1){
                    needsToSort = true;
                }
            }
        }
    }
}
template<typename T>
const unsigned int IDBinder<T>::GetID() const{
    return AssignedID;
}
template<typename T>
size_t IDBinder<T>::SizeID() const{
    return IDs.size();
}
template<typename T>
void IDBinder<T>::OccupyID(){
    unsigned int temp;
    if (AssignedID != 0)
    {
        for(size_t i = 0; i < IDs.size(); i++){
            if(AssignedID == IDs[i]){
                IDs.erase(IDs.begin()+i);
                DeletedIDs.emplace_back(AssignedID);
                AssignedID = 0;
                if(DeletedIDs.size() > 1){
                    needsToSort = true;
                }
            }
        }
        needsToSort = true;
    }
    if(needsToSort){
        for(size_t i = 0; i < DeletedIDs.size(); i++){
            for(size_t j = 0; j < DeletedIDs.size(); j++){
                if(i < j){
                    if(DeletedIDs[i] > DeletedIDs[j]){
                        temp = DeletedIDs[i];
                        DeletedIDs[i] = DeletedIDs[j];
                        DeletedIDs[j] = temp;
                    }
                }else{
                    if(DeletedIDs[i] < DeletedIDs[j]){
                        temp = DeletedIDs[i];
                        DeletedIDs[i] = DeletedIDs[j];
                        DeletedIDs[j] = temp;
                    }
                }
            }
        }
        needsToSort = false;
    }
    
    if(DeletedIDs.size() > 0){
        IDs.emplace_back(DeletedIDs[0]);
        DeletedIDs.erase(DeletedIDs.begin()+0);
        AssignedID = IDs[IDs.size()-1];
        return;
    }
    if(IDs.size() > 0){
        IDs.emplace_back(IDs[IDs.size()-1]+1);
        AssignedID = IDs[IDs.size()-1];
    }
    else{
        IDs.emplace_back(1);
        AssignedID = IDs[IDs.size()-1];
    }

}

template struct IDBinder<Shader>;
template struct IDBinder<GameObject>;
template struct IDBinder<Texture>;