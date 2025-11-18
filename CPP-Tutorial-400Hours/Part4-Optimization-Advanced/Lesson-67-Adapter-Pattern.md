# Lesson 67: Adapter Pattern

**Duration**: 5 hours
**Difficulty**: Advanced
**Pattern Type**: Structural
**Part**: 4 - Optimization & Advanced Techniques

## Table of Contents
1. What is the Adapter Pattern?
2. Problem and Solution
3. Class Adapter vs Object Adapter
4. Structure and Components
5. Real-World Examples
6. Complete Working Examples
7. Two-Way Adapters
8. Advantages and Disadvantages
9. When to Use
10. Exercises

---

## 1. What is the Adapter Pattern?

The **Adapter Pattern** converts the interface of a class into another interface that clients expect. It lets classes work together that couldn't otherwise because of incompatible interfaces.

### Intent:
- Convert one interface to another
- Allow incompatible interfaces to work together
- Wrap existing class with new interface
- Also known as **Wrapper**

### Real-World Analogy:

**Power Adapter**: When traveling to another country, you need a power adapter to connect your device (US plug) to a foreign outlet (EU socket). The adapter **converts** one interface to another.

**Memory Card Reader**: Adapts SD card to USB interface so computer can read it.

**Language Translator**: Translates between two people who speak different languages.

---

## 2. Problem and Solution

### The Problem:

```cpp
// Legacy class you can't modify
class OldPrinter {
public:
    void PrintOldFormat(const string& text) {
        cout << "[OLD FORMAT] " << text << endl;
    }
};

// Modern interface your system expects
class IPrinter {
public:
    virtual void Print(const string& document) = 0;
    virtual ~IPrinter() = default;
};

// ❌ Can't use OldPrinter directly - interface doesn't match!
void ProcessDocument(IPrinter* printer) {
    printer->Print("Document content");
}

int main() {
    OldPrinter oldPrinter;
    // ProcessDocument(&oldPrinter);  // ERROR: OldPrinter doesn't implement IPrinter!
}
```

### The Solution (Object Adapter):

```cpp
#include <iostream>
#include <string>
using namespace std;

// Legacy class (Adaptee)
class OldPrinter {
public:
    void PrintOldFormat(const string& text) {
        cout << "[OLD FORMAT] " << text << endl;
    }
};

// Target interface
class IPrinter {
public:
    virtual void Print(const string& document) = 0;
    virtual ~IPrinter() = default;
};

// Adapter - makes OldPrinter compatible with IPrinter
class PrinterAdapter : public IPrinter {
private:
    OldPrinter* oldPrinter;

public:
    PrinterAdapter(OldPrinter* printer) : oldPrinter(printer) {}

    void Print(const string& document) override {
        // Adapt the interface
        oldPrinter->PrintOldFormat(document);
    }
};

// Client code
void ProcessDocument(IPrinter* printer) {
    printer->Print("Document content");
}

int main() {
    OldPrinter oldPrinter;
    PrinterAdapter adapter(&oldPrinter);

    ProcessDocument(&adapter);  // ✅ Works!

    return 0;
}
```

**Output:**
```
[OLD FORMAT] Document content
```

---

## 3. Class Adapter vs Object Adapter

### Object Adapter (Composition - Preferred):

```cpp
// Uses composition - has-a relationship
class ObjectAdapter : public Target {
private:
    Adaptee* adaptee;  // Composition

public:
    ObjectAdapter(Adaptee* a) : adaptee(a) {}

    void Request() override {
        adaptee->SpecificRequest();
    }
};
```

**Advantages:**
- ✅ More flexible (can adapt multiple adaptees)
- ✅ Can add functionality easily
- ✅ Runtime binding

### Class Adapter (Inheritance):

```cpp
// Uses multiple inheritance - is-a relationship
class ClassAdapter : public Target, private Adaptee {
public:
    void Request() override {
        SpecificRequest();  // Directly call inherited method
    }
};
```

**Advantages:**
- ✅ No extra object
- ✅ Can override adaptee behavior

**Disadvantages:**
- ❌ C++ doesn't support true multiple inheritance from interfaces and classes well
- ❌ Less flexible
- ❌ Compile-time binding

---

## 4. Structure and Components

### UML Structure:

```
┌──────────────┐
│    Client    │
└──────┬───────┘
       │ uses
       ▼
┌──────────────┐
│   Target     │ ← Interface client expects
├──────────────┤
│ + Request()  │
└──────────────┘
       △
       │ implements
       │
┌──────────────┐         ┌──────────────────┐
│   Adapter    │─────────│    Adaptee       │
├──────────────┤ adapts  ├──────────────────┤
│ - adaptee    │         │ + SpecificReq()  │
│ + Request()  │         └──────────────────┘
└──────────────┘              △
                              │
                         Legacy class
```

