# Lesson 84: Project 2 - Multithreaded Game Engine (Part 1)

**Duration**: 10 hours
**Difficulty**: Expert
**Project Type**: Real-World Game Engine Architecture
**Prerequisites**: Lessons 1-83, Advanced OOP, Threading, Graphics Fundamentals

## Table of Contents
1. Game Engine Architecture Overview
2. Entity Component System (ECS)
3. Scene Graph Implementation
4. Resource Management System
5. Threading Architecture
6. Input System
7. Time Management
8. Physics Integration Foundation
9. Complete Core Engine Implementation
10. Testing and Benchmarking

---

## 1. Game Engine Architecture Overview

### High-Level Architecture

```
┌────────────────────────────────────────────────────┐
│                 Game Application                   │
└──────────────────┬─────────────────────────────────┘
                   │
┌──────────────────┴─────────────────────────────────┐
│              Game Engine Core                      │
│  ┌──────────────┐  ┌──────────────┐               │
│  │     ECS      │  │ Scene Graph  │               │
│  └──────────────┘  └──────────────┘               │
│  ┌──────────────┐  ┌──────────────┐               │
│  │   Physics    │  │   Rendering  │               │
│  └──────────────┘  └──────────────┘               │
│  ┌──────────────┐  ┌──────────────┐               │
│  │    Audio     │  │    Input     │               │
│  └──────────────┘  └──────────────┘               │
└────────────────────────────────────────────────────┘
         │                    │                │
    ┌────┴────┐         ┌─────┴─────┐    ┌────┴────┐
    │Platform │         │  Graphics │    │  Audio  │
    │  Layer  │         │    API    │    │   API   │
    └─────────┘         └───────────┘    └─────────┘
```

```cpp
#include <iostream>
#include <memory>
#include <vector>
#include <unordered_map>
#include <string>
#include <chrono>
#include <thread>
#include <mutex>
#include <atomic>
#include <functional>
#include <queue>
#include <typeindex>
#include <cmath>

namespace GameEngine {

    // Forward declarations
    class Entity;
    class Component;
    class System;
    class Scene;
    class Engine;

    // Common types
    using EntityID = uint64_t;
    using ComponentID = uint64_t;

} // namespace GameEngine
```

---

## 2. Entity Component System (ECS)

### Component Base Class

