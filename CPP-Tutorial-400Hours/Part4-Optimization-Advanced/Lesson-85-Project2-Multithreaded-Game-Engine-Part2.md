# Lesson 85: Project 2 - Multithreaded Game Engine (Part 2)

**Duration**: 10 hours
**Difficulty**: Expert
**Project Type**: Advanced Rendering & Physics Integration
**Prerequisites**: Lesson 84, Graphics Programming, Physics Fundamentals

## Table of Contents
1. Rendering System Architecture
2. Collision Detection System
3. Physics Integration
4. Particle System
5. Audio System
6. Event System
7. Performance Profiling
8. Memory Optimization
9. Complete Enhanced Engine
10. Advanced Techniques

---

## 1. Rendering System

```cpp
namespace GameEngine {
namespace Rendering {

    struct Vertex {
        Vector3 position;
        Vector3 normal;
        float u, v; // UV coordinates

        Vertex() : position(), normal(), u(0), v(0) {}
        Vertex(const Vector3& pos, const Vector3& norm, float texU, float texV)
            : position(pos), normal(norm), u(texU), v(texV) {}
    };

    class RenderCommand {
    public:
        virtual ~RenderCommand() = default;
        virtual void execute() = 0;
        virtual int getPriority() const { return 0; }
    };

    class DrawMeshCommand : public RenderCommand {
    private:
        std::shared_ptr<Mesh> mesh;
        std::shared_ptr<Material> material;
        TransformComponent* transform;

    public:
        DrawMeshCommand(std::shared_ptr<Mesh> m, std::shared_ptr<Material> mat, TransformComponent* t)
            : mesh(m), material(mat), transform(t) {}

        void execute() override {
            if (!mesh || !material || !transform) return;

            // Render mesh with material at transform position
            // In real engine: submit to graphics API
            // std::cout << "Rendering mesh at position: "
            //           << transform->getPosition().x << ", "
            //           << transform->getPosition().y << ", "
            //           << transform->getPosition().z << "\n";
        }

        int getPriority() const override {
            // Opaque objects first, then transparent
            return 0;
        }
    };

    class RenderQueue {
    private:
        std::vector<std::unique_ptr<RenderCommand>> commands;
        std::mutex mutex;

    public:
        void submit(std::unique_ptr<RenderCommand> command) {
            std::lock_guard<std::mutex> lock(mutex);
            commands.push_back(std::move(command));
        }

        void sort() {
            std::sort(commands.begin(), commands.end(),
                [](const std::unique_ptr<RenderCommand>& a,
                   const std::unique_ptr<RenderCommand>& b) {
                    return a->getPriority() < b->getPriority();
                });
        }

        void execute() {
            for (auto& command : commands) {
                command->execute();
            }
        }

        void clear() {
            commands.clear();
        }

        size_t getCommandCount() const { return commands.size(); }
    };

    class Camera {
    private:
        Vector3 position;
        Vector3 target;
        Vector3 up;
        float fov;
        float aspectRatio;
        float nearPlane;
        float farPlane;

    public:
        Camera()
            : position(0, 0, -10),
              target(0, 0, 0),
              up(0, 1, 0),
              fov(60.0f),
              aspectRatio(16.0f / 9.0f),
              nearPlane(0.1f),
              farPlane(1000.0f) {}

        void setPosition(const Vector3& pos) { position = pos; }
        const Vector3& getPosition() const { return position; }

        void setTarget(const Vector3& t) { target = t; }
        const Vector3& getTarget() const { return target; }

        void setFOV(float f) { fov = f; }
        float getFOV() const { return fov; }

        Vector3 getForward() const {
            return (target - position).normalized();
        }

        Vector3 getRight() const {
            Vector3 forward = getForward();
            // Cross product would be here
            return Vector3(1, 0, 0);
        }
    };

    class Renderer {
    private:
        RenderQueue renderQueue;
        std::unique_ptr<Camera> mainCamera;
        std::atomic<size_t> drawCalls{0};
        std::atomic<size_t> trianglesRendered{0};

    public:
        Renderer() {
            mainCamera = std::make_unique<Camera>();
        }

        Camera* getMainCamera() { return mainCamera.get(); }

        void beginFrame() {
            renderQueue.clear();
            drawCalls = 0;
            trianglesRendered = 0;
        }

        void submitMesh(std::shared_ptr<Mesh> mesh,
                       std::shared_ptr<Material> material,
                       TransformComponent* transform) {
            auto command = std::make_unique<DrawMeshCommand>(mesh, material, transform);
            renderQueue.submit(std::move(command));
        }

        void render() {
            // Sort render queue
            renderQueue.sort();

            // Execute all render commands
            renderQueue.execute();

            drawCalls = renderQueue.getCommandCount();
        }

        void endFrame() {
            // Finalize rendering
        }

        size_t getDrawCallCount() const { return drawCalls.load(); }
        size_t getTrianglesRendered() const { return trianglesRendered.load(); }
    };

} // namespace Rendering
} // namespace GameEngine
```

