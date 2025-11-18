# Lesson 68: Bridge Pattern

**Duration**: 5 hours
**Difficulty**: Advanced
**Pattern Type**: Structural
**Part**: 4 - Optimization & Advanced Techniques

## Table of Contents
1. What is the Bridge Pattern?
2. Problem: Cartesian Product Complexity
3. Bridge Solution
4. Structure and Components
5. Real-World Examples
6. Complete Working Examples
7. Bridge vs Adapter
8. Advantages and Disadvantages
9. When to Use
10. Exercises

---

## 1. What is the Bridge Pattern?

The **Bridge Pattern** decouples an abstraction from its implementation so that the two can vary independently.

### Intent:
- Separate abstraction from implementation
- Both can vary independently
- Avoid permanent binding between abstraction and implementation
- Also known as **Handle/Body**

### Real-World Analogy:

**Remote Control**: A TV remote (abstraction) can work with different TV brands (implementations). You can change the TV without changing the remote, and vice versa.

**Graphics API**: A graphics library (abstraction) can use different rendering engines (OpenGL, DirectX, Vulkan) as implementation.

---

## 2. Problem: Cartesian Product Complexity

### The Problem:

```cpp
// Without Bridge - Cartesian Product Explosion!

class RedCircle { /* ... */ };
class BlueCircle { /* ... */ };
class RedSquare { /* ... */ };
class BlueSquare { /* ... */ };
class RedTriangle { /* ... */ };
class BlueTriangle { /* ... */ };

// Adding one more color requires 3 new classes!
// Adding one more shape requires N new classes (where N = number of colors)!
// 3 shapes × 2 colors = 6 classes
// 10 shapes × 5 colors = 50 classes!
```

### The Solution with Bridge:

```cpp
#include <iostream>
#include <memory>
using namespace std;

// Implementation (Color)
class Color {
public:
    virtual void ApplyColor() = 0;
    virtual ~Color() = default;
};

class Red : public Color {
public:
    void ApplyColor() override {
        cout << "Red ";
    }
};

class Blue : public Color {
public:
    void ApplyColor() override {
        cout << "Blue ";
    }
};

// Abstraction (Shape)
class Shape {
protected:
    unique_ptr<Color> color;

public:
    Shape(unique_ptr<Color> c) : color(move(c)) {}
    virtual void Draw() = 0;
    virtual ~Shape() = default;
};

class Circle : public Shape {
public:
    Circle(unique_ptr<Color> c) : Shape(move(c)) {}

    void Draw() override {
        color->ApplyColor();
        cout << "Circle\n";
    }
};

class Square : public Shape {
public:
    Square(unique_ptr<Color> c) : Shape(move(c)) {}

    void Draw() override {
        color->ApplyColor();
        cout << "Square\n";
    }
};

int main() {
    auto redCircle = make_unique<Circle>(make_unique<Red>());
    auto blueSquare = make_unique<Square>(make_unique<Blue>());

    redCircle->Draw();
    blueSquare->Draw();

    // Now: 3 shapes + 2 colors = 5 classes (instead of 6)
    // With 10 shapes + 5 colors = 15 classes (instead of 50!)

    return 0;
}
```

---

## 3. Structure and Components

### UML Structure:

```
┌─────────────────┐
│  Abstraction    │
├─────────────────┤         ┌──────────────────┐
│ - implementor   │────────▶│  Implementor     │
│ + Operation()   │         ├──────────────────┤
└─────────────────┘         │ + OperationImp() │
        △                   └──────────────────┘
        │                           △
        │                           │
┌─────────────────┐         ┌──────────────────┐
│ RefinedAbstract │         │ ConcreteImpA     │
├─────────────────┤         ├──────────────────┤
│ + Operation()   │         │ + OperationImp() │
└─────────────────┘         └──────────────────┘
                            ┌──────────────────┐
                            │ ConcreteImpB     │
                            ├──────────────────┤
                            │ + OperationImp() │
                            └──────────────────┘
```

---

## 4. Real-World Examples

### Example 1: Graphics Rendering System