```cpp
namespace GameEngine {

    // Component interface
    class Component {
    protected:
        EntityID ownerEntity;
        bool enabled;

    public:
        Component() : ownerEntity(0), enabled(true) {}
        virtual ~Component() = default;

        virtual void initialize() {}
        virtual void update(float deltaTime) {}
        virtual void destroy() {}

        void setOwner(EntityID id) { ownerEntity = id; }
        EntityID getOwner() const { return ownerEntity; }

        void setEnabled(bool state) { enabled = state; }
        bool isEnabled() const { return enabled; }

        virtual std::string getTypeName() const = 0;
    };

    // Transform Component
    struct Vector3 {
        float x, y, z;

        Vector3() : x(0), y(0), z(0) {}
        Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}

        Vector3 operator+(const Vector3& other) const {
            return Vector3(x + other.x, y + other.y, z + other.z);
        }

        Vector3 operator-(const Vector3& other) const {
            return Vector3(x - other.x, y - other.y, z - other.z);
        }

        Vector3 operator*(float scalar) const {
            return Vector3(x * scalar, y * scalar, z * scalar);
        }

        float magnitude() const {
            return std::sqrt(x * x + y * y + z * z);
        }

        Vector3 normalized() const {
            float mag = magnitude();
            if (mag > 0) {
                return Vector3(x / mag, y / mag, z / mag);
            }
            return Vector3();
        }
    };

    struct Quaternion {
        float x, y, z, w;

        Quaternion() : x(0), y(0), z(0), w(1) {}
        Quaternion(float _x, float _y, float _z, float _w)
            : x(_x), y(_y), z(_z), w(_w) {}

        static Quaternion fromEuler(float pitch, float yaw, float roll) {
            float cy = std::cos(yaw * 0.5f);
            float sy = std::sin(yaw * 0.5f);
            float cp = std::cos(pitch * 0.5f);
            float sp = std::sin(pitch * 0.5f);
            float cr = std::cos(roll * 0.5f);
            float sr = std::sin(roll * 0.5f);

            Quaternion q;
            q.w = cr * cp * cy + sr * sp * sy;
            q.x = sr * cp * cy - cr * sp * sy;
            q.y = cr * sp * cy + sr * cp * sy;
            q.z = cr * cp * sy - sr * sp * cy;
            return q;
        }
    };

    class TransformComponent : public Component {
    private:
        Vector3 position;
        Quaternion rotation;
        Vector3 scale;
        Vector3 localPosition;
        Quaternion localRotation;
        Vector3 localScale;

        EntityID parentEntity;
        std::vector<EntityID> children;

    public:
        TransformComponent()
            : position(0, 0, 0), rotation(), scale(1, 1, 1),
              localPosition(0, 0, 0), localRotation(), localScale(1, 1, 1),
              parentEntity(0) {}

        std::string getTypeName() const override { return "Transform"; }

        // Position
        void setPosition(const Vector3& pos) { position = pos; }
        const Vector3& getPosition() const { return position; }

        void translate(const Vector3& delta) { position = position + delta; }

        // Rotation
        void setRotation(const Quaternion& rot) { rotation = rot; }
        const Quaternion& getRotation() const { return rotation; }

        void rotate(const Quaternion& delta) {
            // Quaternion multiplication for rotation composition
            rotation = delta;
        }

        // Scale
        void setScale(const Vector3& scl) { scale = scl; }
        const Vector3& getScale() const { return scale; }

        // Hierarchy
        void setParent(EntityID parent) { parentEntity = parent; }
        EntityID getParent() const { return parentEntity; }

        void addChild(EntityID child) { children.push_back(child); }
        const std::vector<EntityID>& getChildren() const { return children; }

        // Direction vectors
        Vector3 forward() const {
            return Vector3(0, 0, 1); // Simplified
        }

        Vector3 right() const {
            return Vector3(1, 0, 0); // Simplified
        }

        Vector3 up() const {
            return Vector3(0, 1, 0); // Simplified
        }
    };

    // Mesh Renderer Component
    class MeshRendererComponent : public Component {
    private:
        std::string meshName;
        std::string materialName;
        bool visible;
        int renderLayer;

    public:
        MeshRendererComponent()
            : meshName(""), materialName(""), visible(true), renderLayer(0) {}

        MeshRendererComponent(const std::string& mesh, const std::string& material)
            : meshName(mesh), materialName(material), visible(true), renderLayer(0) {}

        std::string getTypeName() const override { return "MeshRenderer"; }

        void setMesh(const std::string& mesh) { meshName = mesh; }
        const std::string& getMesh() const { return meshName; }

        void setMaterial(const std::string& material) { materialName = material; }
        const std::string& getMaterial() const { return materialName; }

        void setVisible(bool vis) { visible = vis; }
        bool isVisible() const { return visible; }

        void setRenderLayer(int layer) { renderLayer = layer; }
        int getRenderLayer() const { return renderLayer; }

        void render() {
            if (visible && enabled) {
                // Actual rendering would happen here
                // std::cout << "Rendering mesh: " << meshName << "\n";
            }
        }
    };

    // Rigidbody Component
    class RigidbodyComponent : public Component {
    private:
        Vector3 velocity;
        Vector3 angularVelocity;
        float mass;
        float drag;
        float angularDrag;
        bool useGravity;
        bool isKinematic;

    public:
        RigidbodyComponent()
            : velocity(0, 0, 0), angularVelocity(0, 0, 0),
              mass(1.0f), drag(0.0f), angularDrag(0.05f),
              useGravity(true), isKinematic(false) {}

        std::string getTypeName() const override { return "Rigidbody"; }

        void update(float deltaTime) override {
            if (!enabled || isKinematic) return;

            // Apply gravity
            if (useGravity) {
                velocity.y -= 9.81f * deltaTime;
            }

            // Apply drag
            velocity = velocity * (1.0f - drag * deltaTime);
            angularVelocity = angularVelocity * (1.0f - angularDrag * deltaTime);
        }

        void addForce(const Vector3& force) {
            if (mass > 0) {
                velocity = velocity + (force * (1.0f / mass));
            }
        }

        void addTorque(const Vector3& torque) {
            if (mass > 0) {
                angularVelocity = angularVelocity + (torque * (1.0f / mass));
            }
        }

        // Getters and setters
        void setVelocity(const Vector3& vel) { velocity = vel; }
        const Vector3& getVelocity() const { return velocity; }

        void setMass(float m) { mass = m; }
        float getMass() const { return mass; }

        void setGravity(bool gravity) { useGravity = gravity; }
        bool hasGravity() const { return useGravity; }

        void setKinematic(bool kinematic) { isKinematic = kinematic; }
        bool getIsKinematic() const { return isKinematic; }
    };

    // Script Component (for custom behavior)
    class ScriptComponent : public Component {
    private:
        std::function<void(float)> updateFunc;
        std::function<void()> startFunc;

    public:
        ScriptComponent() {}

        std::string getTypeName() const override { return "Script"; }

        void setStartFunction(std::function<void()> func) {
            startFunc = func;
        }

        void setUpdateFunction(std::function<void(float)> func) {
            updateFunc = func;
        }

        void initialize() override {
            if (startFunc) {
                startFunc();
            }
        }

        void update(float deltaTime) override {
            if (enabled && updateFunc) {
                updateFunc(deltaTime);
            }
        }
    };

} // namespace GameEngine
```

