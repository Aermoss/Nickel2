#include <nickel2/scene.hpp>

namespace nickel2 {
    Scene::Scene() {

    }

    Scene::~Scene() {

    }

    std::vector <Model*>& Scene::getObjects() {
        return objects;
    }

    std::vector <Light>& Scene::getLights() {
        return lights;
    }

    void Scene::submit(Model* object) {
        objects.push_back(object);
    }

    void Scene::submit(Light light) {
        lights.push_back(light);
    }

    void Scene::destroy() {
        for (Model* object : objects) {
            object->destroy();
        } objects.clear();
    }
}