```cpp
#include <iostream>
#include <memory>
#include <string>
using namespace std;

// Implementation - Rendering API
class IRenderer {
public:
    virtual void RenderCircle(double x, double y, double radius) = 0;
    virtual void RenderSquare(double x, double y, double size) = 0;
    virtual ~IRenderer() = default;
};

// Concrete Implementations
class OpenGLRenderer : public IRenderer {
public:
    void RenderCircle(double x, double y, double radius) override {
        cout << "[OpenGL] Drawing circle at (" << x << "," << y
             << ") radius " << radius << "\n";
    }

    void RenderSquare(double x, double y, double size) override {
        cout << "[OpenGL] Drawing square at (" << x << "," << y
             << ") size " << size << "\n";
    }
};

class DirectXRenderer : public IRenderer {
public:
    void RenderCircle(double x, double y, double radius) override {
        cout << "[DirectX] Rendering circle at (" << x << "," << y
             << ") radius " << radius << "\n";
    }

    void RenderSquare(double x, double y, double size) override {
        cout << "[DirectX] Rendering square at (" << x << "," << y
             << ") size " << size << "\n";
    }
};

class VulkanRenderer : public IRenderer {
public:
    void RenderCircle(double x, double y, double radius) override {
        cout << "[Vulkan] Rendering circle at (" << x << "," << y
             << ") radius " << radius << "\n";
    }

    void RenderSquare(double x, double y, double size) override {
        cout << "[Vulkan] Rendering square at (" << x << "," << y
             << ") size " << size << "\n";
    }
};

// Abstraction - Shape
class Shape {
protected:
    unique_ptr<IRenderer> renderer;
    double x, y;

public:
    Shape(unique_ptr<IRenderer> r, double xPos, double yPos)
        : renderer(move(r)), x(xPos), y(yPos) {}

    virtual void Draw() = 0;
    virtual void Move(double newX, double newY) {
        x = newX;
        y = newY;
    }

    virtual ~Shape() = default;
};

// Refined Abstractions
class Circle : public Shape {
private:
    double radius;

public:
    Circle(unique_ptr<IRenderer> r, double xPos, double yPos, double rad)
        : Shape(move(r), xPos, yPos), radius(rad) {}

    void Draw() override {
        renderer->RenderCircle(x, y, radius);
    }

    void SetRadius(double r) { radius = r; }
};

class Square : public Shape {
private:
    double size;

public:
    Square(unique_ptr<IRenderer> r, double xPos, double yPos, double sz)
        : Shape(move(r), xPos, yPos), size(sz) {}

    void Draw() override {
        renderer->RenderSquare(x, y, size);
    }

    void SetSize(double s) { size = s; }
};

int main() {
    cout << "=== Using OpenGL Renderer ===\n";
    Circle openGLCircle(make_unique<OpenGLRenderer>(), 10, 20, 5);
    Square openGLSquare(make_unique<OpenGLRenderer>(), 30, 40, 10);
    openGLCircle.Draw();
    openGLSquare.Draw();

    cout << "\n=== Using DirectX Renderer ===\n";
    Circle directXCircle(make_unique<DirectXRenderer>(), 50, 60, 7);
    Square directXSquare(make_unique<DirectXRenderer>(), 70, 80, 15);
    directXCircle.Draw();
    directXSquare.Draw();

    cout << "\n=== Using Vulkan Renderer ===\n";
    Circle vulkanCircle(make_unique<VulkanRenderer>(), 90, 100, 12);
    vulkanCircle.Draw();

    return 0;
}
```

**Output:**
```
=== Using OpenGL Renderer ===
[OpenGL] Drawing circle at (10,20) radius 5
[OpenGL] Drawing square at (30,40) size 10

=== Using DirectX Renderer ===
[DirectX] Rendering circle at (50,60) radius 7
[DirectX] Rendering square at (70,80) size 15

=== Using Vulkan Renderer ===
[Vulkan] Rendering circle at (90,100) radius 12
```

### Example 2: Device and Remote Control

