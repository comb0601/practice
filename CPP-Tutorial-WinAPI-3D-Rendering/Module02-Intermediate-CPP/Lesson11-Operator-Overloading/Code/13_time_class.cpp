/*
 * Lesson 11: Operator Overloading
 * File: 13_time_class.cpp
 *
 * Time class representing hours, minutes, and seconds.
 * Demonstrates operator overloading for time arithmetic.
 */

#include <iostream>
#include <iomanip>
#include <sstream>
#include <stdexcept>

class Time {
private:
    int hours;
    int minutes;
    int seconds;

    // Normalize time (handle overflow/underflow)
    void normalize() {
        // Handle negative seconds
        while (seconds < 0) {
            seconds += 60;
            minutes--;
        }
        // Handle overflow seconds
        while (seconds >= 60) {
            seconds -= 60;
            minutes++;
        }

        // Handle negative minutes
        while (minutes < 0) {
            minutes += 60;
            hours--;
        }
        // Handle overflow minutes
        while (minutes >= 60) {
            minutes -= 60;
            hours++;
        }

        // Handle negative hours (wrap to 24-hour format)
        while (hours < 0) {
            hours += 24;
        }
        // Keep hours in 24-hour format
        hours %= 24;
    }

public:
    // Constructor
    Time(int h = 0, int m = 0, int s = 0) : hours(h), minutes(m), seconds(s) {
        normalize();
    }

    // Constructor from total seconds
    explicit Time(int totalSeconds) {
        hours = totalSeconds / 3600;
        minutes = (totalSeconds % 3600) / 60;
        seconds = totalSeconds % 60;
        normalize();
    }

    // Getters
    int getHours() const { return hours; }
    int getMinutes() const { return minutes; }
    int getSeconds() const { return seconds; }

    // Convert to total seconds
    int toSeconds() const {
        return hours * 3600 + minutes * 60 + seconds;
    }

    // Addition
    Time operator+(const Time& other) const {
        return Time(hours + other.hours, minutes + other.minutes, seconds + other.seconds);
    }

    // Subtraction
    Time operator-(const Time& other) const {
        return Time(hours - other.hours, minutes - other.minutes, seconds - other.seconds);
    }

    // Multiply by scalar (repeat time)
    Time operator*(int factor) const {
        int totalSecs = toSeconds() * factor;
        return Time(totalSecs);
    }

    // Divide by scalar
    Time operator/(int divisor) const {
        if (divisor == 0) {
            throw std::runtime_error("Division by zero");
        }
        int totalSecs = toSeconds() / divisor;
        return Time(totalSecs);
    }

    // Unary minus
    Time operator-() const {
        return Time(-hours, -minutes, -seconds);
    }

    // Prefix increment (add one second)
    Time& operator++() {
        seconds++;
        normalize();
        return *this;
    }

    // Postfix increment
    Time operator++(int) {
        Time temp(*this);
        ++(*this);
        return temp;
    }

    // Prefix decrement (subtract one second)
    Time& operator--() {
        seconds--;
        normalize();
        return *this;
    }

    // Postfix decrement
    Time operator--(int) {
        Time temp(*this);
        --(*this);
        return temp;
    }

    // Compound assignment
    Time& operator+=(const Time& other) {
        hours += other.hours;
        minutes += other.minutes;
        seconds += other.seconds;
        normalize();
        return *this;
    }

    Time& operator-=(const Time& other) {
        hours -= other.hours;
        minutes -= other.minutes;
        seconds -= other.seconds;
        normalize();
        return *this;
    }

    Time& operator*=(int factor) {
        int totalSecs = toSeconds() * factor;
        *this = Time(totalSecs);
        return *this;
    }

    Time& operator/=(int divisor) {
        if (divisor == 0) {
            throw std::runtime_error("Division by zero");
        }
        int totalSecs = toSeconds() / divisor;
        *this = Time(totalSecs);
        return *this;
    }

    // Comparison operators
    bool operator==(const Time& other) const {
        return hours == other.hours && minutes == other.minutes && seconds == other.seconds;
    }

    bool operator!=(const Time& other) const {
        return !(*this == other);
    }

    bool operator<(const Time& other) const {
        return toSeconds() < other.toSeconds();
    }

    bool operator>(const Time& other) const {
        return other < *this;
    }

    bool operator<=(const Time& other) const {
        return !(other < *this);
    }

    bool operator>=(const Time& other) const {
        return !(*this < other);
    }