---

## 3. Entity System

```cpp
namespace GameEngine {

    class Entity {
    private:
        EntityID id;
        std::string name;
        bool active;
        std::unordered_map<std::type_index, std::shared_ptr<Component>> components;

        static std::atomic<EntityID> nextID;

    public:
        Entity() : id(nextID++), name("Entity_" + std::to_string(id)), active(true) {}

        explicit Entity(const std::string& entityName)
            : id(nextID++), name(entityName), active(true) {}

        EntityID getID() const { return id; }
        const std::string& getName() const { return name; }
        void setName(const std::string& newName) { name = newName; }

        bool isActive() const { return active; }
        void setActive(bool state) { active = state; }

        // Add component
        template<typename T, typename... Args>
        T* addComponent(Args&&... args) {
            static_assert(std::is_base_of<Component, T>::value,
                          "T must derive from Component");

            auto component = std::make_shared<T>(std::forward<Args>(args)...);
            component->setOwner(id);
            components[std::type_index(typeid(T))] = component;
            component->initialize();
            return component.get();
        }

        // Get component
        template<typename T>
        T* getComponent() {
            auto it = components.find(std::type_index(typeid(T)));
            if (it != components.end()) {
                return static_cast<T*>(it->second.get());
            }
            return nullptr;
        }

        // Has component
        template<typename T>
        bool hasComponent() const {
            return components.find(std::type_index(typeid(T))) != components.end();
        }

        // Remove component
        template<typename T>
        void removeComponent() {
            auto it = components.find(std::type_index(typeid(T)));
            if (it != components.end()) {
                it->second->destroy();
                components.erase(it);
            }
        }

        // Update all components
        void update(float deltaTime) {
            if (!active) return;

            for (auto& pair : components) {
                if (pair.second->isEnabled()) {
                    pair.second->update(deltaTime);
                }
            }
        }

        // Get all components
        std::vector<Component*> getAllComponents() {
            std::vector<Component*> result;
            for (auto& pair : components) {
                result.push_back(pair.second.get());
            }
            return result;
        }
    };

    std::atomic<EntityID> Entity::nextID{1};

} // namespace GameEngine
```

---

## 4. Scene Management

