# Lesson 98: Security Best Practices

**Duration**: 6 hours
**Difficulty**: Advanced
**Prerequisites**: Lessons 1-97

## Table of Contents
1. Buffer Overflow Prevention
2. Input Validation
3. Cryptography
4. Secure Coding
5. Common Vulnerabilities

---

## 1. Buffer Overflow Prevention

```cpp
// ❌ UNSAFE
void unsafeFunction(const char* input) {
    char buffer[100];
    strcpy(buffer, input); // DANGEROUS!
}

// ✅ SAFE
void safeFunction(const char* input) {
    char buffer[100];
    strncpy(buffer, input, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';
}

// ✅ BETTER: Use std::string
void betterFunction(const std::string& input) {
    std::string buffer = input; // Safe
}
```

---

## 2. Input Validation

```cpp
class InputValidator {
public:
    static bool validateEmail(const std::string& email) {
        // Basic email validation
        if (email.empty() || email.length() > 254) {
            return false;
        }

        size_t atPos = email.find('@');
        if (atPos == std::string::npos) {
            return false;
        }

        std::string local = email.substr(0, atPos);
        std::string domain = email.substr(atPos + 1);

        return !local.empty() && !domain.empty() &&
               domain.find('.') != std::string::npos;
    }

    static bool validateInteger(const std::string& str, int& result) {
        try {
            size_t pos;
            result = std::stoi(str, &pos);
            return pos == str.length();
        } catch (...) {
            return false;
        }
    }
};
```

---

## 3. Password Hashing

```cpp
#include <openssl/sha.h>
#include <iomanip>
#include <sstream>

class PasswordManager {
public:
    static std::string hashPassword(const std::string& password,
                                    const std::string& salt) {
        std::string combined = password + salt;
        unsigned char hash[SHA256_DIGEST_LENGTH];

        SHA256(reinterpret_cast<const unsigned char*>(combined.c_str()),
               combined.length(), hash);

        std::ostringstream oss;
        for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
            oss << std::hex << std::setw(2) << std::setfill('0')
                << static_cast<int>(hash[i]);
        }

        return oss.str();
    }
};
```

---

## 4. SQL Injection Prevention

```cpp
// ❌ UNSAFE
std::string unsafeQuery(const std::string& username) {
    return "SELECT * FROM users WHERE name = '" + username + "'";
    // Can be exploited: username = "' OR '1'='1"
}

// ✅ SAFE: Use prepared statements
void safeQuery(Database& db, const std::string& username) {
    auto stmt = db.prepare("SELECT * FROM users WHERE name = ?");
    stmt.bind(1, username);
    stmt.execute();
}
```

---

## Summary

✅ **Buffer Safety**: Bounds checking
✅ **Input Validation**: Never trust input
✅ **Cryptography**: Proper hashing
✅ **SQL Injection**: Prepared statements

**Next**: Code Maintenance!