```cpp
#include <iostream>
#include <memory>
#include <string>
using namespace std;

// Implementation - Device
class IDevice {
public:
    virtual void PowerOn() = 0;
    virtual void PowerOff() = 0;
    virtual void SetVolume(int level) = 0;
    virtual void SetChannel(int channel) = 0;
    virtual int GetVolume() = 0;
    virtual int GetChannel() = 0;
    virtual bool IsEnabled() = 0;
    virtual ~IDevice() = default;
};

// Concrete Implementation - TV
class TV : public IDevice {
private:
    bool on;
    int volume;
    int channel;

public:
    TV() : on(false), volume(50), channel(1) {}

    void PowerOn() override {
        on = true;
        cout << "TV: Powered ON\n";
    }

    void PowerOff() override {
        on = false;
        cout << "TV: Powered OFF\n";
    }

    void SetVolume(int level) override {
        volume = level;
        cout << "TV: Volume set to " << volume << "\n";
    }

    void SetChannel(int ch) override {
        channel = ch;
        cout << "TV: Channel changed to " << channel << "\n";
    }

    int GetVolume() override { return volume; }
    int GetChannel() override { return channel; }
    bool IsEnabled() override { return on; }
};

// Concrete Implementation - Radio
class Radio : public IDevice {
private:
    bool on;
    int volume;
    int channel;

public:
    Radio() : on(false), volume(30), channel(101) {}

    void PowerOn() override {
        on = true;
        cout << "Radio: Powered ON\n";
    }

    void PowerOff() override {
        on = false;
        cout << "Radio: Powered OFF\n";
    }

    void SetVolume(int level) override {
        volume = level;
        cout << "Radio: Volume set to " << volume << "\n";
    }

    void SetChannel(int ch) override {
        channel = ch;
        cout << "Radio: Frequency tuned to " << channel << " FM\n";
    }

    int GetVolume() override { return volume; }
    int GetChannel() override { return channel; }
    bool IsEnabled() override { return on; }
};

// Abstraction - Remote Control
class RemoteControl {
protected:
    unique_ptr<IDevice> device;

public:
    RemoteControl(unique_ptr<IDevice> dev) : device(move(dev)) {}

    void TogglePower() {
        if (device->IsEnabled()) {
            device->PowerOff();
        } else {
            device->PowerOn();
        }
    }

    void VolumeUp() {
        int current = device->GetVolume();
        device->SetVolume(current + 10);
    }

    void VolumeDown() {
        int current = device->GetVolume();
        device->SetVolume(current - 10);
    }

    void ChannelUp() {
        int current = device->GetChannel();
        device->SetChannel(current + 1);
    }

    void ChannelDown() {
        int current = device->GetChannel();
        device->SetChannel(current - 1);
    }

    virtual ~RemoteControl() = default;
};

// Refined Abstraction - Advanced Remote
class AdvancedRemote : public RemoteControl {
public:
    AdvancedRemote(unique_ptr<IDevice> dev) : RemoteControl(move(dev)) {}

    void Mute() {
        cout << "Advanced Remote: Muting\n";
        device->SetVolume(0);
    }

    void SetChannel(int channel) {
        cout << "Advanced Remote: Direct channel select\n";
        device->SetChannel(channel);
    }
};

int main() {
    cout << "=== Basic Remote with TV ===\n";
    RemoteControl tvRemote(make_unique<TV>());
    tvRemote.TogglePower();
    tvRemote.VolumeUp();
    tvRemote.ChannelUp();
    tvRemote.TogglePower();

    cout << "\n=== Advanced Remote with Radio ===\n";
    AdvancedRemote radioRemote(make_unique<Radio>());
    radioRemote.TogglePower();
    radioRemote.VolumeUp();
    radioRemote.SetChannel(105);
    radioRemote.Mute();

    return 0;
}
```

---

## 5. Complete Working Examples

### Complete Example: Message Sending System

```cpp
#include <iostream>
#include <memory>
#include <string>
using namespace std;

// Implementation - Message Sender
class IMessageSender {
public:
    virtual void SendMessage(const string& recipient, const string& message) = 0;
    virtual ~IMessageSender() = default;
};

// Concrete Implementations
class EmailSender : public IMessageSender {
public:
    void SendMessage(const string& recipient, const string& message) override {
        cout << "[EMAIL] To: " << recipient << "\n";
        cout << "        Subject: Message\n";
        cout << "        Body: " << message << "\n";
    }
};

class SMSSender : public IMessageSender {
public:
    void SendMessage(const string& recipient, const string& message) override {
        cout << "[SMS] To: " << recipient << "\n";
        cout << "      Message: " << message << "\n";
    }
};

class PushNotificationSender : public IMessageSender {
public:
    void SendMessage(const string& recipient, const string& message) override {
        cout << "[PUSH] Device: " << recipient << "\n";
        cout << "       Notification: " << message << "\n";
    }
};

// Abstraction - Message
class Message {
protected:
    unique_ptr<IMessageSender> sender;
    string content;

public:
    Message(unique_ptr<IMessageSender> s, const string& msg)
        : sender(move(s)), content(msg) {}

    virtual void Send(const string& recipient) = 0;
    virtual ~Message() = default;
};

// Refined Abstractions
class TextMessage : public Message {
public:
    TextMessage(unique_ptr<IMessageSender> s, const string& msg)
        : Message(move(s), msg) {}

    void Send(const string& recipient) override {
        cout << "\n--- Sending Text Message ---\n";
        sender->SendMessage(recipient, content);
    }
};

class UrgentMessage : public Message {
public:
    UrgentMessage(unique_ptr<IMessageSender> s, const string& msg)
        : Message(move(s), msg) {}

    void Send(const string& recipient) override {
        cout << "\n--- Sending URGENT Message ---\n";
        string urgentContent = "[URGENT] " + content;
        sender->SendMessage(recipient, urgentContent);
    }
};

class EncryptedMessage : public Message {
public:
    EncryptedMessage(unique_ptr<IMessageSender> s, const string& msg)
        : Message(move(s), msg) {}

    void Send(const string& recipient) override {
        cout << "\n--- Sending Encrypted Message ---\n";
        string encrypted = EncryptContent(content);
        sender->SendMessage(recipient, encrypted);
    }

private:
    string EncryptContent(const string& msg) {
        // Simple encryption simulation
        return "[ENCRYPTED: " + msg + "]";
    }
};

int main() {
    // Text message via Email
    TextMessage email(make_unique<EmailSender>(), "Hello from email!");
    email.Send("user@example.com");

    // Urgent message via SMS
    UrgentMessage urgentSMS(make_unique<SMSSender>(), "Server is down!");
    urgentSMS.Send("+1234567890");

    // Encrypted message via Push Notification
    EncryptedMessage securePush(make_unique<PushNotificationSender>(),
                                 "Confidential information");
    securePush.Send("device-token-12345");

    // Urgent message via Email
    UrgentMessage urgentEmail(make_unique<EmailSender>(), "Critical bug found!");
    urgentEmail.Send("dev-team@example.com");

    return 0;
}
```