---

## 2. Collision Detection

```cpp
namespace GameEngine {
namespace Physics {

    struct AABB {
        Vector3 min;
        Vector3 max;

        AABB() : min(0, 0, 0), max(0, 0, 0) {}
        AABB(const Vector3& minimum, const Vector3& maximum)
            : min(minimum), max(maximum) {}

        bool intersects(const AABB& other) const {
            return (min.x <= other.max.x && max.x >= other.min.x) &&
                   (min.y <= other.max.y && max.y >= other.min.y) &&
                   (min.z <= other.max.z && max.z >= other.min.z);
        }

        Vector3 getCenter() const {
            return Vector3(
                (min.x + max.x) * 0.5f,
                (min.y + max.y) * 0.5f,
                (min.z + max.z) * 0.5f
            );
        }

        Vector3 getSize() const {
            return Vector3(
                max.x - min.x,
                max.y - min.y,
                max.z - min.z
            );
        }
    };

    class ColliderComponent : public Component {
    protected:
        AABB bounds;
        bool isTrigger;
        int layer;

    public:
        ColliderComponent() : isTrigger(false), layer(0) {}

        std::string getTypeName() const override { return "Collider"; }

        virtual bool testCollision(ColliderComponent* other) = 0;
        virtual AABB getBounds() const { return bounds; }

        void setTrigger(bool trigger) { isTrigger = trigger; }
        bool getIsTrigger() const { return isTrigger; }

        void setLayer(int l) { layer = l; }
        int getLayer() const { return layer; }
    };

    class BoxCollider : public ColliderComponent {
    private:
        Vector3 center;
        Vector3 size;

    public:
        BoxCollider() : center(0, 0, 0), size(1, 1, 1) {}

        BoxCollider(const Vector3& c, const Vector3& s)
            : center(c), size(s) {}

        std::string getTypeName() const override { return "BoxCollider"; }

        bool testCollision(ColliderComponent* other) override {
            if (!other) return false;

            AABB myBounds = getBounds();
            AABB otherBounds = other->getBounds();

            return myBounds.intersects(otherBounds);
        }

        AABB getBounds() const override {
            Vector3 halfSize = size * 0.5f;
            return AABB(center - halfSize, center + halfSize);
        }

        void setCenter(const Vector3& c) { center = c; }
        const Vector3& getCenter() const { return center; }

        void setSize(const Vector3& s) { size = s; }
        const Vector3& getSize() const { return size; }
    };

    class SphereCollider : public ColliderComponent {
    private:
        Vector3 center;
        float radius;

    public:
        SphereCollider() : center(0, 0, 0), radius(0.5f) {}

        SphereCollider(const Vector3& c, float r)
            : center(c), radius(r) {}

        std::string getTypeName() const override { return "SphereCollider"; }

        bool testCollision(ColliderComponent* other) override {
            if (!other) return false;

            if (auto* otherSphere = dynamic_cast<SphereCollider*>(other)) {
                Vector3 diff = center - otherSphere->center;
                float distSq = diff.x * diff.x + diff.y * diff.y + diff.z * diff.z;
                float radiusSum = radius + otherSphere->radius;
                return distSq <= (radiusSum * radiusSum);
            }

            // Fallback to AABB test
            AABB myBounds = getBounds();
            AABB otherBounds = other->getBounds();
            return myBounds.intersects(otherBounds);
        }

        AABB getBounds() const override {
            Vector3 radiusVec(radius, radius, radius);
            return AABB(center - radiusVec, center + radiusVec);
        }

        void setCenter(const Vector3& c) { center = c; }
        const Vector3& getCenter() const { return center; }

        void setRadius(float r) { radius = r; }
        float getRadius() const { return radius; }
    };

    struct CollisionInfo {
        EntityID entityA;
        EntityID entityB;
        Vector3 contactPoint;
        Vector3 contactNormal;
        float penetrationDepth;

        CollisionInfo() : entityA(0), entityB(0),
            contactPoint(), contactNormal(), penetrationDepth(0) {}
    };

    class PhysicsWorld {
    private:
        std::vector<RigidbodyComponent*> rigidbodies;
        std::vector<ColliderComponent*> colliders;
        std::vector<CollisionInfo> collisions;
        Vector3 gravity;
        float fixedTimeStep;
        float accumulator;

    public:
        PhysicsWorld()
            : gravity(0, -9.81f, 0),
              fixedTimeStep(1.0f / 60.0f),
              accumulator(0.0f) {}

        void registerRigidbody(RigidbodyComponent* rb) {
            rigidbodies.push_back(rb);
        }

        void registerCollider(ColliderComponent* collider) {
            colliders.push_back(collider);
        }

        void setGravity(const Vector3& g) { gravity = g; }
        const Vector3& getGravity() const { return gravity; }

        void update(float deltaTime) {
            accumulator += deltaTime;

            while (accumulator >= fixedTimeStep) {
                fixedUpdate(fixedTimeStep);
                accumulator -= fixedTimeStep;
            }
        }

    private:
        void fixedUpdate(float dt) {
            // Apply forces to rigidbodies
            for (auto* rb : rigidbodies) {
                if (rb && rb->isEnabled() && !rb->getIsKinematic()) {
                    if (rb->hasGravity()) {
                        rb->addForce(gravity * rb->getMass());
                    }
                    rb->update(dt);
                }
            }

            // Broad phase collision detection
            collisions.clear();
            detectCollisions();

            // Resolve collisions
            resolveCollisions();
        }

        void detectCollisions() {
            for (size_t i = 0; i < colliders.size(); ++i) {
                for (size_t j = i + 1; j < colliders.size(); ++j) {
                    if (colliders[i]->testCollision(colliders[j])) {
                        CollisionInfo info;
                        info.entityA = colliders[i]->getOwner();
                        info.entityB = colliders[j]->getOwner();
                        collisions.push_back(info);
                    }
                }
            }
        }

        void resolveCollisions() {
            for (const auto& collision : collisions) {
                // Simplified collision response
                // In real engine: proper impulse-based resolution
            }
        }
    };

} // namespace Physics
} // namespace GameEngine
```

