#pragma once
#include <vector>
#include "../object/object.hpp"
#include <glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <set>
class Script;
class Manager
{
    public:
    Manager& operator=(const Manager &) = delete;
    Manager& operator=(Manager &&) = delete;
    Manager(const Manager &) = delete;
    Manager(Manager &&) = delete;
    virtual void Add(Object *) = 0;
    virtual void Remove(Object *) = 0;
    virtual ~Manager() = default;
    protected:
    Manager() = default;
};
class Object_Manager : public Manager{
    public:
    static Object_Manager& GET() { static Object_Manager instance; return instance; };
    void Add(Object*) override;
    void Remove(Object*) override;
    Object & operator[](size_t);
    size_t size();
    ~Object_Manager() override { objects_pool.clear(); }
    private:
    static inline std::vector<Object *> objects_pool;
};
class Render_Manager : public Manager{
    public:
    static Render_Manager& GET() { static Render_Manager instance; return instance; };
    void Add(Object*) override;
    void Remove(Object*) override;
    void InitGameWindows();
    void SetFpsCap(unsigned int fps);
    bool RenderGameWindow(GLFWwindow *);
    static float deltaTime();
    ~Render_Manager() override { objects_pool.clear(); }
    private:
    static inline unsigned int _fps_cap = 60;
    static inline bool _GameWindows_is_init = false;
    static inline std::vector<Script *> scripts_pool;
    static inline std::vector<Object *> objects_pool;
};