---

## 6. Bridge vs Adapter

### Comparison:

| Aspect | Bridge | Adapter |
|--------|--------|---------|
| **Intent** | Design upfront | Fix incompatibility |
| **Timing** | Before implementation | After implementation |
| **Purpose** | Separate abstraction/implementation | Make interfaces compatible |
| **Both Vary** | Yes, independently | No |
| **Structure** | Intentional separation | Wrapper around existing |

---

## 7. Advantages and Disadvantages

### Advantages:

1. ✅ **Independent Variations**: Abstraction and implementation can vary independently
2. ✅ **Reduced Classes**: Avoids cartesian product explosion
3. ✅ **Runtime Binding**: Can switch implementation at runtime
4. ✅ **Single Responsibility**: Separates high-level logic from platform details
5. ✅ **Open/Closed**: Easy to add new abstractions and implementations

### Disadvantages:

1. ❌ **Complexity**: Additional level of indirection
2. ❌ **Initial Overhead**: Requires more planning upfront
3. ❌ **May Be Overkill**: For simple cases, might be unnecessary

---

## 8. When to Use

### Use Bridge When:
- ✅ Want to avoid permanent binding between abstraction and implementation
- ✅ Both abstraction and implementation should be extensible
- ✅ Have multiple dimensions of variation
- ✅ Want to share implementation among multiple objects
- ✅ Need to switch implementation at runtime

### Don't Use When:
- ❌ Only one implementation exists
- ❌ Abstraction and implementation won't vary independently
- ❌ Simple inheritance is sufficient

---

## 9. Exercises

### Exercise 1: Vehicle Control System
Create a bridge between Vehicle types (Car, Boat, Plane) and Engine types (Electric, Gas, Hybrid).

### Exercise 2: Report Generator
Implement bridge between Report types (Sales, Inventory, Financial) and Format types (PDF, HTML, Excel).

### Exercise 3: Payment Processing
Build bridge between Payment methods (CreditCard, PayPal, Crypto) and Currency types (USD, EUR, BTC).

---

## Key Takeaways

1. ✅ Bridge **separates abstraction from implementation**
2. ✅ Both can **vary independently**
3. ✅ Avoids **cartesian product** class explosion
4. ✅ Provides **runtime flexibility**
5. ✅ Uses **composition** over inheritance
6. ✅ Different from **Adapter** (designed upfront)
7. ✅ Follows **Single Responsibility** and **Open/Closed**
8. ✅ Common in **cross-platform systems**

---

## Next Lesson Preview

In **Lesson 69**, we'll learn about the **Composite Pattern**:
- Tree structures of objects
- Part-whole hierarchies
- Uniform treatment of objects
- File systems, UI components, org charts

---

## Summary Checklist

Before moving to Lesson 69, ensure you can:

- [ ] Explain the Bridge pattern
- [ ] Identify cartesian product problem
- [ ] Separate abstraction from implementation
- [ ] Implement independent variations
- [ ] Distinguish Bridge from Adapter
- [ ] Apply Bridge to multi-dimensional problems
- [ ] Use composition effectively

**Excellent!** Bridge pattern mastered!

Next: **Composite Pattern** - tree structures!