---

## 3. Particle System

```cpp
namespace GameEngine {

    struct Particle {
        Vector3 position;
        Vector3 velocity;
        Vector3 acceleration;
        Vector4 color; // RGBA
        float lifetime;
        float age;
        float size;

        Particle()
            : position(), velocity(), acceleration(),
              color(1, 1, 1, 1), lifetime(1.0f), age(0.0f), size(1.0f) {}

        bool isAlive() const { return age < lifetime; }

        void update(float deltaTime) {
            if (!isAlive()) return;

            age += deltaTime;
            velocity = velocity + (acceleration * deltaTime);
            position = position + (velocity * deltaTime);

            // Fade out near end of lifetime
            float lifeRatio = age / lifetime;
            color.w = 1.0f - lifeRatio; // Alpha fade
        }
    };

    struct Vector4 {
        float x, y, z, w;
        Vector4() : x(0), y(0), z(0), w(1) {}
        Vector4(float _x, float _y, float _z, float _w)
            : x(_x), y(_y), z(_z), w(_w) {}
    };

    class ParticleEmitter : public Component {
    private:
        std::vector<Particle> particles;
        size_t maxParticles;
        float emissionRate; // particles per second
        float emissionTimer;

        // Emission properties
        Vector3 emissionDirection;
        float emissionSpread;
        float particleSpeed;
        float particleLifetime;
        Vector3 particleAcceleration;

    public:
        ParticleEmitter(size_t maxCount = 1000)
            : maxParticles(maxCount),
              emissionRate(10.0f),
              emissionTimer(0.0f),
              emissionDirection(0, 1, 0),
              emissionSpread(30.0f),
              particleSpeed(5.0f),
              particleLifetime(2.0f),
              particleAcceleration(0, -9.81f, 0) {
            particles.reserve(maxParticles);
        }

        std::string getTypeName() const override { return "ParticleEmitter"; }

        void update(float deltaTime) override {
            if (!enabled) return;

            // Update existing particles
            for (auto& particle : particles) {
                particle.update(deltaTime);
            }

            // Remove dead particles
            particles.erase(
                std::remove_if(particles.begin(), particles.end(),
                    [](const Particle& p) { return !p.isAlive(); }),
                particles.end()
            );

            // Emit new particles
            emissionTimer += deltaTime;
            float emissionInterval = 1.0f / emissionRate;

            while (emissionTimer >= emissionInterval && particles.size() < maxParticles) {
                emitParticle();
                emissionTimer -= emissionInterval;
            }
        }

        void emitParticle() {
            if (particles.size() >= maxParticles) return;

            Particle p;
            p.position = Vector3(0, 0, 0); // Emitter position
            p.velocity = emissionDirection * particleSpeed;
            p.acceleration = particleAcceleration;
            p.lifetime = particleLifetime;
            p.age = 0.0f;
            p.size = 1.0f;
            p.color = Vector4(1, 1, 1, 1);

            particles.push_back(p);
        }

        void setEmissionRate(float rate) { emissionRate = rate; }
        float getEmissionRate() const { return emissionRate; }

        size_t getActiveParticleCount() const { return particles.size(); }
        size_t getMaxParticles() const { return maxParticles; }
    };

} // namespace GameEngine
```

