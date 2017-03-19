#include <string>
#include <iostream>
using namespace std;

class Date {
private:
    int year;
    int month;
    int day;
public:
    Date(int init_year, int init_month, int init_day);
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
        int Length = 0,
        double init_progress = 0.0
    );
    ~Event(void) {};
    void edit(int cmd, Event &target, string str);
    void edit(int cmd, Event &target, Date d);
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