---

## 5. Real-World Examples

### Example 1: Media Player Adapter

```cpp
#include <iostream>
#include <string>
using namespace std;

// Advanced media players (Adaptees)
class VLCPlayer {
public:
    void PlayVLC(const string& filename) {
        cout << "Playing VLC file: " << filename << endl;
    }
};

class MP4Player {
public:
    void PlayMP4(const string& filename) {
        cout << "Playing MP4 file: " << filename << endl;
    }
};

// Target interface
class IMediaPlayer {
public:
    virtual void Play(const string& audioType, const string& filename) = 0;
    virtual ~IMediaPlayer() = default;
};

// Adapter for advanced players
class MediaAdapter : public IMediaPlayer {
private:
    VLCPlayer* vlcPlayer;
    MP4Player* mp4Player;

public:
    MediaAdapter(const string& audioType) {
        if (audioType == "vlc") {
            vlcPlayer = new VLCPlayer();
            mp4Player = nullptr;
        } else if (audioType == "mp4") {
            mp4Player = new MP4Player();
            vlcPlayer = nullptr;
        }
    }

    void Play(const string& audioType, const string& filename) override {
        if (audioType == "vlc") {
            vlcPlayer->PlayVLC(filename);
        } else if (audioType == "mp4") {
            mp4Player->PlayMP4(filename);
        }
    }

    ~MediaAdapter() {
        delete vlcPlayer;
        delete mp4Player;
    }
};

// Audio player (uses adapter for advanced formats)
class AudioPlayer : public IMediaPlayer {
private:
    MediaAdapter* adapter;

public:
    AudioPlayer() : adapter(nullptr) {}

    void Play(const string& audioType, const string& filename) override {
        // Built-in support for mp3
        if (audioType == "mp3") {
            cout << "Playing MP3 file: " << filename << endl;
        }
        // Use adapter for other formats
        else if (audioType == "vlc" || audioType == "mp4") {
            adapter = new MediaAdapter(audioType);
            adapter->Play(audioType, filename);
            delete adapter;
            adapter = nullptr;
        } else {
            cout << "Invalid format: " << audioType << endl;
        }
    }

    ~AudioPlayer() {
        delete adapter;
    }
};

int main() {
    AudioPlayer player;

    player.Play("mp3", "song.mp3");
    player.Play("mp4", "video.mp4");
    player.Play("vlc", "movie.vlc");
    player.Play("avi", "clip.avi");

    return 0;
}
```

**Output:**
```
Playing MP3 file: song.mp3
Playing MP4 file: video.mp4
Playing VLC file: movie.vlc
Invalid format: avi
```

### Example 2: Payment Gateway Adapter

```cpp
#include <iostream>
#include <string>
using namespace std;

// Third-party payment gateways (Adaptees)
class PayPal {
public:
    void SendPayment(const string& email, double amount) {
        cout << "PayPal: Sending $" << amount << " to " << email << endl;
    }
};

class Stripe {
public:
    void ChargeCard(const string& token, int cents) {
        cout << "Stripe: Charging " << cents << " cents using token " << token << endl;
    }
};

class Square {
public:
    void ProcessTransaction(const string& customerId, double dollars) {
        cout << "Square: Processing $" << dollars << " from customer " << customerId << endl;
    }
};

// Target interface
class IPaymentProcessor {
public:
    virtual void ProcessPayment(const string& account, double amount) = 0;
    virtual ~IPaymentProcessor() = default;
};

// Adapters
class PayPalAdapter : public IPaymentProcessor {
private:
    PayPal paypal;

public:
    void ProcessPayment(const string& account, double amount) override {
        paypal.SendPayment(account, amount);
    }
};

class StripeAdapter : public IPaymentProcessor {
private:
    Stripe stripe;

public:
    void ProcessPayment(const string& account, double amount) override {
        int cents = static_cast<int>(amount * 100);
        stripe.ChargeCard(account, cents);
    }
};

class SquareAdapter : public IPaymentProcessor {
private:
    Square square;

public:
    void ProcessPayment(const string& account, double amount) override {
        square.ProcessTransaction(account, amount);
    }
};

// Shopping cart
class ShoppingCart {
private:
    IPaymentProcessor* paymentProcessor;

public:
    void SetPaymentProcessor(IPaymentProcessor* processor) {
        paymentProcessor = processor;
    }

    void Checkout(const string& account, double amount) {
        cout << "\n=== Processing Checkout ===\n";
        cout << "Total amount: $" << amount << endl;
        paymentProcessor->ProcessPayment(account, amount);
        cout << "Payment completed!\n";
    }
};

int main() {
    ShoppingCart cart;

    // Use PayPal
    PayPalAdapter paypal;
    cart.SetPaymentProcessor(&paypal);
    cart.Checkout("user@example.com", 99.99);

    // Use Stripe
    StripeAdapter stripe;
    cart.SetPaymentProcessor(&stripe);
    cart.Checkout("tok_visa", 49.99);

    // Use Square
    SquareAdapter square;
    cart.SetPaymentProcessor(&square);
    cart.Checkout("CUST123", 149.99);

    return 0;
}
```

