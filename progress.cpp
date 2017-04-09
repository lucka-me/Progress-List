#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>
#include <time.h>
#include <sys/ioctl.h>

using namespace std;

int WINDOW_WIDTH;

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
    string title;
    string description;
    Date startDate;
    Date endDate;
    int length;
    int past;
    double progress;
public:
    Event(
        string  init_title           = "",
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
    string getFileString(void);
};

// Note:
// This linked list is NOT in OOP style,
// because the Data Struct lesson I'm learning is based on C but not C++.
// I'd change it into a class one day.
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
    int commandList[2];
    int getCommand(bool isMenuDisplay = true);
    string getFilenName(void);
    EventNode *getList(void);
    int displayAllEvet(EventNode *EventListHead);
    Event getNewEvent(void);
    int addEvent(EventNode *EventListHead, Event newEvent, int position = 0);
    Event deleteEvent(EventNode *EventListHead, int position = 0);
    int save(EventNode *EventListHead);
};

bool isLeap(int year);
bool isLegalDate(int year, int month, int day);
Date getToday(void);
string to_string(int number, int width);
string to_string(double number, int precision);

/************************************************/
/*                     main                     */
/************************************************/

int main(int argc, char const *argv[]) {
    // Get the terminal window size
    struct winsize windowSize;
    ioctl(0, TIOCGWINSZ, &windowSize);
    WINDOW_WIDTH = windowSize.ws_col;

    consoleClass console;
    int command;
    bool exitFlag = false;
    bool isMenuDisplay = true;
    string filename = console.getFilenName();
    EventNode *EventList = console.getList();
    Event tempEvent;

    do {
        command = console.getCommand(isMenuDisplay);
        isMenuDisplay = true;
        switch (command) {
            case -1:
                isMenuDisplay = false;
                break;
            case 1:
                console.displayAllEvet(EventList);
                break;
            case 2:
                tempEvent = console.getNewEvent();
                console.addEvent(EventList, tempEvent, console.commandList[1]);
                break;
            case 3:
                console.deleteEvent(EventList, console.commandList[1]);
                isMenuDisplay = false;
                break;
            case 4:
                break;
            case 5:
                console.save(EventList);
                isMenuDisplay = false;
                break;
            case 6:
                exitFlag = true;
                break;
            default:
                break;
        }
    } while(!exitFlag);

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
    string  init_title,
    Date    init_startDate,
    Date    init_endDate,
    string  init_description,
    int     init_length,
    int     init_past,
    double  init_progress
) {
    title        = init_title;
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
// 1    Update the title
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
            title = newInfo;
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
    string result = title;

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

string Event::getFileString(void) {
    string result = "#[" + title + "][" + description + "][";

    // Add startDate and endDate, using resultStream and resultTemp
    result += to_string(startDate.year) + ' ';
    result += to_string(startDate.month, 2) + ' ';
    result += to_string(startDate.day, 2);
    result += "][";
    result += to_string(endDate.year) + ' ';
    result += to_string(endDate.month, 2) + ' ';
    result += to_string(endDate.day, 2);
    result += "]";

    return result;
}

/************************************************/
/*                 consoleClass                 */
/************************************************/
consoleClass::consoleClass(void) {
    filename = "";
}

int consoleClass::getCommand(bool isMenuDisplay) {
    if (isMenuDisplay) {
        cout << "/*" << setfill('*') << setw(WINDOW_WIDTH - 4) << '*' << "*/" << endl
             << "/*" << setfill(' ') << setw((WINDOW_WIDTH - 8) / 2) << ' ' << "MENU" << setfill(' ') << setw((WINDOW_WIDTH - 8) / 2) << ' ' << "*/" << endl
             << "/*" << setfill('*') << setw(WINDOW_WIDTH - 4) << '*' << "*/" << endl
             << "/* 1. Display all events"  << setfill(' ') << setw(WINDOW_WIDTH - 26) << ' ' << "*/" << endl
             << "/* 2. Add a new event"     << setfill(' ') << setw(WINDOW_WIDTH - 23) << ' ' << "*/" << endl
             << "/* 3. Delete an event"     << setfill(' ') << setw(WINDOW_WIDTH - 23) << ' ' << "*/" << endl
             << "/* 4. Edit an event"       << setfill(' ') << setw(WINDOW_WIDTH - 21) << ' ' << "*/" << endl
             << "/* 5. Save to file"        << setfill(' ') << setw(WINDOW_WIDTH - 20) << ' ' << "*/" << endl
             << "/* 6. Exit"                << setfill(' ') << setw(WINDOW_WIDTH - 12) << ' ' << "*/" << endl
             << "/*" << setfill('*') << setw(WINDOW_WIDTH - 4) << '*' << "*/" << endl;
    }
    cout << "Enter the command: ";
    cin >> commandList[0];
    switch (commandList[0]) {
        case 1:
        case 5:
        case 6:
            break;
        case 2:
        case 3:
        case 4:
            cout << "Enter the position (0 for the last): ";
            cin >> commandList[1];
            break;
        default:
            cout << "Invalid command!";
            commandList[0] = -1;
            break;
    }

    return commandList[0];
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
    // LINKEDLIST USED
    EventNode *EventListHead = new EventNode;
    EventNode *lastNode, *newNode;
    EventListHead->next = NULL;
    lastNode = EventListHead;

    // One line is like:
    // #[Term][This term][2017 2 27][2017 7 7]
    string  title, description;
    int     startDateYear, startDateMonth, startDateDay;
    int     endDateYear, endDateMonth, endDateDay;
    ifstream file;
    file.open(filename, ios::in);
    while (!file.eof()) {
        file.ignore(2, '[');
        getline(file, title, ']');
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
            title,
            Date(startDateYear, startDateMonth, startDateDay),
            Date(endDateYear, endDateMonth, endDateDay),
            description
        );
        newNode->next = NULL;
        lastNode = newNode;
    }

    return EventListHead;
}

