#include <string>
#include <iostream>
using namespace std;

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

int main(int argc, char const *argv[]) {

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
// 11   entered a string but command 3 or 4.
// 21   entered a Date but command 1 or 2.
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
