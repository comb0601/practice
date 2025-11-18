# Lesson 22: Default Arguments

## Duration
2 hours

## Learning Objectives
By the end of this lesson, you will be able to:
- Understand and use default function arguments
- Combine default arguments with function overloading
- Follow best practices for default argument placement
- Create flexible and user-friendly function interfaces
- Understand the rules and limitations of default arguments
- Debug common default argument issues

## Introduction

Default arguments allow you to specify default values for function parameters, making functions more flexible and easier to use. They enable you to call a function with fewer arguments than it has parameters, with missing arguments taking their default values.

## 1. Basic Default Arguments

### Simple Default Arguments

```cpp
#include <iostream>
#include <string>
using namespace std;

// Function with default arguments
void greet(string name = "Guest", string greeting = "Hello") {
    cout << greeting << ", " << name << "!" << endl;
}

// Function with mix of required and default parameters
void displayInfo(string title, int age = 0, string country = "Unknown") {
    cout << "Title: " << title << endl;
    cout << "Age: " << age << endl;
    cout << "Country: " << country << endl;
}

// Function with all default parameters
void printSeparator(char ch = '-', int count = 40) {
    for (int i = 0; i < count; i++) {
        cout << ch;
    }
    cout << endl;
}

int main() {
    // Call with all arguments
    greet("Alice", "Hi");

    // Call with one argument (uses default greeting)
    greet("Bob");

    // Call with no arguments (uses all defaults)
    greet();

    cout << endl;

    // displayInfo with different argument combinations
    displayInfo("Mr. Smith");
    cout << endl;

    displayInfo("Ms. Johnson", 30);
    cout << endl;

    displayInfo("Dr. Brown", 45, "USA");
    cout << endl;

    // printSeparator variations
    printSeparator();
    printSeparator('=');
    printSeparator('*', 50);

    return 0;
}
```

**Output:**
```
Hi, Alice!
Hello, Bob!
Hello, Guest!

Title: Mr. Smith
Age: 0
Country: Unknown

Title: Ms. Johnson
Age: 30
Country: Unknown

Title: Dr. Brown
Age: 45
Country: USA

----------------------------------------
========================================
**************************************************
```

## 2. Default Arguments Rules and Best Practices

### Placement Rules

```cpp
#include <iostream>
using namespace std;

// CORRECT: Default arguments are rightmost
void function1(int a, int b = 10, int c = 20) {
    cout << "a=" << a << ", b=" << b << ", c=" << c << endl;
}

// CORRECT: All parameters have defaults
void function2(int a = 1, int b = 2, int c = 3) {
    cout << "a=" << a << ", b=" << b << ", c=" << c << endl;
}

// INCORRECT: Cannot have non-default after default
// void function3(int a = 1, int b, int c = 3) { }  // Compilation error

// Default arguments in declaration, not definition
void function4(int a, int b = 10, int c = 20);

void function4(int a, int b, int c) {
    // No defaults here - they're in the declaration
    cout << "a=" << a << ", b=" << b << ", c=" << c << endl;
}

// Using expressions as defaults
int getDefault() {
    return 42;
}

void function5(int a, int b = getDefault(), int c = b * 2) {
    cout << "a=" << a << ", b=" << b << ", c=" << c << endl;
}

int main() {
    function1(5);           // a=5, b=10, c=20
    function1(5, 15);       // a=5, b=15, c=20
    function1(5, 15, 25);   // a=5, b=15, c=25

    cout << endl;

    function2();            // Uses all defaults
    function2(10);          // a=10, others default
    function2(10, 20);      // a=10, b=20, c=3
    function2(10, 20, 30);  // All specified

    cout << endl;

    function4(1);           // Uses declaration's defaults
    function4(1, 2);
    function4(1, 2, 3);

    cout << endl;

    function5(1);           // b=42, c=84

    return 0;
}
```

## 3. Default Arguments with Classes

### Constructor Default Arguments