```cpp
namespace GameEngine {

    class Scene {
    private:
        std::string name;
        std::unordered_map<EntityID, std::unique_ptr<Entity>> entities;
        std::vector<EntityID> rootEntities; // Entities with no parent

        bool active;
        bool loaded;

    public:
        explicit Scene(const std::string& sceneName)
            : name(sceneName), active(false), loaded(false) {}

        const std::string& getName() const { return name; }

        // Entity management
        Entity* createEntity(const std::string& name = "") {
            auto entity = std::make_unique<Entity>(name);
            EntityID id = entity->getID();
            Entity* ptr = entity.get();
            entities[id] = std::move(entity);
            rootEntities.push_back(id);
            return ptr;
        }

        Entity* getEntity(EntityID id) {
            auto it = entities.find(id);
            return (it != entities.end()) ? it->second.get() : nullptr;
        }

        Entity* findEntityByName(const std::string& name) {
            for (auto& pair : entities) {
                if (pair.second->getName() == name) {
                    return pair.second.get();
                }
            }
            return nullptr;
        }

        void destroyEntity(EntityID id) {
            auto it = entities.find(id);
            if (it != entities.end()) {
                // Remove from root entities
                rootEntities.erase(
                    std::remove(rootEntities.begin(), rootEntities.end(), id),
                    rootEntities.end()
                );
                entities.erase(it);
            }
        }

        // Update all entities
        void update(float deltaTime) {
            if (!active) return;

            for (auto& pair : entities) {
                if (pair.second->isActive()) {
                    pair.second->update(deltaTime);
                }
            }
        }

        // Scene state
        void setActive(bool state) { active = state; }
        bool isActive() const { return active; }

        void load() {
            if (!loaded) {
                // Load scene resources
                loaded = true;
                std::cout << "Scene '" << name << "' loaded\n";
            }
        }

        void unload() {
            if (loaded) {
                // Unload scene resources
                entities.clear();
                rootEntities.clear();
                loaded = false;
                std::cout << "Scene '" << name << "' unloaded\n";
            }
        }

        // Statistics
        size_t getEntityCount() const { return entities.size(); }

        void printHierarchy() const {
            std::cout << "\nScene: " << name << "\n";
            std::cout << "Entities: " << entities.size() << "\n";
            std::cout << "Root Entities: " << rootEntities.size() << "\n";

            for (EntityID id : rootEntities) {
                auto it = entities.find(id);
                if (it != entities.end()) {
                    printEntityHierarchy(it->second.get(), 0);
                }
            }
        }

    private:
        void printEntityHierarchy(const Entity* entity, int indent) const {
            std::string indentStr(indent * 2, ' ');
            std::cout << indentStr << "- " << entity->getName()
                      << " (ID: " << entity->getID() << ")\n";

            // Print components
            auto components = const_cast<Entity*>(entity)->getAllComponents();
            for (auto* comp : components) {
                std::cout << indentStr << "  [" << comp->getTypeName() << "]\n";
            }

            // Print children
            auto* transform = const_cast<Entity*>(entity)->getComponent<TransformComponent>();
            if (transform) {
                for (EntityID childID : transform->getChildren()) {
                    auto it = entities.find(childID);
                    if (it != entities.end()) {
                        printEntityHierarchy(it->second.get(), indent + 1);
                    }
                }
            }
        }
    };

} // namespace GameEngine
```

---

## 5. Resource Management System