---

## 4. Event System

```cpp
namespace GameEngine {

    class Event {
    public:
        virtual ~Event() = default;
        virtual std::string getType() const = 0;
    };

    class CollisionEvent : public Event {
    public:
        EntityID entityA;
        EntityID entityB;

        CollisionEvent(EntityID a, EntityID b) : entityA(a), entityB(b) {}

        std::string getType() const override { return "Collision"; }
    };

    class InputEvent : public Event {
    public:
        std::string inputName;
        float value;

        InputEvent(const std::string& name, float val)
            : inputName(name), value(val) {}

        std::string getType() const override { return "Input"; }
    };

    using EventCallback = std::function<void(const Event&)>;

    class EventSystem {
    private:
        std::unordered_map<std::string, std::vector<EventCallback>> listeners;
        std::queue<std::unique_ptr<Event>> eventQueue;
        std::mutex mutex;

    public:
        void subscribe(const std::string& eventType, EventCallback callback) {
            std::lock_guard<std::mutex> lock(mutex);
            listeners[eventType].push_back(callback);
        }

        void publish(std::unique_ptr<Event> event) {
            std::lock_guard<std::mutex> lock(mutex);
            eventQueue.push(std::move(event));
        }

        void processEvents() {
            std::lock_guard<std::mutex> lock(mutex);

            while (!eventQueue.empty()) {
                auto& event = eventQueue.front();
                std::string type = event->getType();

                auto it = listeners.find(type);
                if (it != listeners.end()) {
                    for (auto& callback : it->second) {
                        callback(*event);
                    }
                }

                eventQueue.pop();
            }
        }
    };

} // namespace GameEngine
```

---

## 5. Enhanced Game Engine