```cpp
#include <iostream>
#include <string>
using namespace std;

class Rectangle {
private:
    double width;
    double height;
    string color;

public:
    // Constructor with default arguments
    Rectangle(double w = 1.0, double h = 1.0, string c = "white")
        : width(w), height(h), color(c) {
        cout << "Created " << color << " rectangle: "
             << width << " x " << height << endl;
    }

    double area() const {
        return width * height;
    }

    void display() const {
        cout << "Rectangle(" << width << " x " << height
             << ", " << color << ") = " << area() << " sq units" << endl;
    }
};

class BankAccount {
private:
    string accountNumber;
    double balance;
    double interestRate;

public:
    // Default arguments in constructor
    BankAccount(string accNum,
                double initialBalance = 0.0,
                double rate = 0.02)
        : accountNumber(accNum),
          balance(initialBalance),
          interestRate(rate) {
        cout << "Account " << accountNumber << " created with balance $"
             << balance << " at " << (rate * 100) << "%" << endl;
    }

    void deposit(double amount = 0.0) {
        if (amount > 0) {
            balance += amount;
            cout << "Deposited $" << amount << ". New balance: $" << balance << endl;
        }
    }

    void withdraw(double amount = 0.0) {
        if (amount > 0 && amount <= balance) {
            balance -= amount;
            cout << "Withdrew $" << amount << ". New balance: $" << balance << endl;
        }
    }

    void displayBalance() const {
        cout << "Account " << accountNumber << ": $" << balance << endl;
    }
};

int main() {
    // Rectangle with various constructor calls
    Rectangle r1;                           // 1x1 white
    Rectangle r2(5.0);                      // 5x1 white
    Rectangle r3(4.0, 6.0);                 // 4x6 white
    Rectangle r4(3.0, 7.0, "blue");         // 3x7 blue

    cout << endl;

    r1.display();
    r2.display();
    r3.display();
    r4.display();

    cout << endl;

    // BankAccount with default arguments
    BankAccount acc1("ACC001");             // $0 at 2%
    BankAccount acc2("ACC002", 1000.0);     // $1000 at 2%
    BankAccount acc3("ACC003", 5000.0, 0.05); // $5000 at 5%

    cout << endl;

    acc2.deposit(500);
    acc2.withdraw(200);
    acc2.displayBalance();

    return 0;
}
```

## 4. Default Arguments in Member Functions

```cpp
#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
using namespace std;

class Logger {
private:
    string prefix;
    vector<string> logs;

public:
    Logger(string p = "[LOG]") : prefix(p) {}

    // Default arguments in member functions
    void log(string message, string level = "INFO", bool timestamp = true) {
        string logEntry;

        if (timestamp) {
            // Simplified timestamp
            logEntry = "[2024-01-15 10:30:00] ";
        }

        logEntry += "[" + level + "] " + prefix + " " + message;
        logs.push_back(logEntry);
        cout << logEntry << endl;
    }

    void error(string message, bool critical = false) {
        string level = critical ? "CRITICAL" : "ERROR";
        log(message, level);
    }

    void warning(string message) {
        log(message, "WARNING");
    }

    void info(string message) {
        log(message, "INFO");
    }

    void displayLogs(int count = -1) {
        cout << "\n=== Log History ===" << endl;
        int start = (count > 0) ? max(0, (int)logs.size() - count) : 0;

        for (size_t i = start; i < logs.size(); i++) {
            cout << logs[i] << endl;
        }
    }
};

class DataFormatter {
public:
    // Multiple default arguments
    string formatNumber(double num,
                       int width = 10,
                       int precision = 2,
                       char fill = ' ') {
        ostringstream oss;
        oss << fixed << setprecision(precision)
            << setw(width) << setfill(fill) << num;
        return oss.str();
    }

    string formatCurrency(double amount,
                         string symbol = "$",
                         bool spaceAfterSymbol = false) {
        string space = spaceAfterSymbol ? " " : "";
        ostringstream oss;
        oss << symbol << space << fixed << setprecision(2) << amount;
        return oss.str();
    }

    string formatPercentage(double value,
                           int decimals = 1,
                           bool includeSymbol = true) {
        ostringstream oss;
        oss << fixed << setprecision(decimals) << (value * 100);
        if (includeSymbol) {
            oss << "%";
        }
        return oss.str();
    }
};

int main() {
    Logger logger("[APP]");

    logger.info("Application started");
    logger.warning("Low memory");
    logger.error("File not found");
    logger.error("System failure", true);  // Critical error

    logger.displayLogs();      // Display all logs
    cout << "\nLast 2 logs:" << endl;
    logger.displayLogs(2);     // Display last 2 logs

    cout << "\n=== Data Formatting ===" << endl;
    DataFormatter formatter;

    cout << "Number: " << formatter.formatNumber(123.456) << endl;
    cout << "Number: " << formatter.formatNumber(123.456, 15) << endl;
    cout << "Number: " << formatter.formatNumber(123.456, 15, 4) << endl;
    cout << "Number: " << formatter.formatNumber(123.456, 15, 4, '0') << endl;

    cout << "\nCurrency: " << formatter.formatCurrency(1234.56) << endl;
    cout << "Currency: " << formatter.formatCurrency(1234.56, "€") << endl;
    cout << "Currency: " << formatter.formatCurrency(1234.56, "£", true) << endl;

    cout << "\nPercentage: " << formatter.formatPercentage(0.1234) << endl;
    cout << "Percentage: " << formatter.formatPercentage(0.1234, 2) << endl;
    cout << "Percentage: " << formatter.formatPercentage(0.1234, 3, false) << endl;

    return 0;
}
```