```cpp
namespace GameEngine {

    // Resource base class
    class Resource {
    protected:
        std::string name;
        std::string path;
        bool loaded;

    public:
        Resource(const std::string& resourceName, const std::string& resourcePath)
            : name(resourceName), path(resourcePath), loaded(false) {}

        virtual ~Resource() = default;

        virtual bool load() = 0;
        virtual void unload() = 0;

        const std::string& getName() const { return name; }
        const std::string& getPath() const { return path; }
        bool isLoaded() const { return loaded; }
    };

    // Mesh resource
    class Mesh : public Resource {
    private:
        std::vector<Vector3> vertices;
        std::vector<uint32_t> indices;
        size_t vertexCount;
        size_t triangleCount;

    public:
        Mesh(const std::string& name, const std::string& path)
            : Resource(name, path), vertexCount(0), triangleCount(0) {}

        bool load() override {
            if (loaded) return true;

            // Simulate loading mesh data
            // In real implementation: parse OBJ/FBX file
            std::cout << "Loading mesh: " << name << " from " << path << "\n";

            // Example: create a simple cube
            vertices = {
                Vector3(-1, -1, -1), Vector3(1, -1, -1),
                Vector3(1, 1, -1), Vector3(-1, 1, -1),
                Vector3(-1, -1, 1), Vector3(1, -1, 1),
                Vector3(1, 1, 1), Vector3(-1, 1, 1)
            };

            indices = {
                0, 1, 2, 2, 3, 0, // Front
                4, 5, 6, 6, 7, 4, // Back
                // ... more faces
            };

            vertexCount = vertices.size();
            triangleCount = indices.size() / 3;
            loaded = true;

            return true;
        }

        void unload() override {
            vertices.clear();
            indices.clear();
            loaded = false;
        }

        const std::vector<Vector3>& getVertices() const { return vertices; }
        const std::vector<uint32_t>& getIndices() const { return indices; }
        size_t getVertexCount() const { return vertexCount; }
        size_t getTriangleCount() const { return triangleCount; }
    };

    // Material resource
    class Material : public Resource {
    private:
        Vector3 albedoColor;
        float metallic;
        float roughness;
        std::string albedoTexture;
        std::string normalTexture;

    public:
        Material(const std::string& name, const std::string& path)
            : Resource(name, path),
              albedoColor(1, 1, 1),
              metallic(0.0f),
              roughness(0.5f) {}

        bool load() override {
            if (loaded) return true;

            std::cout << "Loading material: " << name << " from " << path << "\n";

            // Load material properties
            loaded = true;
            return true;
        }

        void unload() override {
            loaded = false;
        }

        void setAlbedoColor(const Vector3& color) { albedoColor = color; }
        const Vector3& getAlbedoColor() const { return albedoColor; }

        void setMetallic(float m) { metallic = m; }
        float getMetallic() const { return metallic; }

        void setRoughness(float r) { roughness = r; }
        float getRoughness() const { return roughness; }
    };

    // Resource Manager
    template<typename T>
    class ResourceManager {
    private:
        std::unordered_map<std::string, std::shared_ptr<T>> resources;
        std::mutex mutex;

    public:
        std::shared_ptr<T> load(const std::string& name, const std::string& path) {
            std::lock_guard<std::mutex> lock(mutex);

            // Check if already loaded
            auto it = resources.find(name);
            if (it != resources.end()) {
                return it->second;
            }

            // Load new resource
            auto resource = std::make_shared<T>(name, path);
            if (resource->load()) {
                resources[name] = resource;
                return resource;
            }

            return nullptr;
        }

        std::shared_ptr<T> get(const std::string& name) {
            std::lock_guard<std::mutex> lock(mutex);

            auto it = resources.find(name);
            if (it != resources.end()) {
                return it->second;
            }
            return nullptr;
        }

        void unload(const std::string& name) {
            std::lock_guard<std::mutex> lock(mutex);

            auto it = resources.find(name);
            if (it != resources.end()) {
                it->second->unload();
                resources.erase(it);
            }
        }

        void unloadAll() {
            std::lock_guard<std::mutex> lock(mutex);

            for (auto& pair : resources) {
                pair.second->unload();
            }
            resources.clear();
        }

        size_t getResourceCount() const {
            return resources.size();
        }
    };

} // namespace GameEngine
```

---

## 6. Threading Architecture