---

## 6. Complete Working Examples

### Complete Example: Temperature Sensor Adapter System

```cpp
#include <iostream>
#include <memory>
#include <vector>
using namespace std;

// Legacy Fahrenheit sensor
class FahrenheitSensor {
public:
    double GetTemperatureF() {
        return 77.0;  // Simulated reading
    }
};

// Legacy Kelvin sensor
class KelvinSensor {
public:
    double ReadKelvin() {
        return 298.15;  // Simulated reading
    }
};

// Modern Celsius-based system (Target interface)
class ITemperatureSensor {
public:
    virtual double GetTemperatureC() = 0;
    virtual string GetSensorType() = 0;
    virtual ~ITemperatureSensor() = default;
};

// Native Celsius sensor
class CelsiusSensor : public ITemperatureSensor {
public:
    double GetTemperatureC() override {
        return 25.0;
    }

    string GetSensorType() override {
        return "Celsius Sensor";
    }
};

// Adapter for Fahrenheit sensor
class FahrenheitAdapter : public ITemperatureSensor {
private:
    FahrenheitSensor* sensor;

public:
    FahrenheitAdapter(FahrenheitSensor* s) : sensor(s) {}

    double GetTemperatureC() override {
        double fahrenheit = sensor->GetTemperatureF();
        return (fahrenheit - 32.0) * 5.0 / 9.0;  // Convert to Celsius
    }

    string GetSensorType() override {
        return "Fahrenheit Sensor (Adapted)";
    }
};

// Adapter for Kelvin sensor
class KelvinAdapter : public ITemperatureSensor {
private:
    KelvinSensor* sensor;

public:
    KelvinAdapter(KelvinSensor* s) : sensor(s) {}

    double GetTemperatureC() override {
        double kelvin = sensor->ReadKelvin();
        return kelvin - 273.15;  // Convert to Celsius
    }

    string GetSensorType() override {
        return "Kelvin Sensor (Adapted)";
    }
};

// Monitoring system
class TemperatureMonitor {
private:
    vector<unique_ptr<ITemperatureSensor>> sensors;

public:
    void AddSensor(unique_ptr<ITemperatureSensor> sensor) {
        sensors.push_back(move(sensor));
    }

    void DisplayReadings() {
        cout << "\n=== Temperature Monitor Readings ===\n";
        for (size_t i = 0; i < sensors.size(); ++i) {
            cout << "Sensor " << (i + 1) << " (" << sensors[i]->GetSensorType() << "): "
                 << sensors[i]->GetTemperatureC() << "°C\n";
        }
        cout << "====================================\n";
    }

    double GetAverageTemperature() {
        if (sensors.empty()) return 0.0;

        double sum = 0.0;
        for (const auto& sensor : sensors) {
            sum += sensor->GetTemperatureC();
        }
        return sum / sensors.size();
    }
};

int main() {
    TemperatureMonitor monitor;

    // Add native Celsius sensor
    monitor.AddSensor(make_unique<CelsiusSensor>());

    // Add Fahrenheit sensor through adapter
    FahrenheitSensor fahrenheitSensor;
    monitor.AddSensor(make_unique<FahrenheitAdapter>(&fahrenheitSensor));

    // Add Kelvin sensor through adapter
    KelvinSensor kelvinSensor;
    monitor.AddSensor(make_unique<KelvinAdapter>(&kelvinSensor));

    // Display all readings
    monitor.DisplayReadings();

    cout << "\nAverage Temperature: "
         << monitor.GetAverageTemperature() << "°C\n";

    return 0;
}
```

**Output:**
```
=== Temperature Monitor Readings ===
Sensor 1 (Celsius Sensor): 25°C
Sensor 2 (Fahrenheit Sensor (Adapted)): 25°C
Sensor 3 (Kelvin Sensor (Adapted)): 25°C
====================================

Average Temperature: 25°C
```

