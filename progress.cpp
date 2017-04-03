#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>
#include <time.h>

using namespace std;

#define WINDOW_WIDTH 80

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
    friend class Event;
};

class Event {
private:
    string name;
    string description;
    Date startDate;
    Date endDate;
    int length;
    int past;
    double progress;
public:
    Event(
        string  init_name           = "",
        Date    init_startDate      = Date(),
        Date    init_endDate        = Date(),
        string  init_description    = "",
        int     init_length         = 0,
        int     init_past         = 0,
        double  init_progress       = 0.0
    );
    ~Event(void) {};
    int initiate(void);
    int update(int cmd, Event &target, string newInfo);
    int update(int cmd, Event &target, Date newDate);
    int getLength(void);
    double getProgress(void);
    string getDisplayString(void);
};

typedef struct node {
    Event data;
    struct node *next;
} EventNode;

class consoleClass {
private:
    string filename;
public:
    consoleClass(void);
    ~consoleClass(void){};
    string getFilenName(void);
    EventNode *getList(void);
    int displayAllEvet(EventNode *EventList);
    int save(void);
};

bool isLeap(int year);
Date getToday(void);
string to_string(int number, int width);
string to_string(double number, int precision);

/************************************************/
/*                     main                     */
/************************************************/

int main(int argc, char const *argv[]) {
    consoleClass console;
    string filename = console.getFilenName();
    EventNode *EventList = console.getList();
    console.displayAllEvet(EventList);
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
    string  init_name,
    Date    init_startDate,
    Date    init_endDate,
    string  init_description,
    int     init_length,
    int     init_past,
    double  init_progress
) {
    name        = init_name;
    description = init_description;
    startDate   = init_startDate;
    endDate     = init_endDate;
    length      = init_length;
    past      = init_past,
    progress    = init_progress;
    this->initiate();
}

int Event::initiate(void) {
    int flag = 0;
    Date today = getToday();
    length = endDate - startDate;
    past = today - startDate;
    progress = double(past) / length;
    return flag;
}

// Commands:
// 1    Update the name
// 2    Update the description
// 3    Update the startDate
// 4    Update the endDate
// Flags:
// 0    Normal
// 1    Invalid command, not 1, 2, 3 or 4 either.
// 11   Entered a string but command 3 or 4.
// 21   Entered a Date but command 1 or 2.
int Event::update(int cmd, Event &target, string newInfo) {
    int flag = 0;
    switch (cmd) {
        case 1:
            name = newInfo;
            break;
        case 2:
            description = newInfo;
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
    if (!flag) {
        this->initiate();
    }
    return flag;
}

int Event::update(int cmd, Event &target, Date newDate) {
    int flag = 0;
    switch (cmd) {
        case 1:
        case 2:
            cout << "Do you mean command 3 or 4?" << endl;
            flag = 21;
            break;
        case 3:
            startDate = newDate;
            break;
        case 4:
            endDate = newDate;
            break;
        default:
            cout << "Invalid command!" << endl;
            flag = 1;
            break;
    }
    if (!flag) {
        this->initiate();
    }
    return flag;
}

int Event::getLength(void) {
    return length;
}

double Event::getProgress(void) {
    return progress;
}

string Event::getDisplayString(void) {
    string result = name;

    // Add description
    if (description != "") {
        result += '[' + description + ']';
    }
    result += '\n';

    // Add startDate and endDate, using resultStream and resultTemp
    result += to_string(startDate.year) + '-';
    result += to_string(startDate.month, 2) + '-';
    result += to_string(startDate.day, 2);
    result += " -> ";
    result += to_string(endDate.year) + '-';
    result += to_string(endDate.month, 2) + '-';
    result += to_string(endDate.day, 2);
    result += '\n';

    // Progress
    result += to_string(past) + '/' + to_string(length) + "days, ";
    result += to_string(progress * 100, 2) + "% passed.";
    result += '\n';

    int pastBlockNum = WINDOW_WIDTH * progress;
    // Round off
    pastBlockNum += ((progress * 100.0) - int(pastBlockNum / WINDOW_WIDTH * 100)) >= 0.5 ? 1 : 0;
    for (int i = 0; i < pastBlockNum; i++) {
        result += "▓";
    }
    for (int i = pastBlockNum; i < WINDOW_WIDTH; i++) {
        result += "░";
    }
    return result;
}

/************************************************/
/*                 consoleClass                 */
/************************************************/
consoleClass::consoleClass(void) {
    filename = "";
}

string consoleClass::getFilenName(void) {
	fstream detectFile;
	char errCmd;
	do {
		cout << "Enter the Database filename\n(\"data\" for default): ";
		getline(cin, filename);
		filename = (filename == "") ? "data" : filename;
		detectFile.open(filename, ios::in | ios::out);
		if (!detectFile) {
			cerr << "Could NOT open the file!" << endl;
			do {
				cout << "Enter again?(Y/N)";
				errCmd = cin.get();
				while (cin.get() != '\n');
				if (errCmd == 'N' || errCmd == 'n') {
					exit(1);
				}
			} while(errCmd != 'Y' && errCmd != 'y');
		}
	} while(!detectFile);
	cout << "Open file successfully!" << endl;
	return filename;
}

EventNode *consoleClass::getList(void) {
    EventNode *EventListHead = new EventNode;
    EventNode *lastNode, *newNode;
    EventListHead->next = NULL;
    lastNode = EventListHead;

    // One line is like:
    // #[Term][This term][2017 2 27][2017 7 7]
    string  name, description, temp;
    int     startDateYear, startDateMonth, startDateDay;
    int     endDateYear, endDateMonth, endDateDay;
    ifstream file;
    file.open(filename, ios::in);
    while (!file.eof()) {
        file.ignore(2, '[');
        getline(file, name, ']');
        file.ignore(1, '[');
        getline(file, description, ']');
        file.ignore(1, '[');
        file >> startDateYear >> startDateMonth >> startDateDay;
        file.ignore(2, '[');
        file >> endDateYear >> endDateMonth >> endDateDay;
        file.ignore(2, '\n');
        if(file.eof()) break;
        newNode = new EventNode;
        lastNode->next = newNode;
        newNode->data = Event(
            name,
            Date(startDateYear, startDateMonth, startDateDay),
            Date(endDateYear, endDateMonth, endDateDay),
            description
        );
        newNode->next = NULL;
        lastNode = newNode;
    }

    return EventListHead;
}

int consoleClass::displayAllEvet(EventNode *EventList) {
    int flag = 0;
    int counter = 0;
    EventNode *scanner = EventList;
    while (scanner->next != NULL) {
        scanner = scanner->next;
        counter++;
        cout << setfill('0') << setw(2) << counter << ". " << scanner->data.getDisplayString() << endl;
	}
    return flag;
}

int consoleClass::save(void) {
    int flag = 0;
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

Date getToday(void) {
    time_t timep;
    struct tm *pTIME;
    time(&timep);
    pTIME = localtime(&timep);
    int year = 1900 + pTIME->tm_year;
    int month = 1 + pTIME->tm_mon;
    int day = pTIME->tm_mday;
    return Date(year, month, day);
}

string to_string(int number, int width) {
    stringstream tempStringStream;
    string result;

    tempStringStream << setfill('0') << setw(width) << number;
    tempStringStream >> result;
    return result;
}

string to_string(double number, int precision) {
    stringstream tempStringStream;
    string result;
    // The "fixed" converts the float point number to fixed point number
    // setprecision() sets the precision of:
    // fixed and scientific -> after decimal point
    // other -> the whole number
    tempStringStream << fixed << setprecision(precision) << number;
    tempStringStream >> result;
    return result;
}