## 5. Combining Default Arguments with Overloading

```cpp
#include <iostream>
#include <string>
#include <vector>
using namespace std;

class TextProcessor {
public:
    // Overloaded functions with default arguments
    string process(const string& text, bool uppercase = false) {
        string result = text;
        if (uppercase) {
            for (char& c : result) {
                c = toupper(c);
            }
        }
        return result;
    }

    // Overload for vector processing
    vector<string> process(const vector<string>& texts,
                          bool uppercase = false,
                          bool trim = false) {
        vector<string> results;
        for (const auto& text : texts) {
            string processed = text;

            if (trim) {
                // Simple trim implementation
                size_t start = processed.find_first_not_of(" \t");
                size_t end = processed.find_last_not_of(" \t");
                if (start != string::npos && end != string::npos) {
                    processed = processed.substr(start, end - start + 1);
                }
            }

            if (uppercase) {
                for (char& c : processed) {
                    c = toupper(c);
                }
            }

            results.push_back(processed);
        }
        return results;
    }

    // Overload with different parameters
    int process(int number, int multiplier = 1, int offset = 0) {
        return number * multiplier + offset;
    }
};

int main() {
    TextProcessor processor;

    // String processing
    cout << processor.process("hello") << endl;
    cout << processor.process("hello", true) << endl;
    cout << endl;

    // Vector processing
    vector<string> texts = {"  hello  ", "world  ", "  C++  "};
    auto result1 = processor.process(texts);
    auto result2 = processor.process(texts, true);
    auto result3 = processor.process(texts, true, true);

    cout << "Normal: ";
    for (const auto& s : result1) cout << "[" << s << "] ";
    cout << endl;

    cout << "Uppercase: ";
    for (const auto& s : result2) cout << "[" << s << "] ";
    cout << endl;

    cout << "Uppercase + Trim: ";
    for (const auto& s : result3) cout << "[" << s << "] ";
    cout << endl << endl;

    // Number processing
    cout << processor.process(10) << endl;
    cout << processor.process(10, 5) << endl;
    cout << processor.process(10, 5, 3) << endl;

    return 0;
}
```

## 6. Advanced Default Arguments

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
using namespace std;

// Using nullptr as default
class DataContainer {
private:
    vector<int> data;

public:
    void addData(int value) {
        data.push_back(value);
    }

    // Default nullptr for optional callback
    void processData(function<void(int)> callback = nullptr,
                    bool ascending = true) {
        if (ascending) {
            sort(data.begin(), data.end());
        } else {
            sort(data.begin(), data.end(), greater<int>());
        }

        if (callback) {
            for (int val : data) {
                callback(val);
            }
        }
    }

    void display() const {
        cout << "Data: ";
        for (int val : data) {
            cout << val << " ";
        }
        cout << endl;
    }
};

// Using static member as default
class Configuration {
private:
    static const int DEFAULT_TIMEOUT = 30;
    static const int DEFAULT_RETRIES = 3;

public:
    static const int MAX_CONNECTIONS = 100;

    void connect(string server,
                int timeout = DEFAULT_TIMEOUT,
                int retries = DEFAULT_RETRIES,
                int maxConn = MAX_CONNECTIONS) {
        cout << "Connecting to " << server << endl;
        cout << "Timeout: " << timeout << "s" << endl;
        cout << "Retries: " << retries << endl;
        cout << "Max connections: " << maxConn << endl;
    }
};