---

## 7. Two-Way Adapters

### Bidirectional Adapter:

```cpp
#include <iostream>
using namespace std;

class RoundPeg {
protected:
    double radius;

public:
    RoundPeg(double r) : radius(r) {}
    virtual double GetRadius() { return radius; }
};

class SquarePeg {
protected:
    double width;

public:
    SquarePeg(double w) : width(w) {}
    double GetWidth() { return width; }
};

// Two-way adapter
class SquarePegAdapter : public RoundPeg {
private:
    SquarePeg* squarePeg;

public:
    SquarePegAdapter(SquarePeg* peg)
        : RoundPeg(0), squarePeg(peg) {}

    double GetRadius() override {
        // Square diagonal / 2 = radius of enclosing circle
        return squarePeg->GetWidth() * 1.414 / 2.0;
    }

    double GetWidth() {
        return squarePeg->GetWidth();
    }
};

class RoundHole {
    double radius;

public:
    RoundHole(double r) : radius(r) {}

    bool Fits(RoundPeg* peg) {
        return peg->GetRadius() <= radius;
    }
};

int main() {
    RoundHole hole(5.0);

    RoundPeg roundPeg(4.0);
    cout << "Round peg (radius 4) fits: "
         << (hole.Fits(&roundPeg) ? "Yes" : "No") << "\n";

    SquarePeg smallSquare(5.0);
    SquarePeg largeSquare(10.0);

    SquarePegAdapter smallAdapter(&smallSquare);
    SquarePegAdapter largeAdapter(&largeSquare);

    cout << "Small square peg (width 5) fits: "
         << (hole.Fits(&smallAdapter) ? "Yes" : "No") << "\n";

    cout << "Large square peg (width 10) fits: "
         << (hole.Fits(&largeAdapter) ? "Yes" : "No") << "\n";

    return 0;
}
```

---

## 8. Advantages and Disadvantages

### Advantages:

1. ✅ **Reuse Existing Code**: Don't modify legacy code
2. ✅ **Single Responsibility**: Separates interface conversion
3. ✅ **Flexibility**: Can introduce new adapters without changing client
4. ✅ **Open/Closed**: Open for extension, closed for modification

### Disadvantages:

1. ❌ **Complexity**: Adds extra layer
2. ❌ **Performance**: Slight overhead from indirection
3. ❌ **Sometimes Simple Alternative**: Sometimes easier to modify interface directly

---

## 9. When to Use

### Use Adapter When:
- ✅ Want to use existing class with incompatible interface
- ✅ Need to create reusable class with unrelated classes
- ✅ Integrating third-party libraries
- ✅ Legacy code integration

### Don't Use When:
- ❌ You can modify the original interface
- ❌ Only one adaptation needed (might be overkill)
- ❌ Performance is critical (adds indirection)

---

## 10. Exercises

### Exercise 1: Database Adapter
Create adapters for different database systems (MySQL, PostgreSQL, SQLite) to work with a uniform interface.

### Exercise 2: File Format Adapter
Build adapters to read different file formats (XML, JSON, CSV) through a common interface.

### Exercise 3: Logging Adapter
Adapt different logging libraries (custom logger, syslog, Windows Event Log) to a common logging interface.

---

## Key Takeaways

1. ✅ Adapter **converts interfaces** to make them compatible
2. ✅ Also known as **Wrapper** pattern
3. ✅ **Object Adapter** (composition) is preferred over Class Adapter (inheritance)
4. ✅ Great for **legacy code integration**
5. ✅ Useful for **third-party libraries**
6. ✅ Follows **Single Responsibility** and **Open/Closed** principles
7. ✅ **Two-way adapters** provide bidirectional compatibility
8. ✅ Common in **real-world integration** scenarios

---

## Next Lesson Preview

In **Lesson 68**, we'll learn about the **Bridge Pattern**:
- Separating abstraction from implementation
- Avoiding cartesian product problem
- Platform-independent abstractions
- Multiple dimensions of variation

---

## Summary Checklist

Before moving to Lesson 68, ensure you can:

- [ ] Explain the Adapter pattern
- [ ] Distinguish Object vs Class adapter
- [ ] Implement adapters for legacy code
- [ ] Create two-way adapters
- [ ] Identify when to use Adapter
- [ ] Integrate third-party libraries with adapters
- [ ] Apply Single Responsibility with adapters

**Great progress!** Adapter pattern mastered!

Next: **Bridge Pattern** - separating abstraction from implementation!
