// #include "GameLoopsRegistry.hpp"
// #include "../../core/Debugger/debug.hpp"
// bool Registry::CreateToken(void(*func_to_send)(), const char mark){
//     LOG("Register - " << mark << " func", TRACE);
//     switch (mark)
//     {
//     case 'S':
//         Starts_Funcs.emplace_back(func_to_send);
//         return true;
//         break;
//     case 'A':
//         Awake_Funcs.emplace_back(func_to_send);
//         return true;
//         break;
//     case 'U':
//         Update_Funcs.emplace_back(func_to_send);
//         return true;
//         break;    
//     default:
//         break;
//     }
    
//     return false;
// }
// Registry & Registry::GET(){
//     static Registry instance;
//     return instance;
// }
// template<typename T>
// MonoImplemantation<T>::T instance{};
// template<typename T>
// bool MonoImplemantation<T>::LinkToRegistry(){
//     LOG("Here", DEBUG);
//     void(T::*start_func)();
//     void(T::*upd_func)();
//     void(T::*awake_func)();
//     start_func = Start;
//     upd_func= Update;
//     awake_func = Awake;
//     bool CanUsed = false;
//     Registry& rg = Registry::GET();
//     if(rg.CreateToken(start_func, 'S')){
//         if(rg.CreateToken(upd_func, 'U')){
//             if(rg.CreateToken(awake_func, 'A')){
//                 CanUsed = true;
//             }
//         }
//     } 
//     return CanUsed;       
// }