// Default argument using previous parameter
void createRange(int start, int end, int step = 1) {
    cout << "Range from " << start << " to " << end
         << " with step " << step << ": ";

    for (int i = start; i <= end; i += step) {
        cout << i << " ";
    }
    cout << endl;
}

// Complex default expressions
struct Point {
    int x, y;
    Point(int xVal = 0, int yVal = 0) : x(xVal), y(yVal) {}
};

void drawLine(Point start = Point(),
             Point end = Point(100, 100),
             int thickness = 1,
             string color = "black") {
    cout << "Drawing line from (" << start.x << "," << start.y << ")"
         << " to (" << end.x << "," << end.y << ")"
         << " with thickness " << thickness
         << " and color " << color << endl;
}

int main() {
    DataContainer container;
    container.addData(50);
    container.addData(20);
    container.addData(80);
    container.addData(10);
    container.addData(40);

    cout << "=== Without callback ===" << endl;
    container.processData();
    container.display();

    cout << "\n=== With callback ===" << endl;
    container.processData([](int x) {
        cout << "Processing: " << x << endl;
    });

    cout << "\n=== Descending order ===" << endl;
    container.processData(nullptr, false);
    container.display();

    cout << "\n=== Configuration ===" << endl;
    Configuration config;
    config.connect("server1.example.com");
    cout << endl;
    config.connect("server2.example.com", 60);
    cout << endl;
    config.connect("server3.example.com", 60, 5);

    cout << "\n=== Range Creation ===" << endl;
    createRange(1, 10);
    createRange(0, 20, 2);
    createRange(10, 100, 10);

    cout << "\n=== Line Drawing ===" << endl;
    drawLine();
    drawLine(Point(10, 10));
    drawLine(Point(10, 10), Point(200, 200));
    drawLine(Point(10, 10), Point(200, 200), 3);
    drawLine(Point(10, 10), Point(200, 200), 3, "red");

    return 0;
}
```

## Exercises

### Exercise 1: HTTP Request Builder
Create a function that builds HTTP requests with default arguments for method, headers, timeout, and retry count.

### Exercise 2: Report Generator
Implement a class that generates reports with default formatting options (page size, orientation, font size, margins).

### Exercise 3: Database Query Builder
Create a query builder with default arguments for limit, offset, ordering, and filtering options.

### Exercise 4: Image Processor
Implement an image processing class with functions that have default arguments for quality, format, resize dimensions, and filters.

### Exercise 5: Game Character Creator
Create a character creator with default arguments for attributes, equipment, skills, and appearance options.

## Exercise Solutions

### Solution 1: HTTP Request Builder

```cpp
#include <iostream>
#include <string>
#include <map>
#include <vector>
using namespace std;

class HTTPRequest {
public:
    enum Method { GET, POST, PUT, DELETE };

private:
    Method method;
    string url;
    map<string, string> headers;
    string body;
    int timeout;
    int retries;

public:
    HTTPRequest(string requestUrl,
               Method requestMethod = GET,
               int requestTimeout = 30,
               int requestRetries = 3)
        : url(requestUrl),
          method(requestMethod),
          timeout(requestTimeout),
          retries(requestRetries) {

        // Default headers
        headers["User-Agent"] = "CustomClient/1.0";
        headers["Accept"] = "*/*";
    }

    void addHeader(string key, string value) {
        headers[key] = value;
    }

    void setBody(string content, string contentType = "application/json") {
        body = content;
        headers["Content-Type"] = contentType;
        headers["Content-Length"] = to_string(content.length());
    }

    void send(bool verbose = false,
             bool followRedirects = true,
             int maxRedirects = 5) {

        cout << "=== HTTP Request ===" << endl;
        cout << "URL: " << url << endl;
        cout << "Method: " << methodToString(method) << endl;
        cout << "Timeout: " << timeout << "s" << endl;
        cout << "Retries: " << retries << endl;
        cout << "Follow Redirects: " << (followRedirects ? "Yes" : "No") << endl;
        cout << "Max Redirects: " << maxRedirects << endl;

        if (verbose) {
            cout << "\nHeaders:" << endl;
            for (const auto& header : headers) {
                cout << "  " << header.first << ": " << header.second << endl;
            }

            if (!body.empty()) {
                cout << "\nBody:" << endl;
                cout << body << endl;
            }
        }

        cout << "===================" << endl << endl;
    }

private:
    string methodToString(Method m) {
        switch (m) {
            case GET: return "GET";
            case POST: return "POST";
            case PUT: return "PUT";
            case DELETE: return "DELETE";
            default: return "UNKNOWN";
        }
    }
};

