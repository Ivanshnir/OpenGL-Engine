#pragma once
#include <vector>
#include <memory>

class Shader;
class Texture;
class GameObject;

class Render_Manager{
    public:
    void RemoveAll();
    template <typename T>
    void Render_Add(T *);
    template <typename T>
    void Render_Remove(T *);
    Render_Manager(const Render_Manager&) = delete;
    Render_Manager& operator=(const Render_Manager&) = delete;
    Render_Manager(Render_Manager &&) = delete;
    Render_Manager& operator=(Render_Manager&&) = delete;
    static Render_Manager & GET();
    void FrameRender();
    private:
    std::vector<GameObject*> objects_pool;
    std::vector<Shader*> shaders_pool;
    std::vector<Texture*> textures_pool;
    Render_Manager() {};
    void _obj_render(GameObject &);
};