```cpp
namespace GameEngine {

    class EnhancedEngine {
    private:
        // Core systems
        std::unique_ptr<JobSystem> jobSystem;
        std::unique_ptr<Time> time;
        std::unique_ptr<Scene> activeScene;
        std::unique_ptr<Rendering::Renderer> renderer;
        std::unique_ptr<Physics::PhysicsWorld> physicsWorld;
        std::unique_ptr<EventSystem> eventSystem;

        // Resource managers
        ResourceManager<Mesh> meshManager;
        ResourceManager<Material> materialManager;

        // Engine state
        std::atomic<bool> running;
        bool initialized;

        // Performance metrics
        struct PerformanceMetrics {
            std::atomic<uint64_t> updateTime{0};
            std::atomic<uint64_t> physicsTime{0};
            std::atomic<uint64_t> renderTime{0};
            std::atomic<uint64_t> totalFrameTime{0};
        } metrics;

    public:
        EnhancedEngine() : initialized(false), running(false) {}

        ~EnhancedEngine() {
            shutdown();
        }

        bool initialize() {
            if (initialized) return true;

            std::cout << "Initializing Enhanced Game Engine...\n";

            // Initialize all subsystems
            jobSystem = std::make_unique<JobSystem>();
            time = std::make_unique<Time>();
            renderer = std::make_unique<Rendering::Renderer>();
            physicsWorld = std::make_unique<Physics::PhysicsWorld>();
            eventSystem = std::make_unique<EventSystem>();

            // Create default scene
            activeScene = std::make_unique<Scene>("DefaultScene");
            activeScene->load();
            activeScene->setActive(true);

            // Subscribe to collision events
            eventSystem->subscribe("Collision", [](const Event& e) {
                auto& collision = static_cast<const CollisionEvent&>(e);
                std::cout << "Collision between entities: "
                          << collision.entityA << " and " << collision.entityB << "\n";
            });

            initialized = true;
            std::cout << "Enhanced Game Engine initialized\n";

            return true;
        }

        void run() {
            if (!initialized) return;

            running.store(true);

            while (running.load()) {
                auto frameStart = std::chrono::high_resolution_clock::now();

                // Update time
                time->update();
                float deltaTime = time->getDeltaTime();

                // Process events
                eventSystem->processEvents();

                // Update physics
                auto physicsStart = std::chrono::high_resolution_clock::now();
                physicsWorld->update(deltaTime);
                auto physicsEnd = std::chrono::high_resolution_clock::now();
                metrics.physicsTime = std::chrono::duration_cast<std::chrono::microseconds>(
                    physicsEnd - physicsStart).count();

                // Update scene
                auto updateStart = std::chrono::high_resolution_clock::now();
                if (activeScene && activeScene->isActive()) {
                    activeScene->update(deltaTime);
                }
                auto updateEnd = std::chrono::high_resolution_clock::now();
                metrics.updateTime = std::chrono::duration_cast<std::chrono::microseconds>(
                    updateEnd - updateStart).count();

                // Render
                auto renderStart = std::chrono::high_resolution_clock::now();
                renderer->beginFrame();
                renderScene();
                renderer->render();
                renderer->endFrame();
                auto renderEnd = std::chrono::high_resolution_clock::now();
                metrics.renderTime = std::chrono::duration_cast<std::chrono::microseconds>(
                    renderEnd - renderStart).count();

                auto frameEnd = std::chrono::high_resolution_clock::now();
                metrics.totalFrameTime = std::chrono::duration_cast<std::chrono::microseconds>(
                    frameEnd - frameStart).count();

                // Print stats
                if (time->getFrameCount() % 60 == 0) {
                    printPerformanceStats();
                }

                // Frame limiting
                std::this_thread::sleep_for(std::chrono::milliseconds(16)); // ~60 FPS
            }
        }

        void stop() {
            running.store(false);
        }

        void shutdown() {
            if (!initialized) return;

            std::cout << "Shutting down Enhanced Engine...\n";

            running.store(false);

            if (activeScene) {
                activeScene->unload();
            }

            jobSystem->shutdown();
            meshManager.unloadAll();
            materialManager.unloadAll();

            initialized = false;
        }

        // Accessors
        Scene* getActiveScene() { return activeScene.get(); }
        Rendering::Renderer* getRenderer() { return renderer.get(); }
        Physics::PhysicsWorld* getPhysicsWorld() { return physicsWorld.get(); }
        EventSystem* getEventSystem() { return eventSystem.get(); }

    private:
        void renderScene() {
            // Submit all visible meshes to renderer
            // In a real engine, this would iterate through all entities
            // with MeshRenderer components and submit them
        }

        void printPerformanceStats() {
            std::cout << "\n=== Performance Stats ===\n";
            std::cout << "FPS: " << time->getFPS() << "\n";
            std::cout << "Frame: " << time->getFrameCount() << "\n";
            std::cout << "Update Time: " << (metrics.updateTime.load() / 1000.0) << " ms\n";
            std::cout << "Physics Time: " << (metrics.physicsTime.load() / 1000.0) << " ms\n";
            std::cout << "Render Time: " << (metrics.renderTime.load() / 1000.0) << " ms\n";
            std::cout << "Total Frame Time: " << (metrics.totalFrameTime.load() / 1000.0) << " ms\n";
            std::cout << "Draw Calls: " << renderer->getDrawCallCount() << "\n";
        }
    };

} // namespace GameEngine
```