```cpp
namespace GameEngine {

    // Job system for parallel task execution
    class Job {
    public:
        virtual ~Job() = default;
        virtual void execute() = 0;
        virtual std::string getName() const = 0;
    };

    class LambdaJob : public Job {
    private:
        std::function<void()> func;
        std::string name;

    public:
        LambdaJob(std::function<void()> f, const std::string& jobName = "Lambda")
            : func(f), name(jobName) {}

        void execute() override {
            func();
        }

        std::string getName() const override {
            return name;
        }
    };

    class JobSystem {
    private:
        std::vector<std::thread> workers;
        std::queue<std::unique_ptr<Job>> jobQueue;
        std::mutex queueMutex;
        std::condition_variable condition;
        std::atomic<bool> stopping;
        std::atomic<size_t> activeJobs;

    public:
        explicit JobSystem(size_t numThreads = 0) : stopping(false), activeJobs(0) {
            if (numThreads == 0) {
                numThreads = std::thread::hardware_concurrency();
            }

            for (size_t i = 0; i < numThreads; ++i) {
                workers.emplace_back(&JobSystem::workerThread, this);
            }

            std::cout << "Job System initialized with " << numThreads << " threads\n";
        }

        ~JobSystem() {
            shutdown();
        }

        void submitJob(std::unique_ptr<Job> job) {
            {
                std::lock_guard<std::mutex> lock(queueMutex);
                jobQueue.push(std::move(job));
            }
            condition.notify_one();
        }

        void submitLambda(std::function<void()> func, const std::string& name = "Lambda") {
            submitJob(std::make_unique<LambdaJob>(func, name));
        }

        void shutdown() {
            stopping.store(true);
            condition.notify_all();

            for (auto& worker : workers) {
                if (worker.joinable()) {
                    worker.join();
                }
            }
            workers.clear();
        }

        size_t getActiveJobCount() const {
            return activeJobs.load();
        }

    private:
        void workerThread() {
            while (!stopping.load()) {
                std::unique_ptr<Job> job;

                {
                    std::unique_lock<std::mutex> lock(queueMutex);
                    condition.wait(lock, [this] {
                        return stopping.load() || !jobQueue.empty();
                    });

                    if (stopping.load() && jobQueue.empty()) {
                        return;
                    }

                    if (!jobQueue.empty()) {
                        job = std::move(jobQueue.front());
                        jobQueue.pop();
                    }
                }

                if (job) {
                    activeJobs++;
                    try {
                        job->execute();
                    } catch (const std::exception& e) {
                        std::cerr << "Job exception: " << e.what() << "\n";
                    }
                    activeJobs--;
                }
            }
        }
    };

} // namespace GameEngine
```

---

## 7. Time Management

```cpp
namespace GameEngine {

    class Time {
    private:
        std::chrono::steady_clock::time_point startTime;
        std::chrono::steady_clock::time_point lastFrameTime;
        float deltaTime;
        float timeScale;
        uint64_t frameCount;
        float fps;
        float fpsUpdateInterval;
        float timeSinceLastFPSUpdate;
        uint64_t framesSinceLastFPSUpdate;

    public:
        Time()
            : startTime(std::chrono::steady_clock::now()),
              lastFrameTime(startTime),
              deltaTime(0.0f),
              timeScale(1.0f),
              frameCount(0),
              fps(0.0f),
              fpsUpdateInterval(0.5f),
              timeSinceLastFPSUpdate(0.0f),
              framesSinceLastFPSUpdate(0) {}

        void update() {
            auto currentTime = std::chrono::steady_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
                currentTime - lastFrameTime).count();

            deltaTime = (duration / 1000000.0f) * timeScale;
            lastFrameTime = currentTime;
            frameCount++;

            // Update FPS
            timeSinceLastFPSUpdate += deltaTime / timeScale; // Unscaled delta
            framesSinceLastFPSUpdate++;

            if (timeSinceLastFPSUpdate >= fpsUpdateInterval) {
                fps = framesSinceLastFPSUpdate / timeSinceLastFPSUpdate;
                timeSinceLastFPSUpdate = 0.0f;
                framesSinceLastFPSUpdate = 0;
            }
        }

        float getDeltaTime() const { return deltaTime; }
        float getUnscaledDeltaTime() const { return deltaTime / timeScale; }
        float getTimeScale() const { return timeScale; }
        void setTimeScale(float scale) { timeScale = std::max(0.0f, scale); }

        uint64_t getFrameCount() const { return frameCount; }
        float getFPS() const { return fps; }

        float getTimeSinceStart() const {
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::steady_clock::now() - startTime).count();
            return duration / 1000.0f;
        }
    };

} // namespace GameEngine
```