int main() {
    // Simple GET request with all defaults
    HTTPRequest req1("https://api.example.com/users");
    req1.send();

    // POST request with custom timeout
    HTTPRequest req2("https://api.example.com/users", HTTPRequest::POST, 60);
    req2.setBody("{\"name\":\"John\",\"age\":30}");
    req2.send(true);

    // PUT request with custom timeout and retries
    HTTPRequest req3("https://api.example.com/users/123",
                    HTTPRequest::PUT, 45, 5);
    req3.setBody("{\"name\":\"Jane\",\"age\":25}");
    req3.addHeader("Authorization", "Bearer token123");
    req3.send(true, false);

    return 0;
}
```

### Solution 2: Report Generator

```cpp
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
using namespace std;

class ReportGenerator {
public:
    enum PageSize { A4, LETTER, LEGAL };
    enum Orientation { PORTRAIT, LANDSCAPE };

private:
    string title;
    vector<vector<string>> data;
    PageSize pageSize;
    Orientation orientation;
    int fontSize;
    int marginTop, marginBottom, marginLeft, marginRight;

public:
    ReportGenerator(string reportTitle,
                   PageSize size = A4,
                   Orientation orient = PORTRAIT,
                   int font = 12,
                   int margins = 20)
        : title(reportTitle),
          pageSize(size),
          orientation(orient),
          fontSize(font),
          marginTop(margins),
          marginBottom(margins),
          marginLeft(margins),
          marginRight(margins) {}

    void setMargins(int top = 20, int bottom = 20,
                   int left = 20, int right = 20) {
        marginTop = top;
        marginBottom = bottom;
        marginLeft = left;
        marginRight = right;
    }

    void addRow(vector<string> row) {
        data.push_back(row);
    }

    void generate(bool includeHeader = true,
                 bool includeFooter = true,
                 bool alternateRowColors = false,
                 string headerColor = "blue") {

        cout << "=== REPORT: " << title << " ===" << endl;
        cout << "Page Size: " << pageSizeToString() << endl;
        cout << "Orientation: " << orientationToString() << endl;
        cout << "Font Size: " << fontSize << "pt" << endl;
        cout << "Margins: T=" << marginTop << " B=" << marginBottom
             << " L=" << marginLeft << " R=" << marginRight << endl;
        cout << string(60, '=') << endl;

        if (includeHeader) {
            cout << "HEADER (Color: " << headerColor << ")" << endl;
            cout << "Generated: 2024-01-15 10:30:00" << endl;
            cout << string(60, '-') << endl;
        }

        for (size_t i = 0; i < data.size(); i++) {
            if (alternateRowColors && i % 2 == 1) {
                cout << "[GRAY] ";
            }

            for (const auto& cell : data[i]) {
                cout << setw(15) << left << cell;
            }
            cout << endl;
        }

        if (includeFooter) {
            cout << string(60, '-') << endl;
            cout << "FOOTER" << endl;
            cout << "Total Rows: " << data.size() << endl;
        }

        cout << string(60, '=') << endl << endl;
    }

private:
    string pageSizeToString() {
        switch (pageSize) {
            case A4: return "A4 (210x297mm)";
            case LETTER: return "Letter (8.5x11in)";
            case LEGAL: return "Legal (8.5x14in)";
            default: return "Unknown";
        }
    }

    string orientationToString() {
        return (orientation == PORTRAIT) ? "Portrait" : "Landscape";
    }
};

int main() {
    ReportGenerator report1("Sales Report");
    report1.addRow({"Name", "Department", "Sales"});
    report1.addRow({"John Doe", "Marketing", "$50,000"});
    report1.addRow({"Jane Smith", "Sales", "$75,000"});
    report1.addRow({"Bob Johnson", "IT", "$60,000"});

    report1.generate();

    ReportGenerator report2("Quarterly Report",
                          ReportGenerator::LETTER,
                          ReportGenerator::LANDSCAPE,
                          14);
    report2.setMargins(30, 30, 25, 25);
    report2.addRow({"Q1", "Q2", "Q3", "Q4"});
    report2.addRow({"$100K", "$120K", "$110K", "$130K"});

    report2.generate(true, true, true, "green");

    return 0;
}
```

### Solution 3: Database Query Builder

```cpp
#include <iostream>
#include <string>
#include <vector>
#include <map>
using namespace std;