---

## 6. Complete Demo

```cpp
using namespace GameEngine;

int main() {
    std::cout << "=== Enhanced Game Engine Demo ===\n\n";

    EnhancedEngine engine;
    if (!engine.initialize()) {
        return 1;
    }

    Scene* scene = engine.getActiveScene();

    // Create player with physics
    Entity* player = scene->createEntity("Player");
    auto* playerTransform = player->addComponent<TransformComponent>();
    playerTransform->setPosition(Vector3(0, 5, 0));

    auto* playerRB = player->addComponent<RigidbodyComponent>();
    playerRB->setMass(70.0f);

    auto* playerCollider = player->addComponent<Physics::BoxCollider>();
    playerCollider->setSize(Vector3(1, 2, 1));

    auto* playerRenderer = player->addComponent<MeshRendererComponent>("Player", "PlayerMat");

    engine.getPhysicsWorld()->registerRigidbody(playerRB);
    engine.getPhysicsWorld()->registerCollider(playerCollider);

    // Create ground
    Entity* ground = scene->createEntity("Ground");
    auto* groundTransform = ground->addComponent<TransformComponent>();
    groundTransform->setPosition(Vector3(0, 0, 0));
    groundTransform->setScale(Vector3(100, 1, 100));

    auto* groundCollider = ground->addComponent<Physics::BoxCollider>();
    groundCollider->setSize(Vector3(100, 1, 100));

    engine.getPhysicsWorld()->registerCollider(groundCollider);

    // Create particle system
    Entity* particleSystem = scene->createEntity("ParticleSystem");
    auto* emitter = particleSystem->addComponent<ParticleEmitter>(1000);
    emitter->setEmissionRate(50.0f);

    scene->printHierarchy();

    // Run engine
    std::thread engineThread([&engine]() {
        engine.run();
    });

    std::this_thread::sleep_for(std::chrono::seconds(5));

    engine.stop();
    engineThread.join();

    std::cout << "\n=== Demo Complete ===\n";

    return 0;
}
```

---

## Summary

Part 2 adds:

1. ✅ **Rendering System**: Command buffer, camera, render queue
2. ✅ **Collision Detection**: AABB, sphere, box colliders
3. ✅ **Physics Integration**: Rigidbody, forces, gravity
4. ✅ **Particle System**: Emission, lifetime, updates
5. ✅ **Event System**: Pub/sub pattern for game events
6. ✅ **Performance Profiling**: Real-time metrics

**Next**: Network Server with Connection Pooling!

---

**Compilation**:
```bash
g++ -std=c++17 -O3 -pthread game_engine_part2.cpp -o game_engine
./game_engine
```
