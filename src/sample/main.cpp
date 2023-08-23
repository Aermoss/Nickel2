#include <iostream>
#include <vector>

#include <bullet/btBulletDynamicsCommon.h>

#include <nickel2/nickel2.hpp>

int main() {
    nickel2::Context context;
    nickel2::Window window(1200, 600, "Nickel2", nickel2::Color(0, 0, 0), true);

    nickel2::Renderer renderer(&window, "res/hdr_textures/train_station.hdr");
    nickel2::FirstPersonCamera camera(&window, 90.0f, 100.0f, 0.01f, 1000.0f);

    nickel2::Scene scene;

    scene.lights = {
        nickel2::Light(glm::vec3(5.0f, 5.0f, 5.0f), glm::vec3(1.0f, 1.0f, 1.0f), 3.0f)
    };

    nickel2::Model model("res/cube.obj");
    model.transform->setPosition(glm::vec3(0.0f, 2.0f, 0.0f));
    model.transform->setRotation(glm::vec3(0.0f, 0.0f, 0.0f));
    model.transform->setScale(glm::vec3(1.0f, 1.0f, 1.0f));
    scene.submit(&model);

    nickel2::Model ground("res/cube.obj");
    ground.transform->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    ground.transform->setRotation(glm::vec3(0.0f, 0.0f, 0.0f));
    ground.transform->setScale(glm::vec3(10.0f, 1.0f, 10.0f));
    scene.submit(&ground);

    float fov = 45.0f;
    float lastScrollY = 0.0;
    float interpolatedPitch = 0.0f, interpolatedYaw = 0.0f;
    float realPitch = 0.0f, realYaw = 0.0f;

    nickel2::Scene* currentScene = &scene;
    nickel2::audio::Listener listener(&camera);
    nickel2::audio::Source source(&listener, "res/music.wav", true);
    source.play();

    btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
    btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);
    btBroadphaseInterface* overlappingPairCache = new btDbvtBroadphase();
    btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;
    btDiscreteDynamicsWorld* dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);
    dynamicsWorld->setGravity(btVector3(0.0f, -10.0f, 0.0f));

    btAlignedObjectArray <btCollisionShape*> collisionShapes;

    {
        btCollisionShape* groundShape = new btBoxShape(btVector3(btScalar(10.0f), btScalar(1.0f), btScalar(10.0f)));

        collisionShapes.push_back(groundShape);

        btTransform groundTransform;
        groundTransform.setIdentity();
        groundTransform.setOrigin(btVector3(0.0f, 0.0f, 0.0f));

        btScalar mass(0.0f);

        bool isDynamic = (mass != 0.0f);

        btVector3 localInertia(0.0f, 0.0f, 0.0f);

        if (isDynamic)
            groundShape->calculateLocalInertia(mass, localInertia);

        btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
        btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, groundShape, localInertia);
        btRigidBody* body = new btRigidBody(rbInfo);

        dynamicsWorld->addRigidBody(body);
    }

    btCollisionShape* colShape = new btBoxShape(btVector3(btScalar(1.0f), btScalar(1.0f), btScalar(1.0f)));
    collisionShapes.push_back(colShape);

    btTransform startTransform;
    startTransform.setIdentity();

    btScalar mass(1.0f);

    bool isDynamic = (mass != 0.0f);

    btVector3 localInertia(0.0f, 0.0f, 0.0f);

    if (isDynamic)
        colShape->calculateLocalInertia(mass, localInertia);

    startTransform.setOrigin(btVector3(0.0f, 100.0f, 0.0f));

    btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);
    btRigidBody* body = new btRigidBody(rbInfo);

    dynamicsWorld->addRigidBody(body);

    while (!window.shouldClose()) {
        context.pollEvents();
        context.makeCurrent();
        listener.update();
        window.update();
        window.clear();

        dynamicsWorld->stepSimulation(window.deltaTime, 10);

        btTransform trans;
        body->getMotionState()->getWorldTransform(trans);
        body->activate(true);
        model.transform->setPosition(glm::vec3(float(trans.getOrigin().getX()), float(trans.getOrigin().getY()), float(trans.getOrigin().getZ())));
        model.transform->setRotationQuat(glm::quat(float(trans.getRotation().getW()), float(trans.getRotation().getX()), float(trans.getRotation().getY()), float(trans.getRotation().getZ())));
        
        // btScalar angle = trans.getRotation().getAngle();
        // model.transform->setRotation(glm::degrees(glm::vec3(float(trans.getRotation().getAxis().getX() * angle), float(trans.getRotation().getAxis().getY() * angle), float(trans.getRotation().getAxis().getZ() * angle))));

        if (window.input->getKey(NICKEL2_KEY_RIGHT)) {
            body->applyCentralForce(btVector3(0.0f, 0.0f, 10.0f));
        }

        if (window.input->getKey(NICKEL2_KEY_LEFT)) {
            body->applyCentralForce(btVector3(0.0f, 0.0f, -10.0f));
        }

        if (window.input->getKey(NICKEL2_KEY_UP)) {
            body->applyCentralForce(btVector3(10.0f, 0.0f, 0.0f));
        }

        if (window.input->getKey(NICKEL2_KEY_DOWN)) {
            body->applyCentralForce(btVector3(-10.0f, 0.0f, 0.0f));
        }

        camera.pitch = realPitch;
        camera.yaw = realYaw;
        camera.processInputs();
        realPitch = camera.pitch;
        realYaw = camera.yaw;

        interpolatedPitch = glm::lerp(interpolatedPitch, realPitch, 0.2f);
        interpolatedYaw = glm::lerp(interpolatedYaw, realYaw, 0.2f);
        camera.pitch = interpolatedPitch;
        camera.yaw = interpolatedYaw;

        double change = lastScrollY - nickel2::input::scrollY;
        lastScrollY = nickel2::input::scrollY;

        fov = glm::clamp((fov += change * 5.0f), 10.0f, 110.0f);
        camera.fov = glm::lerp(camera.fov, fov, 0.15f);
        camera.sensitivity = camera.fov * 1.5f + 10.0f;

        if (window.input->getKey(NICKEL2_KEY_Q)) {
            renderer.removeHDRTexture();
        }
        
        if (window.input->getKey(NICKEL2_KEY_E)) {
            renderer.loadHDRTexture("res/hdr_textures/square_night.hdr");
        }

        if (window.input->getKey(NICKEL2_KEY_R)) {
            renderer.reloadShaders();
            std::cout << "shaders reloaded" << std::endl;
        }

        renderer.render(&camera, currentScene);
        window.swapBuffers();
    }

    for (int32_t i = dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--) {
        btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i];
        btRigidBody* body = btRigidBody::upcast(obj);

        if (body && body->getMotionState()) {
            delete body->getMotionState();
        }

        dynamicsWorld->removeCollisionObject(obj);
        delete obj;
    }

    for (uint32_t i = 0; i < collisionShapes.size(); i++) {
        btCollisionShape* shape = collisionShapes[i];
        collisionShapes[i] = 0;
        delete shape;
    }

    delete dynamicsWorld;
    delete solver;
    delete overlappingPairCache;
    delete dispatcher;
    delete collisionConfiguration;
    collisionShapes.clear();

    scene.destroy();
    camera.destroy();
    renderer.destroy();
    window.destroy();
    context.destroy();
    return 0;
}