class QueryBuilder {
private:
    string tableName;
    vector<string> columns;
    map<string, string> conditions;
    string orderBy;
    bool ascending;
    int limitValue;
    int offsetValue;

public:
    QueryBuilder(string table) : tableName(table),
                                ascending(true),
                                limitValue(-1),
                                offsetValue(0) {}

    QueryBuilder& select(vector<string> cols = {"*"}) {
        columns = cols;
        return *this;
    }

    QueryBuilder& where(string column, string value) {
        conditions[column] = value;
        return *this;
    }

    QueryBuilder& orderByColumn(string column = "id",
                               bool asc = true) {
        orderBy = column;
        ascending = asc;
        return *this;
    }

    QueryBuilder& limit(int count = 10) {
        limitValue = count;
        return *this;
    }

    QueryBuilder& offset(int count = 0) {
        offsetValue = count;
        return *this;
    }

    string build(bool prettify = false) {
        string query = "SELECT ";

        // Columns
        if (columns.empty()) {
            query += "*";
        } else {
            for (size_t i = 0; i < columns.size(); i++) {
                query += columns[i];
                if (i < columns.size() - 1) query += ", ";
            }
        }

        // Table
        query += " FROM " + tableName;

        // Conditions
        if (!conditions.empty()) {
            query += " WHERE ";
            size_t count = 0;
            for (const auto& cond : conditions) {
                query += cond.first + " = '" + cond.second + "'";
                if (++count < conditions.size()) query += " AND ";
            }
        }

        // Order By
        if (!orderBy.empty()) {
            query += " ORDER BY " + orderBy;
            query += ascending ? " ASC" : " DESC";
        }

        // Limit
        if (limitValue > 0) {
            query += " LIMIT " + to_string(limitValue);
        }

        // Offset
        if (offsetValue > 0) {
            query += " OFFSET " + to_string(offsetValue);
        }

        return query + ";";
    }

    void execute(bool showQuery = true,
                bool showResults = false,
                int resultCount = 5) {

        if (showQuery) {
            cout << "Query: " << build() << endl;
        }

        if (showResults) {
            cout << "Results (" << resultCount << " rows):" << endl;
            for (int i = 0; i < resultCount; i++) {
                cout << "  Row " << (i + 1) << ": [sample data]" << endl;
            }
        }

        cout << endl;
    }
};

int main() {
    // Simple query with defaults
    QueryBuilder query1("users");
    query1.select().execute();

    // Query with specific columns and limit
    QueryBuilder query2("products");
    query2.select({"name", "price", "stock"})
          .limit(20)
          .execute();

    // Complex query with all options
    QueryBuilder query3("orders");
    query3.select({"id", "customer", "total", "date"})
          .where("status", "completed")
          .where("total", ">1000")
          .orderByColumn("date", false)
          .limit(10)
          .offset(5)
          .execute(true, true);

    // Pagination example
    QueryBuilder query4("employees");
    query4.select({"id", "name", "department"})
          .orderByColumn("name")
          .limit(10)
          .offset(0)
          .execute();

    return 0;
}
```

### Solution 4: Image Processor

```cpp
#include <iostream>
#include <string>
using namespace std;

class ImageProcessor {
public:
    enum Format { JPG, PNG, BMP, WEBP };
    enum Filter { NONE, BLUR, SHARPEN, GRAYSCALE, SEPIA };

private:
    string filename;

public:
    ImageProcessor(string file) : filename(file) {}

    void resize(int width = -1,
               int height = -1,
               bool maintainAspectRatio = true,
               string resampleMethod = "bicubic") {

        cout << "Resizing " << filename << endl;

        if (width > 0 && height > 0) {
            cout << "  New size: " << width << "x" << height << endl;
        } else if (width > 0) {
            cout << "  New width: " << width << " (height auto)" << endl;
        } else if (height > 0) {
            cout << "  New height: " << height << " (width auto)" << endl;
        }

        cout << "  Maintain aspect ratio: " << (maintainAspectRatio ? "Yes" : "No") << endl;
        cout << "  Resample method: " << resampleMethod << endl;
        cout << endl;
    }