    // Format options
    std::string to12HourFormat() const {
        std::ostringstream oss;
        int h = hours % 12;
        if (h == 0) h = 12;
        oss << h << ":"
            << std::setfill('0') << std::setw(2) << minutes << ":"
            << std::setw(2) << seconds;
        oss << (hours < 12 ? " AM" : " PM");
        return oss.str();
    }

    std::string to24HourFormat() const {
        std::ostringstream oss;
        oss << std::setfill('0')
            << std::setw(2) << hours << ":"
            << std::setw(2) << minutes << ":"
            << std::setw(2) << seconds;
        return oss.str();
    }

    // Stream output (24-hour format by default)
    friend std::ostream& operator<<(std::ostream& os, const Time& t) {
        os << std::setfill('0')
           << std::setw(2) << t.hours << ":"
           << std::setw(2) << t.minutes << ":"
           << std::setw(2) << t.seconds;
        return os;
    }

    // Stream input
    friend std::istream& operator>>(std::istream& is, Time& t) {
        char colon;
        is >> t.hours >> colon >> t.minutes >> colon >> t.seconds;
        t.normalize();
        return is;
    }
};

// Non-member multiplication
Time operator*(int factor, const Time& t) {
    return t * factor;
}

int main() {
    std::cout << "=== Time Class Demo ===\n\n";

    // Create time objects
    std::cout << "--- Creating Time Objects ---\n";
    Time t1(10, 30, 45);
    Time t2(2, 45, 30);
    Time t3(23, 50, 0);

    std::cout << "t1 = " << t1 << " (" << t1.to12HourFormat() << ")\n";
    std::cout << "t2 = " << t2 << " (" << t2.to12HourFormat() << ")\n";
    std::cout << "t3 = " << t3 << " (" << t3.to12HourFormat() << ")\n\n";

    // Arithmetic operations
    std::cout << "--- Arithmetic Operations ---\n";
    Time sum = t1 + t2;
    std::cout << "t1 + t2 = " << sum << "\n";

    Time diff = t1 - t2;
    std::cout << "t1 - t2 = " << diff << "\n";

    Time doubled = t2 * 2;
    std::cout << "t2 * 2 = " << doubled << "\n";

    Time halved = t1 / 2;
    std::cout << "t1 / 2 = " << halved << "\n\n";

    // Time overflow handling
    std::cout << "--- Overflow Handling ---\n";
    Time late = Time(23, 30, 0);
    Time duration = Time(1, 45, 0);
    std::cout << "late = " << late << "\n";
    std::cout << "duration = " << duration << "\n";
    Time next = late + duration;
    std::cout << "late + duration = " << next << " (wraps to next day)\n\n";

    // Increment/Decrement
    std::cout << "--- Increment/Decrement ---\n";
    Time t4(12, 0, 58);
    std::cout << "t4 = " << t4 << "\n";
    ++t4;
    std::cout << "++t4 = " << t4 << "\n";
    ++t4;
    std::cout << "++t4 = " << t4 << " (minute increments)\n";
    --t4;
    std::cout << "--t4 = " << t4 << "\n\n";

    // Compound assignment
    std::cout << "--- Compound Assignment ---\n";
    Time t5(10, 0, 0);
    std::cout << "t5 = " << t5 << "\n";
    t5 += Time(0, 30, 0);
    std::cout << "After t5 += 00:30:00: " << t5 << "\n";
    t5 -= Time(0, 15, 30);
    std::cout << "After t5 -= 00:15:30: " << t5 << "\n";
    t5 *= 2;
    std::cout << "After t5 *= 2: " << t5 << "\n\n";

    // Comparison
    std::cout << "--- Comparison ---\n";
    Time morning(9, 0, 0);
    Time afternoon(14, 30, 0);
    Time evening(9, 0, 0);

    std::cout << "morning = " << morning << "\n";
    std::cout << "afternoon = " << afternoon << "\n";
    std::cout << "evening = " << evening << "\n\n";

    std::cout << "morning == evening: " << (morning == evening ? "true" : "false") << "\n";
    std::cout << "morning < afternoon: " << (morning < afternoon ? "true" : "false") << "\n";
    std::cout << "afternoon > morning: " << (afternoon > morning ? "true" : "false") << "\n\n";

    // Sorting times
    std::cout << "--- Sorting Times ---\n";
    std::vector<Time> times = {
        Time(14, 30, 0),
        Time(9, 15, 30),
        Time(23, 45, 0),
        Time(6, 0, 0),
        Time(12, 0, 0)
    };

    std::cout << "Unsorted: ";
    for (const auto& t : times) {
        std::cout << t << " ";
    }
    std::cout << "\n";

    std::sort(times.begin(), times.end());

    std::cout << "Sorted:   ";
    for (const auto& t : times) {
        std::cout << t << " ";
    }
    std::cout << "\n\n";

    // Real-world example: Work schedule
    std::cout << "--- Work Schedule Example ---\n";
    Time workStart(9, 0, 0);
    Time lunchBreak(1, 0, 0);
    Time workEnd(17, 30, 0);

    Time actualWork = workEnd - workStart - lunchBreak;
    std::cout << "Work start: " << workStart << "\n";
    std::cout << "Work end: " << workEnd << "\n";
    std::cout << "Lunch break: " << lunchBreak << "\n";
    std::cout << "Actual work time: " << actualWork << "\n";
    std::cout << "Work hours: " << actualWork.getHours() << " hours, "
              << actualWork.getMinutes() << " minutes\n\n";

    // Meeting duration
    std::cout << "--- Meeting Duration ---\n";
    Time meetingStart(14, 15, 0);
    Time meetingEnd(15, 45, 0);
    Time meetingDuration = meetingEnd - meetingStart;
    std::cout << "Meeting: " << meetingStart << " to " << meetingEnd << "\n";
    std::cout << "Duration: " << meetingDuration << "\n";
    std::cout << "In minutes: " << (meetingDuration.toSeconds() / 60) << " minutes\n\n";

    // Travel time calculation
    std::cout << "--- Travel Time Example ---\n";
    Time departure(8, 30, 0);
    Time arrival(11, 15, 0);
    Time travelTime = arrival - departure;
    std::cout << "Departure: " << departure << "\n";
    std::cout << "Arrival: " << arrival << "\n";
    std::cout << "Travel time: " << travelTime << "\n\n";

    // Time zone difference
    std::cout << "--- Time Zone Conversion ---\n";
    Time nyTime(14, 0, 0);
    Time timeZoneDiff(3, 0, 0);  // LA is 3 hours behind
    Time laTime = nyTime - timeZoneDiff;
    std::cout << "New York: " << nyTime << " (" << nyTime.to12HourFormat() << ")\n";
    std::cout << "Los Angeles: " << laTime << " (" << laTime.to12HourFormat() << ")\n\n";

    // Sports timer
    std::cout << "--- Sports Timer ---\n";
    Time quarterLength(0, 15, 0);  // 15 minutes per quarter
    Time fullGame = quarterLength * 4;
    std::cout << "Quarter length: " << quarterLength << "\n";
    std::cout << "Full game (4 quarters): " << fullGame << "\n";

    Time overtime(0, 5, 0);
    Time gameWithOT = fullGame + overtime;
    std::cout << "Game with overtime: " << gameWithOT << "\n\n";

    // Cooking timer
    std::cout << "--- Cooking Timer ---\n";
    Time prepTime(0, 20, 0);
    Time cookTime(1, 30, 0);
    Time totalTime = prepTime + cookTime;
    std::cout << "Prep time: " << prepTime << "\n";
    std::cout << "Cook time: " << cookTime << "\n";
    std::cout << "Total time: " << totalTime << "\n";

    Time startCooking(17, 30, 0);
    Time finishTime = startCooking + totalTime;
    std::cout << "Start cooking at: " << startCooking << "\n";
    std::cout << "Finish at: " << finishTime << "\n\n";

    // Countdown example
    std::cout << "--- Countdown Example ---\n";
    Time countdown(0, 0, 10);  // 10 seconds
    std::cout << "Countdown: ";
    for (int i = 0; i < 10; i++) {
        std::cout << countdown << " ";
        countdown--;
    }
    std::cout << countdown << " DONE!\n";

    return 0;
}

/*
 * OUTPUT (partial):
 * =================================
 * === Time Class Demo ===
 *
 * --- Creating Time Objects ---
 * t1 = 10:30:45 (10:30:45 AM)
 * t2 = 02:45:30 (2:45:30 AM)
 * t3 = 23:50:00 (11:50:00 PM)
 *
 * --- Arithmetic Operations ---
 * t1 + t2 = 13:16:15
 * t1 - t2 = 07:45:15
 * t2 * 2 = 05:31:00
 * t1 / 2 = 05:15:22
 *
 * --- Overflow Handling ---
 * late = 23:30:00
 * duration = 01:45:00
 * late + duration = 01:15:00 (wraps to next day)
 *
 * --- Increment/Decrement ---
 * t4 = 12:00:58
 * ++t4 = 12:00:59
 * ++t4 = 12:01:00 (minute increments)
 * --t4 = 12:00:59
 *
 * ... (more output follows)
 */