int consoleClass::displayAllEvet(EventNode *EventListHead) {
    int flag = 0;
    int counter = 0;
    // LINKEDLIST USED
    EventNode *scanner = EventListHead;
    while (scanner->next != NULL) {
        scanner = scanner->next;
        counter++;
        cout << setfill('0') << setw(2) << counter << ". " << scanner->data.getDisplayString() << "\n\n";
	}
    return flag;
}

Event consoleClass::getNewEvent(void) {
    string  title = "", description = "";
    int     startDateYear, startDateMonth, startDateDay;
    int     endDateYear, endDateMonth, endDateDay;
    // Clean the istream
    while (cin.get() != '\n');
    cout << "Enter the title: ";
    getline(cin, title, '\n');
    if (title == "") {
        cout << "A title is recommend. If you\'re sure what you want, please enter nothing again." << endl;
        cout << "Enter the title: ";
        getline(cin, title, '\n');
        if (title == "") title = "EventName";
    }

    cout << "Enter the description (optional):" << endl;
    getline(cin, description, '\n');

    cout << "Enter the begining date (format: yyyy mm dd): ";
    cin >> startDateYear >> startDateMonth >> startDateDay;
    while (!isLegalDate(startDateYear, startDateMonth, startDateDay)) {
        cout << "Invalid input!" << endl;
        cout << "Enter the begining date (format: yyyy mm dd): ";
        cin >> startDateYear >> startDateMonth >> startDateDay;
    }
    cout << "Enter the ending date (format: yyyy mm dd): ";
    cin >> endDateYear >> endDateMonth >> endDateDay;
    while (!isLegalDate(endDateYear, endDateMonth, endDateDay)) {
        cout << "Invalid input!" << endl;
        cout << "Enter the ending date (format: yyyy mm dd): ";
        cin >> endDateYear >> endDateMonth >> endDateDay;
    }
    return Event(
        title,
        Date(startDateYear, startDateMonth, startDateDay),
        Date(endDateYear, endDateMonth, endDateDay),
        description
    );
}

int consoleClass::addEvent(EventNode *EventListHead, Event newEvent, int position) {
    int flag = 0;
    // LINKEDLIST USED
    EventNode *scanner = EventListHead;
    EventNode *newEventNode = new EventNode;

    // position == 0 -> add to the last
    if (position == 0) {
        while (scanner->next != NULL) {
            scanner = scanner->next;
        }
    } else {
        int counter = 0;
        while (counter < position - 1 && scanner->next != NULL) {
            counter++;
            scanner = scanner->next;
        }
    }
    newEventNode->data = newEvent;
    newEventNode->next = scanner->next;
    scanner->next = newEventNode;
    cout << "Event added." << endl;
    return flag;
}

Event consoleClass::deleteEvent(EventNode *EventListHead, int position) {
    Event result;
    // LINKEDLIST USED
    EventNode *scanner = EventListHead;
    EventNode *lastNode = EventListHead;

    // position == 0 -> add to the last
    if (position == 0) {
        while (scanner->next != NULL) {
            lastNode = scanner;
            scanner = scanner->next;
        }
    } else {
        int counter = 0;
        while (counter < position && scanner->next != NULL) {
            counter++;
            lastNode = scanner;
            scanner = scanner->next;
        }
    }
    result = scanner->data;
    lastNode->next = scanner->next;
    delete(scanner);
    return result;
}

int consoleClass::save(EventNode *EventListHead) {
    int flag = 0;
    EventNode *scanner = EventListHead;

    ofstream file;
    file.open(filename, ios::out);
    while (scanner->next != NULL) {
        scanner = scanner->next;
        file << scanner->data.getFileString() << endl;
	}
    cout << "Saved." << endl;
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

bool isLegalDate(int year, int month, int day) {
    int CommonYearList[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int LeapYearList[] = {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if (month < 1 || month > 12) {
        return false;
    } else if (day < 1) {
        return false;
    } else if (month != 2 && day > CommonYearList[month]) {
        return false;
    } else if (isLeap(year) && month == 2 && day > 29) {
        return false;
    } else if (month == 2 && day > 28) {
        return false;
    }
    return true;
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