---

## 8. Core Game Engine

```cpp
namespace GameEngine {

    class Engine {
    private:
        // Core systems
        std::unique_ptr<JobSystem> jobSystem;
        std::unique_ptr<Time> time;
        std::unique_ptr<Scene> activeScene;

        // Resource managers
        ResourceManager<Mesh> meshManager;
        ResourceManager<Material> materialManager;

        // Engine state
        std::atomic<bool> running;
        bool initialized;

        // Configuration
        int targetFPS;
        bool vsync;

    public:
        Engine()
            : initialized(false), running(false), targetFPS(60), vsync(false) {}

        ~Engine() {
            shutdown();
        }

        bool initialize() {
            if (initialized) return true;

            std::cout << "Initializing Game Engine...\n";

            // Initialize subsystems
            jobSystem = std::make_unique<JobSystem>();
            time = std::make_unique<Time>();

            // Create default scene
            activeScene = std::make_unique<Scene>("DefaultScene");
            activeScene->load();
            activeScene->setActive(true);

            initialized = true;
            std::cout << "Game Engine initialized successfully\n";

            return true;
        }

        void run() {
            if (!initialized) {
                std::cerr << "Engine not initialized!\n";
                return;
            }

            running.store(true);

            std::cout << "Starting game loop...\n";

            float targetFrameTime = 1.0f / targetFPS;

            while (running.load()) {
                auto frameStart = std::chrono::steady_clock::now();

                // Update time
                time->update();
                float deltaTime = time->getDeltaTime();

                // Update scene
                if (activeScene && activeScene->isActive()) {
                    activeScene->update(deltaTime);
                }

                // Render (in actual engine, this would be separate)
                render();

                // Frame timing
                auto frameEnd = std::chrono::steady_clock::now();
                auto frameDuration = std::chrono::duration_cast<std::chrono::milliseconds>(
                    frameEnd - frameStart).count();

                // Sleep to maintain target FPS
                if (!vsync && frameDuration < targetFrameTime * 1000) {
                    std::this_thread::sleep_for(
                        std::chrono::milliseconds(
                            static_cast<int>(targetFrameTime * 1000 - frameDuration)
                        )
                    );
                }

                // Print stats occasionally
                if (time->getFrameCount() % 60 == 0) {
                    std::cout << "FPS: " << time->getFPS()
                              << " | Frame: " << time->getFrameCount()
                              << " | Entities: " << activeScene->getEntityCount()
                              << "\n";
                }
            }
        }

        void stop() {
            running.store(false);
        }

        void shutdown() {
            if (!initialized) return;

            std::cout << "Shutting down Game Engine...\n";

            running.store(false);

            if (activeScene) {
                activeScene->unload();
            }

            jobSystem->shutdown();

            meshManager.unloadAll();
            materialManager.unloadAll();

            initialized = false;
            std::cout << "Game Engine shutdown complete\n";
        }

        // Accessors
        Scene* getActiveScene() { return activeScene.get(); }
        JobSystem* getJobSystem() { return jobSystem.get(); }
        Time* getTime() { return time.get(); }

        ResourceManager<Mesh>& getMeshManager() { return meshManager; }
        ResourceManager<Material>& getMaterialManager() { return materialManager; }

        void setTargetFPS(int fps) { targetFPS = fps; }
        void setVSync(bool enable) { vsync = enable; }

    private:
        void render() {
            // In actual engine: rendering logic here
            // For now, just count renderable entities
            if (!activeScene) return;

            // Would iterate over all entities with MeshRenderer
            // and submit them to the rendering system
        }
    };

} // namespace GameEngine
```

