# Lesson 12: Enumerations and Type Definitions

**Duration**: 2 hours
**Prerequisite**: Lesson 11 - Structures and Unions

---

## Table of Contents
1. [Introduction to Enumerations](#introduction)
2. [Basic Enumerations](#basic-enums)
3. [Scoped Enumerations (enum class)](#scoped-enums)
4. [Enumerations with Specific Values](#enum-values)
5. [Type Aliases - typedef](#typedef)
6. [Type Aliases - using (C++11)](#using-aliases)
7. [Enums with Structures](#enums-structures)
8. [Practical Exercises](#exercises)
9. [Complete Projects](#projects)

---

## 1. Introduction to Enumerations {#introduction}

### What is an Enumeration?

An **enumeration** (enum) is a user-defined type consisting of a set of named integer constants.

**Why Use Enumerations?**
- Make code more readable
- Prevent invalid values
- Type safety
- Self-documenting code

### Example Without Enums (Bad)

```cpp
#include <iostream>

int main() {
    int day = 2;  // What does 2 mean?

    if (day == 0) {
        std::cout << "Sunday" << std::endl;
    } else if (day == 1) {
        std::cout << "Monday" << std::endl;
    } else if (day == 2) {
        std::cout << "Tuesday" << std::endl;
    }

    // Easy to make mistakes:
    day = 999;  // Invalid, but compiles!

    return 0;
}
```

### Example With Enums (Good)

```cpp
#include <iostream>

enum Day {
    SUNDAY,    // 0
    MONDAY,    // 1
    TUESDAY,   // 2
    WEDNESDAY, // 3
    THURSDAY,  // 4
    FRIDAY,    // 5
    SATURDAY   // 6
};

int main() {
    Day today = TUESDAY;  // Clear and readable!

    if (today == TUESDAY) {
        std::cout << "It's Tuesday!" << std::endl;
    }

    return 0;
}
```

---

## 2. Basic Enumerations {#basic-enums}

### Defining and Using Enums

```cpp
#include <iostream>

enum Color {
    RED,     // 0
    GREEN,   // 1
    BLUE     // 2
};

int main() {
    Color myColor = RED;

    std::cout << "Color value: " << myColor << std::endl;  // Outputs: 0

    if (myColor == RED) {
        std::cout << "The color is red!" << std::endl;
    }

    return 0;
}
```

**Output:**
```
Color value: 0
The color is red!
```

### Enum Values are Integers

```cpp
#include <iostream>

enum Status {
    PENDING,   // 0
    RUNNING,   // 1
    COMPLETED  // 2
};

int main() {
    Status s1 = PENDING;
    Status s2 = RUNNING;
    Status s3 = COMPLETED;

    std::cout << "PENDING = " << PENDING << std::endl;
    std::cout << "RUNNING = " << RUNNING << std::endl;
    std::cout << "COMPLETED = " << COMPLETED << std::endl;

    // Can assign enum to int
    int value = s2;
    std::cout << "s2 as int: " << value << std::endl;

    // Can't directly assign int to enum (needs cast)
    // s1 = 1;  // Error!
    s1 = static_cast<Status>(1);  // OK
    std::cout << "s1 now: " << s1 << std::endl;

    return 0;
}
```

### Switch Statement with Enums

```cpp
#include <iostream>
#include <string>

enum TrafficLight {
    RED_LIGHT,
    YELLOW_LIGHT,
    GREEN_LIGHT
};

std::string getLightAction(TrafficLight light) {
    switch (light) {
        case RED_LIGHT:
            return "Stop";
        case YELLOW_LIGHT:
            return "Slow down";
        case GREEN_LIGHT:
            return "Go";
        default:
            return "Unknown";
    }
}

int main() {
    TrafficLight light = RED_LIGHT;

    std::cout << "Light is " << light << ": " << getLightAction(light) << std::endl;

    light = GREEN_LIGHT;
    std::cout << "Light is " << light << ": " << getLightAction(light) << std::endl;

    return 0;
}
```

---

## 3. Scoped Enumerations (enum class) {#scoped-enums}

### Problems with Traditional Enums

```cpp
#include <iostream>

enum Color {
    RED,
    GREEN,
    BLUE
};

enum Fruit {
    APPLE,
    BANANA,
    // RED  // Error! RED already defined
};

int main() {
    Color c = RED;
    // int x = RED;  // Implicit conversion - can cause issues

    return 0;
}
```

### Scoped Enums (C++11) - Better

```cpp
#include <iostream>

enum class Color {
    Red,
    Green,
    Blue
};

enum class Fruit {
    Apple,
    Banana,
    Red  // OK! No conflict
};

int main() {
    Color c = Color::Red;        // Must use scope
    Fruit f = Fruit::Red;        // Different from Color::Red

    // int x = c;  // Error! No implicit conversion
    int x = static_cast<int>(c);  // Must explicitly cast

    std::cout << "Color value: " << x << std::endl;

    // if (c == f)  // Error! Different types
    if (c == Color::Red) {  // OK
        std::cout << "Color is Red" << std::endl;
    }

    return 0;
}
```

### Benefits of enum class

```cpp
#include <iostream>

enum class FileMode {
    Read,
    Write,
    Append
};

enum class Direction {
    North,
    South,
    East,
    West
};

void openFile(FileMode mode) {
    switch (mode) {
        case FileMode::Read:
            std::cout << "Opening file for reading\n";
            break;
        case FileMode::Write:
            std::cout << "Opening file for writing\n";
            break;
        case FileMode::Append:
            std::cout << "Opening file for appending\n";
            break;
    }
}

int main() {
    FileMode mode = FileMode::Read;
    openFile(mode);

    // Type safe - can't mix different enum types
    Direction dir = Direction::North;
    // openFile(dir);  // Error! Wrong type

    return 0;
}
```

---

## 4. Enumerations with Specific Values {#enum-values}

### Custom Enum Values

```cpp
#include <iostream>

enum HTTPStatus {
    OK = 200,
    NOT_FOUND = 404,
    SERVER_ERROR = 500
};

enum BitFlags {
    FLAG_NONE = 0,
    FLAG_READ = 1,      // 0001
    FLAG_WRITE = 2,     // 0010
    FLAG_EXECUTE = 4,   // 0100
    FLAG_ALL = 7        // 0111
};

int main() {
    HTTPStatus status = OK;
    std::cout << "HTTP Status: " << status << std::endl;

    status = NOT_FOUND;
    std::cout << "HTTP Status: " << status << std::endl;

    // Bit flags
    int permissions = FLAG_READ | FLAG_WRITE;
    std::cout << "Permissions: " << permissions << std::endl;

    if (permissions & FLAG_WRITE) {
        std::cout << "Has write permission\n";
    }

    return 0;
}
```

### Sequential Values After Custom Value

```cpp
#include <iostream>

enum Level {
    BEGINNER = 1,
    INTERMEDIATE,  // 2 (automatic)
    ADVANCED,      // 3 (automatic)
    EXPERT         // 4 (automatic)
};

enum Priority {
    LOW = 10,
    MEDIUM = 20,
    HIGH = 30,
    CRITICAL = 100
};

int main() {
    Level level = INTERMEDIATE;
    Priority priority = HIGH;

    std::cout << "Level: " << level << std::endl;
    std::cout << "Priority: " << priority << std::endl;

    return 0;
}
```

### Underlying Type for enum class

```cpp
#include <iostream>
#include <cstdint>

// Specify underlying type
enum class ErrorCode : uint8_t {
    Success = 0,
    FileNotFound = 1,
    PermissionDenied = 2,
    NetworkError = 3
};

enum class LargeEnum : int64_t {
    VeryLargeValue = 10000000000LL
};

int main() {
    ErrorCode error = ErrorCode::Success;

    std::cout << "Size of ErrorCode: " << sizeof(ErrorCode) << " byte(s)\n";
    std::cout << "Size of LargeEnum: " << sizeof(LargeEnum) << " bytes\n";

    std::cout << "Error value: " << static_cast<int>(error) << std::endl;

    return 0;
}
```

---

## 5. Type Aliases - typedef {#typedef}

### Basic typedef

```cpp
#include <iostream>
#include <string>

// Create alias for unsigned int
typedef unsigned int uint;

// Create alias for string
typedef std::string String;

// Pointer alias
typedef int* IntPtr;

// Function pointer alias
typedef void (*FunctionPtr)(int);

void myFunction(int x) {
    std::cout << "Value: " << x << std::endl;
}

int main() {
    uint age = 25;
    String name = "Alice";
    IntPtr ptr = &age;

    std::cout << "Name: " << name << std::endl;
    std::cout << "Age: " << age << std::endl;
    std::cout << "Pointer value: " << *ptr << std::endl;

    FunctionPtr func = myFunction;
    func(42);

    return 0;
}
```

### typedef with Structures

```cpp
#include <iostream>
#include <string>

// Without typedef
struct StudentStruct {
    std::string name;
    int id;
};

// With typedef
typedef struct {
    std::string name;
    int id;
} Student;

// Or combine:
typedef struct EmployeeStruct {
    std::string name;
    int id;
    double salary;
} Employee;

int main() {
    // Without typedef - must use "struct" keyword in C (not required in C++)
    StudentStruct s1 = {"Alice", 101};

    // With typedef - cleaner
    Student s2 = {"Bob", 102};
    Employee e1 = {"Carol", 201, 50000.0};

    std::cout << s2.name << ": " << s2.id << std::endl;
    std::cout << e1.name << ": " << e1.salary << std::endl;

    return 0;
}
```

---

## 6. Type Aliases - using (C++11) {#using-aliases}

### Modern Type Aliases with using

```cpp
#include <iostream>
#include <vector>
#include <map>
#include <string>

// Modern style (preferred in C++11+)
using uint = unsigned int;
using String = std::string;
using IntVector = std::vector<int>;
using StringMap = std::map<std::string, std::string>;

// Template alias (only possible with 'using', not 'typedef')
template<typename T>
using Vector = std::vector<T>;

template<typename K, typename V>
using Map = std::map<K, V>;

int main() {
    uint age = 30;
    String name = "Alice";

    IntVector numbers = {1, 2, 3, 4, 5};
    StringMap dict = {{"hello", "world"}, {"foo", "bar"}};

    // Template aliases
    Vector<double> prices = {9.99, 19.99, 29.99};
    Map<int, std::string> users = {{1, "Alice"}, {2, "Bob"}};

    std::cout << "Name: " << name << std::endl;
    std::cout << "First number: " << numbers[0] << std::endl;
    std::cout << "First price: " << prices[0] << std::endl;

    return 0;
}
```

### typedef vs using Comparison

```cpp
#include <iostream>
#include <functional>

// Function pointers
typedef void (*FuncPtr1)(int);
using FuncPtr2 = void(*)(int);

// Arrays
typedef int IntArray[10];
using IntArray2 = int[10];

// Complex types
typedef std::function<void(int)> Callback1;
using Callback2 = std::function<void(int)>;

// Template alias - ONLY with 'using'
template<typename T>
using Ptr = T*;

int main() {
    // Using template alias
    Ptr<int> intPtr;
    Ptr<double> doublePtr;

    int x = 42;
    intPtr = &x;

    std::cout << "Value: " << *intPtr << std::endl;

    return 0;
}
```

---

## 7. Enums with Structures {#enums-structures}

### Combining Enums and Structures

```cpp
#include <iostream>
#include <string>

enum class TaskStatus {
    Pending,
    InProgress,
    Completed,
    Cancelled
};

struct Task {
    int id;
    std::string description;
    TaskStatus status;
};

std::string statusToString(TaskStatus status) {
    switch (status) {
        case TaskStatus::Pending:
            return "Pending";
        case TaskStatus::InProgress:
            return "In Progress";
        case TaskStatus::Completed:
            return "Completed";
        case TaskStatus::Cancelled:
            return "Cancelled";
        default:
            return "Unknown";
    }
}

void displayTask(const Task& task) {
    std::cout << "Task #" << task.id << ": " << task.description << std::endl;
    std::cout << "Status: " << statusToString(task.status) << std::endl;
}

int main() {
    Task task1 = {1, "Write documentation", TaskStatus::InProgress};
    Task task2 = {2, "Fix bug #42", TaskStatus::Completed};
    Task task3 = {3, "Review code", TaskStatus::Pending};

    displayTask(task1);
    std::cout << std::endl;
    displayTask(task2);
    std::cout << std::endl;
    displayTask(task3);

    return 0;
}
```

### Complex Example with Multiple Enums

```cpp
#include <iostream>
#include <string>

enum class UserRole {
    Guest,
    User,
    Moderator,
    Admin
};

enum class AccountStatus {
    Active,
    Suspended,
    Deleted
};

struct User {
    int id;
    std::string username;
    UserRole role;
    AccountStatus status;
};

bool canModerate(const User& user) {
    return user.status == AccountStatus::Active &&
           (user.role == UserRole::Moderator || user.role == UserRole::Admin);
}

bool canDeleteUsers(const User& user) {
    return user.status == AccountStatus::Active &&
           user.role == UserRole::Admin;
}

std::string roleToString(UserRole role) {
    switch (role) {
        case UserRole::Guest: return "Guest";
        case UserRole::User: return "User";
        case UserRole::Moderator: return "Moderator";
        case UserRole::Admin: return "Admin";
        default: return "Unknown";
    }
}

int main() {
    User user1 = {1, "alice", UserRole::Admin, AccountStatus::Active};
    User user2 = {2, "bob", UserRole::User, AccountStatus::Active};
    User user3 = {3, "carol", UserRole::Moderator, AccountStatus::Suspended};

    std::cout << user1.username << " (" << roleToString(user1.role) << ")\n";
    std::cout << "Can moderate: " << (canModerate(user1) ? "Yes" : "No") << std::endl;
    std::cout << "Can delete users: " << (canDeleteUsers(user1) ? "Yes" : "No") << std::endl;

    std::cout << "\n" << user3.username << " (" << roleToString(user3.role) << ")\n";
    std::cout << "Can moderate: " << (canModerate(user3) ? "Yes" : "No") << std::endl;

    return 0;
}
```

---

## 8. Practical Exercises {#exercises}

### Exercise 1: Day of Week Calculator

```cpp
#include <iostream>
#include <string>

enum class DayOfWeek {
    Monday,
    Tuesday,
    Wednesday,
    Thursday,
    Friday,
    Saturday,
    Sunday
};

std::string dayName(DayOfWeek day) {
    switch (day) {
        case DayOfWeek::Monday: return "Monday";
        case DayOfWeek::Tuesday: return "Tuesday";
        case DayOfWeek::Wednesday: return "Wednesday";
        case DayOfWeek::Thursday: return "Thursday";
        case DayOfWeek::Friday: return "Friday";
        case DayOfWeek::Saturday: return "Saturday";
        case DayOfWeek::Sunday: return "Sunday";
        default: return "Unknown";
    }
}

bool isWeekend(DayOfWeek day) {
    return day == DayOfWeek::Saturday || day == DayOfWeek::Sunday;
}

bool isWorkday(DayOfWeek day) {
    return !isWeekend(day);
}

DayOfWeek nextDay(DayOfWeek day) {
    int current = static_cast<int>(day);
    int next = (current + 1) % 7;
    return static_cast<DayOfWeek>(next);
}

int main() {
    DayOfWeek today = DayOfWeek::Friday;

    std::cout << "Today is: " << dayName(today) << std::endl;
    std::cout << "Is weekend? " << (isWeekend(today) ? "Yes" : "No") << std::endl;
    std::cout << "Is workday? " << (isWorkday(today) ? "Yes" : "No") << std::endl;

    DayOfWeek tomorrow = nextDay(today);
    std::cout << "Tomorrow is: " << dayName(tomorrow) << std::endl;
    std::cout << "Is weekend? " << (isWeekend(tomorrow) ? "Yes" : "No") << std::endl;

    return 0;
}
```

### Exercise 2: State Machine

```cpp
#include <iostream>
#include <string>

enum class State {
    Idle,
    Running,
    Paused,
    Stopped,
    Error
};

enum class Event {
    Start,
    Pause,
    Resume,
    Stop,
    ErrorOccurred,
    Reset
};

struct Machine {
    State currentState;
    std::string name;
};

std::string stateToString(State state) {
    switch (state) {
        case State::Idle: return "Idle";
        case State::Running: return "Running";
        case State::Paused: return "Paused";
        case State::Stopped: return "Stopped";
        case State::Error: return "Error";
        default: return "Unknown";
    }
}

void handleEvent(Machine& machine, Event event) {
    std::cout << "\nEvent: ";
    switch (event) {
        case Event::Start: std::cout << "Start"; break;
        case Event::Pause: std::cout << "Pause"; break;
        case Event::Resume: std::cout << "Resume"; break;
        case Event::Stop: std::cout << "Stop"; break;
        case Event::ErrorOccurred: std::cout << "Error"; break;
        case Event::Reset: std::cout << "Reset"; break;
    }
    std::cout << std::endl;

    State previousState = machine.currentState;

    switch (machine.currentState) {
        case State::Idle:
            if (event == Event::Start) {
                machine.currentState = State::Running;
            }
            break;

        case State::Running:
            if (event == Event::Pause) {
                machine.currentState = State::Paused;
            } else if (event == Event::Stop) {
                machine.currentState = State::Stopped;
            } else if (event == Event::ErrorOccurred) {
                machine.currentState = State::Error;
            }
            break;

        case State::Paused:
            if (event == Event::Resume) {
                machine.currentState = State::Running;
            } else if (event == Event::Stop) {
                machine.currentState = State::Stopped;
            }
            break;

        case State::Stopped:
        case State::Error:
            if (event == Event::Reset) {
                machine.currentState = State::Idle;
            }
            break;
    }

    std::cout << stateToString(previousState) << " -> "
              << stateToString(machine.currentState) << std::endl;
}

int main() {
    Machine machine = {State::Idle, "MyMachine"};

    std::cout << "Initial state: " << stateToString(machine.currentState) << std::endl;

    handleEvent(machine, Event::Start);
    handleEvent(machine, Event::Pause);
    handleEvent(machine, Event::Resume);
    handleEvent(machine, Event::ErrorOccurred);
    handleEvent(machine, Event::Reset);
    handleEvent(machine, Event::Start);
    handleEvent(machine, Event::Stop);

    return 0;
}
```

---

## 9. Complete Projects {#projects}

### Project 1: Card Game System

```cpp
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>

enum class Suit {
    Hearts,
    Diamonds,
    Clubs,
    Spades
};

enum class Rank {
    Two = 2,
    Three,
    Four,
    Five,
    Six,
    Seven,
    Eight,
    Nine,
    Ten,
    Jack,
    Queen,
    King,
    Ace
};

struct Card {
    Rank rank;
    Suit suit;
};

using Deck = std::vector<Card>;

std::string suitToString(Suit suit) {
    switch (suit) {
        case Suit::Hearts: return "♥";
        case Suit::Diamonds: return "♦";
        case Suit::Clubs: return "♣";
        case Suit::Spades: return "♠";
        default: return "?";
    }
}

std::string rankToString(Rank rank) {
    switch (rank) {
        case Rank::Two: return "2";
        case Rank::Three: return "3";
        case Rank::Four: return "4";
        case Rank::Five: return "5";
        case Rank::Six: return "6";
        case Rank::Seven: return "7";
        case Rank::Eight: return "8";
        case Rank::Nine: return "9";
        case Rank::Ten: return "10";
        case Rank::Jack: return "J";
        case Rank::Queen: return "Q";
        case Rank::King: return "K";
        case Rank::Ace: return "A";
        default: return "?";
    }
}

void displayCard(const Card& card) {
    std::cout << rankToString(card.rank) << suitToString(card.suit);
}

Deck createDeck() {
    Deck deck;
    for (int s = 0; s < 4; s++) {
        for (int r = 2; r <= 14; r++) {
            deck.push_back({static_cast<Rank>(r), static_cast<Suit>(s)});
        }
    }
    return deck;
}

void shuffleDeck(Deck& deck) {
    std::random_shuffle(deck.begin(), deck.end());
}

void displayDeck(const Deck& deck) {
    for (size_t i = 0; i < deck.size(); i++) {
        displayCard(deck[i]);
        std::cout << " ";
        if ((i + 1) % 13 == 0) {
            std::cout << std::endl;
        }
    }
    std::cout << std::endl;
}

Card dealCard(Deck& deck) {
    Card card = deck.back();
    deck.pop_back();
    return card;
}

int main() {
    std::srand(std::time(0));

    std::cout << "=== Card Game System ===\n\n";

    Deck deck = createDeck();

    std::cout << "New deck (" << deck.size() << " cards):\n";
    displayDeck(deck);

    std::cout << "\nShuffling...\n\n";
    shuffleDeck(deck);

    std::cout << "Shuffled deck:\n";
    displayDeck(deck);

    std::cout << "\nDealing 5 cards:\n";
    Deck hand;
    for (int i = 0; i < 5; i++) {
        hand.push_back(dealCard(deck));
    }

    for (const auto& card : hand) {
        displayCard(card);
        std::cout << " ";
    }
    std::cout << "\n\nRemaining cards in deck: " << deck.size() << std::endl;

    return 0;
}
```

### Project 2: Traffic Light Simulation

```cpp
#include <iostream>
#include <string>
#include <thread>
#include <chrono>

enum class LightState {
    Red,
    Yellow,
    Green
};

enum class Direction {
    NorthSouth,
    EastWest
};

struct TrafficLight {
    Direction direction;
    LightState state;
    int timer;
};

std::string stateToString(LightState state) {
    switch (state) {
        case LightState::Red: return "🔴 RED   ";
        case LightState::Yellow: return "🟡 YELLOW";
        case LightState::Green: return "🟢 GREEN ";
        default: return "UNKNOWN";
    }
}

std::string directionToString(Direction dir) {
    switch (dir) {
        case Direction::NorthSouth: return "North-South";
        case Direction::EastWest: return "East-West  ";
        default: return "Unknown    ";
    }
}

void displayLights(const TrafficLight& light1, const TrafficLight& light2) {
    std::cout << "\r"  // Carriage return to overwrite line
              << directionToString(light1.direction) << ": "
              << stateToString(light1.state) << " (" << light1.timer << "s)  |  "
              << directionToString(light2.direction) << ": "
              << stateToString(light2.state) << " (" << light2.timer << "s)  "
              << std::flush;
}

LightState nextState(LightState current) {
    switch (current) {
        case LightState::Red:
            return LightState::Green;
        case LightState::Green:
            return LightState::Yellow;
        case LightState::Yellow:
            return LightState::Red;
        default:
            return LightState::Red;
    }
}

int getStateDuration(LightState state) {
    switch (state) {
        case LightState::Red:
            return 8;
        case LightState::Green:
            return 10;
        case LightState::Yellow:
            return 2;
        default:
            return 0;
    }
}

int main() {
    TrafficLight nsLight = {Direction::NorthSouth, LightState::Green, 10};
    TrafficLight ewLight = {Direction::EastWest, LightState::Red, 8};

    std::cout << "=== Traffic Light Simulation ===\n";
    std::cout << "(Press Ctrl+C to stop)\n\n";

    int cycles = 0;
    const int MAX_CYCLES = 20;

    while (cycles < MAX_CYCLES) {
        displayLights(nsLight, ewLight);

        std::this_thread::sleep_for(std::chrono::seconds(1));

        nsLight.timer--;
        ewLight.timer--;

        if (nsLight.timer <= 0) {
            nsLight.state = nextState(nsLight.state);
            nsLight.timer = getStateDuration(nsLight.state);
        }

        if (ewLight.timer <= 0) {
            ewLight.state = nextState(ewLight.state);
            ewLight.timer = getStateDuration(ewLight.state);
        }

        cycles++;
    }

    std::cout << "\n\nSimulation complete!\n";

    return 0;
}
```

### Project 3: Order Management System

```cpp
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>

enum class OrderStatus {
    Pending,
    Processing,
    Shipped,
    Delivered,
    Cancelled
};

enum class PaymentMethod {
    CreditCard,
    DebitCard,
    PayPal,
    Cash
};

enum class Priority {
    Low = 1,
    Normal = 2,
    High = 3,
    Urgent = 4
};

struct Order {
    int id;
    std::string customerName;
    double amount;
    OrderStatus status;
    PaymentMethod payment;
    Priority priority;
};

using OrderList = std::vector<Order>;

std::string statusToString(OrderStatus status) {
    switch (status) {
        case OrderStatus::Pending: return "Pending   ";
        case OrderStatus::Processing: return "Processing";
        case OrderStatus::Shipped: return "Shipped   ";
        case OrderStatus::Delivered: return "Delivered ";
        case OrderStatus::Cancelled: return "Cancelled ";
        default: return "Unknown   ";
    }
}

std::string paymentToString(PaymentMethod payment) {
    switch (payment) {
        case PaymentMethod::CreditCard: return "Credit Card";
        case PaymentMethod::DebitCard: return "Debit Card ";
        case PaymentMethod::PayPal: return "PayPal     ";
        case PaymentMethod::Cash: return "Cash       ";
        default: return "Unknown    ";
    }
}

std::string priorityToString(Priority priority) {
    switch (priority) {
        case Priority::Low: return "Low   ";
        case Priority::Normal: return "Normal";
        case Priority::High: return "High  ";
        case Priority::Urgent: return "Urgent";
        default: return "N/A   ";
    }
}

void displayOrder(const Order& order) {
    std::cout << std::setw(5) << order.id << " | ";
    std::cout << std::setw(20) << std::left << order.customerName << " | ";
    std::cout << "$" << std::setw(8) << std::right << std::fixed << std::setprecision(2) << order.amount << " | ";
    std::cout << statusToString(order.status) << " | ";
    std::cout << paymentToString(order.payment) << " | ";
    std::cout << priorityToString(order.priority) << std::endl;
}

void displayAllOrders(const OrderList& orders) {
    std::cout << "\n=== Order Management System ===\n";
    std::cout << std::setw(5) << "ID" << " | ";
    std::cout << std::setw(20) << std::left << "Customer" << " | ";
    std::cout << std::setw(9) << std::right << "Amount" << " | ";
    std::cout << "Status     | Payment      | Priority\n";
    std::cout << std::string(90, '-') << std::endl;

    for (const auto& order : orders) {
        displayOrder(order);
    }
}

void filterByStatus(const OrderList& orders, OrderStatus status) {
    std::cout << "\n=== Orders with status: " << statusToString(status) << "===\n";
    bool found = false;

    for (const auto& order : orders) {
        if (order.status == status) {
            displayOrder(order);
            found = true;
        }
    }

    if (!found) {
        std::cout << "No orders found with this status.\n";
    }
}

double getTotalRevenue(const OrderList& orders) {
    double total = 0.0;
    for (const auto& order : orders) {
        if (order.status != OrderStatus::Cancelled) {
            total += order.amount;
        }
    }
    return total;
}

int main() {
    OrderList orders = {
        {1001, "Alice Johnson", 129.99, OrderStatus::Delivered, PaymentMethod::CreditCard, Priority::Normal},
        {1002, "Bob Smith", 59.99, OrderStatus::Shipped, PaymentMethod::PayPal, Priority::High},
        {1003, "Carol Davis", 249.99, OrderStatus::Processing, PaymentMethod::DebitCard, Priority::Urgent},
        {1004, "David Lee", 89.99, OrderStatus::Pending, PaymentMethod::CreditCard, Priority::Normal},
        {1005, "Eve Wilson", 199.99, OrderStatus::Cancelled, PaymentMethod::Cash, Priority::Low},
        {1006, "Frank Brown", 149.99, OrderStatus::Processing, PaymentMethod::CreditCard, Priority::High}
    };

    displayAllOrders(orders);

    filterByStatus(orders, OrderStatus::Processing);

    double revenue = getTotalRevenue(orders);
    std::cout << "\nTotal Revenue (excluding cancelled): $"
              << std::fixed << std::setprecision(2) << revenue << std::endl;

    return 0;
}
```

---

## Summary

In this lesson, you learned:

### Key Concepts

1. **Basic Enumerations**
   - Creating named constants
   - Making code more readable
   - Using enums in switch statements

2. **Scoped Enumerations (enum class)**
   - Type safety
   - Preventing name collisions
   - Explicit scoping required
   - No implicit conversions

3. **Custom Enum Values**
   - Setting specific integer values
   - Sequential auto-increment
   - Bit flags
   - Underlying type specification

4. **Type Aliases**
   - `typedef` (C-style)
   - `using` (modern C++11)
   - Template aliases
   - Simplifying complex types

5. **Enums with Structures**
   - Combining enums for state management
   - Multiple enums in one structure
   - Type-safe state machines

### Best Practices

- Prefer `enum class` over plain `enum`
- Use meaningful enum names (PascalCase for values)
- Provide conversion functions (enum to string)
- Use enums instead of magic numbers
- Prefer `using` over `typedef` in modern C++
- Use underlying type when size matters

---

## Next Lesson

**Lesson 13: File I/O Operations**

You'll learn about:
- Reading from files
- Writing to files
- File streams (ifstream, ofstream, fstream)
- Binary file operations
- Error handling in file I/O

---

## Quick Reference

```cpp
// Basic enum
enum Color {
    RED, GREEN, BLUE
};

// Scoped enum (preferred)
enum class Status {
    Active,
    Inactive
};

// Custom values
enum ErrorCode {
    SUCCESS = 0,
    ERROR = -1
};

// With underlying type
enum class Flag : uint8_t {
    None = 0,
    Read = 1
};

// Type aliases
typedef unsigned int uint;
using String = std::string;

// Template alias
template<typename T>
using Ptr = T*;
```

**Compilation:**
```bash
g++ -std=c++20 -Wall lesson12.cpp -o lesson12
./lesson12
```

---

**End of Lesson 12**
