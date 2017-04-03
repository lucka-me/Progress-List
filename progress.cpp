#include <string>
#include <iostream>
using namespace std;

/************************************************/
/*                     Head                     */
/************************************************/

class Date {
private:
    int year;
    int month;
    int day;
public:
    Date(
        int init_year = 1980,
        int init_month = 1,
        int init_day = 1);
    ~Date(void) {};
    bool isLeap(void);
    friend int operator - (Date &endDate, Date &startDate);
};

class Event {
private:
    int number;
    string name;
    string description;
    Date startDate;
    Date endDate;
    int length;
    double progress;
public:
    Event(
        string init_name,
        Date init_startDate,
        Date init_endDate,
        string init_description = "",
        int init_num = 0,
        int init_length = 0,
        double init_progress = 0.0
    );
    ~Event(void) {};
    int edit(int cmd, Event &target, string str);
    int edit(int cmd, Event &target, Date d);
    int getLength(void);
    double getProgress(void);
    string getLine(void);
};

typedef struct node {
    Event date;
    node *next;
    node *previous;
} NewNode;

bool isLeap(int year);


/************************************************/
/*                     main                     */
/************************************************/

int main(int argc, char const *argv[]) {
    Date start;
    Date end;
    int tempYear, tempMonth, tempDay;
    cout << "Enter the began: ";
    cin >> tempYear >> tempMonth >> tempDay;
    start = Date(tempYear, tempMonth, tempDay);
    cout << "isLeap:" << start.isLeap() << endl;
    cout << "Enter the end: ";
    cin >> tempYear >> tempMonth >> tempDay;
    end = Date(tempYear, tempMonth, tempDay);
    cout << end - start << "days" << endl;
    return 0;
}

/************************************************/
/*                     Date                     */
/************************************************/
Date::Date(int init_year, int init_month, int init_day) {
    year = init_year;
    month = init_month;
    day = init_day;
}

bool Date::isLeap(void) {
    return year % 400 == 0 || (year % 4 == 0 && year % 100 != 0);
}
/************************************************/
/*                     Event                    */
/************************************************/
Event::Event(
    string init_name,
    Date init_startDate,
    Date init_endDate,
    string init_description,
    int init_num,
    int init_length,
    double init_progress
) {
    number = init_num;
    name = init_name;
    description = init_description;
    startDate = init_startDate;
    endDate = init_endDate;
    length = init_length;
    progress = init_progress;
}

// flags:
// 0    Normal
// 1    Invalid command, not 1, 2, 3 or 4 either.
// 11   Entered a string but command 3 or 4.
// 21   Entered a Date but command 1 or 2.
int Event::edit(int cmd, Event &target, string str) {
    int flag = 0;
    switch (cmd) {
        case 1:
            name = str;
            break;
        case 2:
            description = str;
            break;
        case 3:
        case 4:
            cout << "Do you mean command 1 or 2?" << endl;
            flag = 11;
            break;
        default:
            cout << "Invalid command!" << endl;
            flag = 1;
            break;
    }
    return flag;
}

int Event::edit(int cmd, Event &target, Date d) {
    int flag = 0;
    switch (cmd) {
        case 1:
        case 2:
            cout << "Do you mean command 3 or 4?" << endl;
            flag = 21;
            break;
        case 3:
            startDate = d;
            break;
        case 4:
            endDate = d;
            break;
        default:
            cout << "Invalid command!" << endl;
            flag = 1;
            break;
    }
    return flag;
}

/************************************************/
/*                   Operator                   */
/************************************************/
int operator - (Date &endDate, Date &startDate) {
    // Invalid operate
    if (
        (endDate.year < startDate.year
        ) || (
            (endDate.year == startDate.year) &&
            (endDate.month < startDate.month)
        ) || (
            (endDate.year == startDate.year) &&
            (endDate.month == startDate.month) &&
            (endDate.day < startDate.day)
        )
    ) {
        return -1;
    }

    int result = 0;
    int CommonYearList[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int LeapYearList[] = {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    // In the same year
    if (endDate.year == startDate.year) {
        cout << "In the same year." << endl;
        // In the same month
        if (endDate.month == startDate.month) {
            result = endDate.day - startDate.day;
        } else {
            // Common year or leap year
            if (endDate.isLeap()) {
                result += LeapYearList[startDate.month] - startDate.day;
                for (int scanner = startDate.month + 1; scanner < endDate.month; scanner++) {
                    result += LeapYearList[scanner];
                }
                result += endDate.day;
            } else {
                result += CommonYearList[startDate.month] - startDate.day;
                for (int scanner = startDate.month + 1; scanner < endDate.month; scanner++) {
                    result += CommonYearList[scanner];
                }
                result += endDate.day;
            }
        }
    } else {
        // Start in leap year
        if (startDate.isLeap()) {
            result += LeapYearList[startDate.month] - startDate.day;
            for (int scanner = startDate.month + 1; scanner <= 12; scanner++) {
                result += LeapYearList[scanner];
            }
        } else {
            result += CommonYearList[startDate.month] - startDate.day;
            for (int scanner = startDate.month + 1; scanner <= 12; scanner++) {
                result += CommonYearList[scanner];
            }
        }
        // End in leap year
        if (endDate.isLeap()) {
            for (int scanner = 1; scanner < endDate.month; scanner++) {
                result += LeapYearList[scanner];
            }
            result += startDate.day;
        } else {
            for (int scanner = 1; scanner < endDate.month; scanner++) {
                result += CommonYearList[scanner];
            }
            result += startDate.day;
        }
        // Years between start and end
        for (int scanner = startDate.year + 1; scanner < endDate.year; scanner++) {
            result += isLeap(scanner) ? 366 : 365;
        }
    }

    return result;
}

/************************************************/
/*                   Functions                  */
/************************************************/
bool isLeap(int year) {
    return year % 400 == 0 || (year % 4 == 0 && year % 100 != 0);
}