---

## 9. Demo Application

```cpp
using namespace GameEngine;

int main() {
    std::cout << "=== Multithreaded Game Engine Demo ===\n\n";

    // Create and initialize engine
    Engine engine;
    if (!engine.initialize()) {
        std::cerr << "Failed to initialize engine\n";
        return 1;
    }

    // Get active scene
    Scene* scene = engine.getActiveScene();

    // Create game objects
    std::cout << "\nCreating game objects...\n";

    // Player entity
    Entity* player = scene->createEntity("Player");
    auto* playerTransform = player->addComponent<TransformComponent>();
    playerTransform->setPosition(Vector3(0, 0, 0));

    auto* playerRenderer = player->addComponent<MeshRendererComponent>("PlayerMesh", "PlayerMaterial");
    auto* playerRigidbody = player->addComponent<RigidbodyComponent>();
    playerRigidbody->setMass(75.0f);

    // Enemy entities
    for (int i = 0; i < 5; ++i) {
        Entity* enemy = scene->createEntity("Enemy_" + std::to_string(i));
        auto* transform = enemy->addComponent<TransformComponent>();
        transform->setPosition(Vector3(i * 2.0f, 0, 10));

        enemy->addComponent<MeshRendererComponent>("EnemyMesh", "EnemyMaterial");
        auto* rb = enemy->addComponent<RigidbodyComponent>();
        rb->setMass(50.0f);

        // Add simple AI script
        auto* script = enemy->addComponent<ScriptComponent>();
        script->setUpdateFunction([transform, i](float deltaTime) {
            // Simple patrol movement
            float speed = 2.0f;
            Vector3 pos = transform->getPosition();
            pos.x = std::sin(engine.getTime()->getTimeSinceStart() + i) * 5.0f;
            transform->setPosition(pos);
        });
    }

    // Environment entities
    Entity* ground = scene->createEntity("Ground");
    auto* groundTransform = ground->addComponent<TransformComponent>();
    groundTransform->setPosition(Vector3(0, -1, 0));
    groundTransform->setScale(Vector3(100, 1, 100));
    ground->addComponent<MeshRendererComponent>("Plane", "GroundMaterial");

    // Print scene hierarchy
    scene->printHierarchy();

    // Load resources
    std::cout << "\nLoading resources...\n";
    engine.getMeshManager().load("PlayerMesh", "assets/player.obj");
    engine.getMeshManager().load("EnemyMesh", "assets/enemy.obj");
    engine.getMeshManager().load("Plane", "assets/plane.obj");

    engine.getMaterialManager().load("PlayerMaterial", "assets/player.mat");
    engine.getMaterialManager().load("EnemyMaterial", "assets/enemy.mat");
    engine.getMaterialManager().load("GroundMaterial", "assets/ground.mat");

    // Configure engine
    engine.setTargetFPS(60);
    engine.setVSync(false);

    // Run for a limited time for demo
    std::thread engineThread([&engine]() {
        engine.run();
    });

    // Let it run for 5 seconds
    std::this_thread::sleep_for(std::chrono::seconds(5));

    // Stop engine
    engine.stop();
    engineThread.join();

    std::cout << "\n=== Demo Complete ===\n";

    return 0;
}
```

---

## Summary of Part 1

You've implemented:

1. ✅ **Entity Component System**: Flexible, data-oriented architecture
2. ✅ **Scene Management**: Hierarchical scene graphs
3. ✅ **Component System**: Transform, Renderer, Physics, Script
4. ✅ **Resource Management**: Efficient asset loading/unloading
5. ✅ **Job System**: Multithreaded task execution
6. ✅ **Time Management**: Frame timing and FPS control
7. ✅ **Core Engine Loop**: Complete game loop implementation

**Next Lesson**: Part 2 will add rendering system, physics simulation, collision detection, and advanced optimizations!

---

**Compilation**:
```bash
g++ -std=c++17 -O3 -pthread game_engine_part1.cpp -o game_engine
./game_engine
```