    void convert(Format format = JPG,
                int quality = 85,
                bool progressive = false,
                bool optimized = true) {

        cout << "Converting " << filename << endl;
        cout << "  Format: " << formatToString(format) << endl;
        cout << "  Quality: " << quality << "%" << endl;
        cout << "  Progressive: " << (progressive ? "Yes" : "No") << endl;
        cout << "  Optimized: " << (optimized ? "Yes" : "No") << endl;
        cout << endl;
    }

    void applyFilter(Filter filter = NONE,
                    int intensity = 50,
                    bool preserveColors = false) {

        cout << "Applying filter to " << filename << endl;
        cout << "  Filter: " << filterToString(filter) << endl;
        cout << "  Intensity: " << intensity << "%" << endl;
        cout << "  Preserve colors: " << (preserveColors ? "Yes" : "No") << endl;
        cout << endl;
    }

    void crop(int x = 0,
             int y = 0,
             int width = -1,
             int height = -1,
             bool fromCenter = false) {

        cout << "Cropping " << filename << endl;
        cout << "  Position: (" << x << ", " << y << ")" << endl;

        if (width > 0 && height > 0) {
            cout << "  Size: " << width << "x" << height << endl;
        } else {
            cout << "  Size: Full image" << endl;
        }

        cout << "  From center: " << (fromCenter ? "Yes" : "No") << endl;
        cout << endl;
    }

    void watermark(string text = "Copyright",
                  string position = "bottom-right",
                  int opacity = 30,
                  int fontSize = 12,
                  string color = "white") {

        cout << "Adding watermark to " << filename << endl;
        cout << "  Text: " << text << endl;
        cout << "  Position: " << position << endl;
        cout << "  Opacity: " << opacity << "%" << endl;
        cout << "  Font size: " << fontSize << "pt" << endl;
        cout << "  Color: " << color << endl;
        cout << endl;
    }

private:
    string formatToString(Format fmt) {
        switch (fmt) {
            case JPG: return "JPEG";
            case PNG: return "PNG";
            case BMP: return "BMP";
            case WEBP: return "WebP";
            default: return "Unknown";
        }
    }

    string filterToString(Filter flt) {
        switch (flt) {
            case NONE: return "None";
            case BLUR: return "Blur";
            case SHARPEN: return "Sharpen";
            case GRAYSCALE: return "Grayscale";
            case SEPIA: return "Sepia";
            default: return "Unknown";
        }
    }
};

int main() {
    ImageProcessor img("photo.jpg");

    // Resize with various options
    img.resize(800);
    img.resize(800, 600);
    img.resize(800, 600, false);

    // Convert with defaults
    img.convert();
    img.convert(ImageProcessor::PNG);
    img.convert(ImageProcessor::JPG, 95, true);

    // Apply filters
    img.applyFilter(ImageProcessor::BLUR);
    img.applyFilter(ImageProcessor::GRAYSCALE, 100);

    // Crop
    img.crop(100, 100, 400, 300);

    // Watermark
    img.watermark();
    img.watermark("My Photo", "center");
    img.watermark("© 2024", "bottom-right", 50, 16, "black");

    return 0;
}
```

### Solution 5: Game Character Creator

```cpp
#include <iostream>
#include <string>
#include <vector>
#include <map>
using namespace std;

class Character {
public:
    enum Class { WARRIOR, MAGE, ROGUE, ARCHER };
    enum Race { HUMAN, ELF, DWARF, ORC };

private:
    string name;
    Class charClass;
    Race charRace;
    int level;
    map<string, int> attributes;
    vector<string> equipment;
    vector<string> skills;

public:
    Character(string characterName,
             Class cClass = WARRIOR,
             Race cRace = HUMAN,
             int startLevel = 1)
        : name(characterName),
          charClass(cClass),
          charRace(cRace),
          level(startLevel) {

        // Default attributes
        attributes["strength"] = 10;
        attributes["agility"] = 10;
        attributes["intelligence"] = 10;
        attributes["vitality"] = 10;
    }

    void setAttributes(int strength = 10,
                      int agility = 10,
                      int intelligence = 10,
                      int vitality = 10) {
        attributes["strength"] = strength;
        attributes["agility"] = agility;
        attributes["intelligence"] = intelligence;
        attributes["vitality"] = vitality;
    }

