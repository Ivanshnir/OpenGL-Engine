#pragma once
class MainWorkMethods{
    public:
    virtual void Awake() = 0;
    virtual void Update() = 0;
    virtual void Start() = 0;
};

template<typename T>
class MonoImplemantation : MainWorkMethods{
    
};