    void equipItem(string item,
                  string slot = "inventory",
                  bool replaceExisting = true) {

        if (replaceExisting) {
            cout << "Equipping " << item << " in " << slot << endl;
        } else {
            cout << "Adding " << item << " to " << slot << endl;
        }

        equipment.push_back(item);
    }

    void learnSkill(string skill,
                   int requiredLevel = 1,
                   bool checkRequirements = true) {

        if (checkRequirements && level < requiredLevel) {
            cout << "Cannot learn " << skill
                 << " (requires level " << requiredLevel << ")" << endl;
            return;
        }

        skills.push_back(skill);
        cout << "Learned skill: " << skill << endl;
    }

    void displayStats(bool showEquipment = true,
                     bool showSkills = true,
                     bool detailedStats = false) {

        cout << "\n=== CHARACTER: " << name << " ===" << endl;
        cout << "Class: " << classToString() << endl;
        cout << "Race: " << raceToString() << endl;
        cout << "Level: " << level << endl;

        cout << "\nAttributes:" << endl;
        for (const auto& attr : attributes) {
            cout << "  " << attr.first << ": " << attr.second;

            if (detailedStats) {
                cout << " (" << calculateDerivedStat(attr.first) << " effective)";
            }
            cout << endl;
        }

        if (showEquipment && !equipment.empty()) {
            cout << "\nEquipment:" << endl;
            for (const auto& item : equipment) {
                cout << "  - " << item << endl;
            }
        }

        if (showSkills && !skills.empty()) {
            cout << "\nSkills:" << endl;
            for (const auto& skill : skills) {
                cout << "  - " << skill << endl;
            }
        }

        cout << "========================" << endl << endl;
    }

private:
    string classToString() {
        switch (charClass) {
            case WARRIOR: return "Warrior";
            case MAGE: return "Mage";
            case ROGUE: return "Rogue";
            case ARCHER: return "Archer";
            default: return "Unknown";
        }
    }

    string raceToString() {
        switch (charRace) {
            case HUMAN: return "Human";
            case ELF: return "Elf";
            case DWARF: return "Dwarf";
            case ORC: return "Orc";
            default: return "Unknown";
        }
    }

    int calculateDerivedStat(string attr) {
        // Simple calculation with race bonuses
        int base = attributes[attr];
        int bonus = 0;

        if (attr == "strength" && charRace == ORC) bonus = 5;
        if (attr == "agility" && charRace == ELF) bonus = 5;
        if (attr == "intelligence" && charRace == ELF) bonus = 3;
        if (attr == "vitality" && charRace == DWARF) bonus = 5;

        return base + bonus;
    }
};

int main() {
    // Create character with defaults
    Character hero1("Aragorn");
    hero1.displayStats();

    // Create character with custom class and race
    Character hero2("Legolas", Character::ARCHER, Character::ELF);
    hero2.setAttributes(12, 18, 14, 10);
    hero2.equipItem("Elven Bow", "weapon");
    hero2.equipItem("Leather Armor", "chest");
    hero2.learnSkill("Multi-shot");
    hero2.displayStats();

    // Create character with all custom options
    Character hero3("Gandalf", Character::MAGE, Character::HUMAN, 50);
    hero3.setAttributes(10, 10, 25, 15);
    hero3.equipItem("Staff of Power", "weapon");
    hero3.equipItem("Wizard Robes", "chest");
    hero3.learnSkill("Fireball", 10);
    hero3.learnSkill("Lightning Bolt", 20);
    hero3.learnSkill("Meteor", 40);
    hero3.displayStats(true, true, true);

    return 0;
}
```

## Summary

In this lesson, you learned:
- How to define and use default function arguments
- The rules for placing default arguments in function signatures
- How to combine default arguments with function overloading
- Best practices for using default arguments in classes
- Advanced techniques like using expressions and nullptr as defaults
- How to create flexible, user-friendly function interfaces

## Checklist

- [ ] I understand how default arguments work
- [ ] I know the rules for placing default arguments in function signatures
- [ ] I can combine default arguments with function overloading
- [ ] I can use default arguments in constructors and member functions
- [ ] I understand when to use default arguments vs function overloading
- [ ] I can create flexible APIs using default arguments
- [ ] I completed all exercises and understand their solutions
- [ ] I can debug issues related to default arguments

## Next Steps
In the next lesson, we'll explore Inline Functions, learning how to optimize function calls and understand when inline functions are beneficial.
