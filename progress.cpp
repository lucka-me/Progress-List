#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>
#include <time.h>
// In Windows OS, delete the following line
#include <sys/ioctl.h>

using namespace std;

// In Windows OS, assign WINDOW_WIDTH a value such as 100
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
    friend bool isLegalDate(Date d);
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
        string  init_title          = "",
        Date    init_startDate      = Date(),
        Date    init_endDate        = Date(),
        string  init_description    = ""
    );
    ~Event(void) {};
    int initiate(void);
    int update(int cmd, string newInfo);
    int update(int cmd, Date newDate);
    int getLength(void);
    double getProgress(void);
    string getDisplayString(void);
    string getFileString(void);
};

typedef struct _EventNode {
    Event data;
    struct _EventNode *next;
} EventNode;

class EventListClass {
private:
    EventNode *listHead;
public:
    EventListClass(void);
    ~EventListClass(void) {};
    int eventCount;
    Event &getEvent(int position);
    int addEvent(Event newEvent, int position = 0);
    Event deleteEvent(int position = 0);
};

class consoleClass {
private:
    string filename;
    EventListClass eventList;
public:
    consoleClass(void);
    ~consoleClass(void){};
    int commandList[2];
    EventListClass &buildEventList(void);
    void setEventList(EventListClass &newList);
    int getCommand(bool isMenuDisplay = true);
    string getFilenName(void);
    EventListClass &getEventList(void);
    int displayAllEvet(void);
    Date getNewDate(string name = "");
    Event getNewEvent(void);
    int addEvent(Event newEvent, int position = 0);
    Event deleteEvent(int position = 0);
    int editEvent(int position = 0);
    int save(void);
};

bool isLeap(int year);
bool isLegalDate(int year, int month, int day);
bool isLegalDate(Date d);
Date getToday(void);
string to_string(int number, int width);
string to_string(double number, int precision);

/************************************************/
/*                     main                     */
/************************************************/

int main(int argc, char const *argv[]) {
    // Get the terminal window size
    // In Windows OS, delete the following THREE lines
    struct winsize windowSize;
    ioctl(0, TIOCGWINSZ, &windowSize);
    WINDOW_WIDTH = windowSize.ws_col;

    consoleClass console;
    int command;
    bool exitFlag = false;
    bool isMenuDisplay = true;

    console.getFilenName();
    console.buildEventList();

    do {
        command = console.getCommand(isMenuDisplay);
        isMenuDisplay = true;
        switch (command) {
            case -1:
                isMenuDisplay = false;
                break;
            case 1:
                console.displayAllEvet();
                break;
            case 2:
                console.addEvent(console.getNewEvent(), console.commandList[1]);
                break;
            case 3:
                console.deleteEvent(console.commandList[1]);
                isMenuDisplay = false;
                break;
            case 4:
                console.editEvent(console.commandList[1]);
                break;
            case 5:
                console.save();
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
    string  init_description
) {
    title       = init_title;
    description = init_description;
    startDate   = init_startDate;
    endDate     = init_endDate;
    this->initiate();
}

int Event::initiate(void) {
    int flag = 0;
    Date today = getToday();
    length = endDate - startDate;
    past = today - startDate;
    // If the event has finished
    past = past >= length ? length : past;
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
int Event::update(int cmd, string newInfo) {
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
            flag = 11;
            break;
        default:
            flag = 1;
            break;
    }
    if (!flag) {
        this->initiate();
    }
    return flag;
}

int Event::update(int cmd, Date newDate) {
    int flag = 0;
    switch (cmd) {
        case 1:
        case 2:
            flag = 21;
            break;
        case 3:
            startDate = newDate;
            break;
        case 4:
            endDate = newDate;
            break;
        default:
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
        result += " [" + description + ']';
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
/*                EventListClass                */
/************************************************/
EventListClass::EventListClass(void) {
    listHead = new EventNode;
    listHead->next = NULL;
    eventCount = 0;
}

Event &EventListClass::getEvent(int position) {
    EventNode *scanner = listHead;

    // position == 0 -> the last
    if (position == 0) {
        while (scanner->next != NULL) {
            scanner = scanner->next;
        }
    } else {
        int counter = 0;
        while (counter < position && scanner->next != NULL) {
            counter++;
            scanner = scanner->next;
        }
    }
    return scanner->data;
}

int EventListClass::addEvent(Event newEvent, int position) {
    int flag = 0;
    EventNode *scanner = listHead;
    EventNode *newEventNode = new EventNode;

    if (position > eventCount) position = 0;
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
    EventListClass::eventCount++;
    return flag;
}

Event EventListClass::deleteEvent(int position) {
    Event result;
    EventNode *scanner = listHead;
    EventNode *lastNode = listHead;

    if (position > eventCount) position = 0;
    // position == 0 -> delete the last
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
    EventListClass::eventCount--;
    cout << "Event deleted." << endl;
    return result;
}


/************************************************/
/*                 consoleClass                 */
/************************************************/
consoleClass::consoleClass(void) {
    filename = "";
}

void consoleClass::setEventList(EventListClass &newList) {
    eventList = newList;
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

EventListClass &consoleClass::buildEventList(void) {
    EventNode *EventListHead = new EventNode;
    EventNode *lastNode, *newNode;
    EventListHead->next = NULL;
    lastNode = EventListHead;

    // One line is like:
    // #[Term][This term][2017 2 27][2017 7 7]
    string  title, description;
    int     startDateYear, startDateMonth, startDateDay;
    int     endDateYear, endDateMonth, endDateDay;
    Date    startDate, endDate;
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
        // Check if the date is legal
        startDate = Date(startDateYear, startDateMonth, startDateDay);
        endDate = Date(endDateYear, endDateMonth, endDateDay);
        if (!isLegalDate(startDate)) {
            cout << "ERROR: illegal date." << endl
                 << "ERROR Information:" << endl
                 << " - Title: " << title << endl
                 << " - Description: " << description << endl
                 << " - Start data: "
                 << startDateYear
                 << "-" << setfill('0') << setw(2) << startDateMonth
                 << "-" << setfill('0') << setw(2) << startDateDay << endl
                 << "It's an illegal data! Please check your Database file." << endl;
        } else if (!isLegalDate(endDate)) {
            cout << "ERROR: illegal date." << endl
                 << "ERROR Information:" << endl
                 << " - Title: " << title << endl
                 << " - Description: " << description << endl
                 << " - End data: "
                 << endDateYear
                 << "-" << setfill('0') << setw(2) << endDateMonth
                 << "-" << setfill('0') << setw(2) << endDateDay << endl
                 << "It's an illegal data! Please check your Database file." << endl;
        } else if (endDate - startDate <= 0) {
            cout << "ERROR: illegal length." << endl
                 << "ERROR Information:" << endl
                 << " - Title: " << title << endl
                 << " - Description: " << description << endl
                 << " - Start data: "
                 << startDateYear
                 << "-" << setfill('0') << setw(2) << startDateMonth
                 << "-" << setfill('0') << setw(2) << startDateDay << endl
                 << " - End data: "
                 << endDateYear
                 << "-" << setfill('0') << setw(2) << endDateMonth
                 << "-" << setfill('0') << setw(2) << endDateDay << endl
                 << "The start date is later than or same of the end date. Please check your Database file." << endl;
        } else {
            eventList.addEvent(Event(
                title,
                startDate,
                endDate,
                description
            ));
        }
    }

    return eventList;
}

int consoleClass::displayAllEvet(void) {
    int flag = 0;

    for (int scanner = 1; scanner <= eventList.eventCount; scanner++) {
        cout << setfill('0') << setw(2) << scanner << ". "<< eventList.getEvent(scanner).getDisplayString() << endl;
    }
    cout << eventList.eventCount << " events." << endl;
    return flag;
}

Date consoleClass::getNewDate(string name) {
    int     year, month, day;
    cout << "Enter the " << name << " date (format: yyyy mm dd): ";
    cin >> year >> month >> day;
    while (!isLegalDate(year, month, day)) {
        cout << "Invalid date!" << endl
             << "Enter the begining date (format: yyyy mm dd): ";
        cin >> year >> month >> day;
    }
    return Date(year, month, day);
}

Event consoleClass::getNewEvent(void) {
    string  title = "", description = "";
    Date startDate, endDate;

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

    startDate = getNewDate("begining");
    endDate = getNewDate("ending");
    while (startDate - endDate <= 0) {
        cout << "ERROR: illegal length." << endl
             << "ERROR Information:" << endl
             << "The start date is later than or same of the end date. Please enter again." << endl;
             startDate = getNewDate("begining");
             endDate = getNewDate("ending");
    }

    return Event(title, startDate, endDate, description);
}

int consoleClass::addEvent(Event newEvent, int position) {
    return eventList.addEvent(newEvent, position);
}

Event consoleClass::deleteEvent(int position) {
    return eventList.deleteEvent(position);
}

int consoleClass::editEvent(int position) {
    int flag = 0;
    int command;
    bool isGiveup = false;

    cout << "/*" << setfill('*') << setw(WINDOW_WIDTH - 4) << '*' << "*/" << endl
         << "/*" << setfill(' ') << setw((WINDOW_WIDTH - 14) / 2) << ' ' << "Edit Event" << setfill(' ') << setw((WINDOW_WIDTH - 14) / 2) << ' ' << "*/" << endl
         << "/*" << setfill('*') << setw(WINDOW_WIDTH - 4) << '*' << "*/" << endl
         << setfill('0') << setw(2) << position << ". "<< eventList.getEvent(position).getDisplayString() << endl
         << "/*" << setfill('*') << setw(WINDOW_WIDTH - 4) << '*' << "*/" << endl
         << "/* 1. Update the title"        << setfill(' ') << setw(WINDOW_WIDTH - 24) << ' ' << "*/" << endl
         << "/* 2. Update the description"  << setfill(' ') << setw(WINDOW_WIDTH - 30) << ' ' << "*/" << endl
         << "/* 3. Update the startDate"    << setfill(' ') << setw(WINDOW_WIDTH - 28) << ' ' << "*/" << endl
         << "/* 4. Update the endDate"      << setfill(' ') << setw(WINDOW_WIDTH - 26) << ' ' << "*/" << endl
         << "/* 5. Give up"                 << setfill(' ') << setw(WINDOW_WIDTH - 15) << ' ' << "*/" << endl
         << "/*" << setfill('*') << setw(WINDOW_WIDTH - 4) << '*' << "*/" << endl
         << "Enter the command: ";

    while (!isGiveup) {
        cin >> command;
        switch (command) {
            case 1: {
                string title;
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
                flag = eventList.getEvent(position).update(command, title);
                break;
            }
            case 2: {
                string description;
                // Clean the istream
                while (cin.get() != '\n');
                cout << "Enter the description (optional):" << endl;
                getline(cin, description, '\n');
                flag = eventList.getEvent(position).update(command, description);
                break;
            }
            case 3: {
                flag = eventList.getEvent(position).update(command, getNewDate("new startDate"));
                break;
            }
            case 4: {
                flag = eventList.getEvent(position).update(command, getNewDate("new endDate"));
                break;
            }
            case 5: {
                isGiveup = true;
                break;
            }
            default: {
                cout << "Invalid command!";
                break;
            }
        }
        if (!flag) isGiveup = true;
    }
    return flag;
}

int consoleClass::save(void) {
    int flag = 0;

    ofstream file;
    file.open(filename, ios::out);
    for (int scanner = 1; scanner < eventList.eventCount; scanner++) {
        file << eventList.getEvent(scanner).getFileString() << endl;
    }
    cout << "Saved." << endl;
    return flag;
}
/************************************************/
/*                   Operator                   */
/************************************************/
int operator - (Date &endDate, Date &startDate) {
    if (
        (endDate.year == startDate.year) &&
        (endDate.month == startDate.month) &&
        (endDate.day == startDate.day)
    ) {
        return 0;
    }

    // If endDate < startDate
    bool isNegative = false;
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
        isNegative = true;
        Date temp = endDate;
        endDate = startDate;
        startDate = temp;
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
            } else {
                result += CommonYearList[startDate.month] - startDate.day;
                for (int scanner = startDate.month + 1; scanner < endDate.month; scanner++) {
                    result += CommonYearList[scanner];
                }
            }
            result += endDate.day;
        }
    } else {
        // In the different year
        // Start in leap year or common year
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
        // End in leap year or common year
        if (endDate.isLeap()) {
            for (int scanner = 1; scanner < endDate.month; scanner++) {
                result += LeapYearList[scanner];
            }
        } else {
            for (int scanner = 1; scanner < endDate.month; scanner++) {
                result += CommonYearList[scanner];
            }
        }
        result += endDate.day;
        // Years between start and end
        for (int scanner = startDate.year + 1; scanner < endDate.year; scanner++) {
            result += isLeap(scanner) ? 366 : 365;
        }
    }

    if (isNegative) {
        result = 0 - result;
        Date temp = endDate;
        endDate = startDate;
        startDate = temp;
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

bool isLegalDate(Date d) {
    int year = d.year;
    int month = d.month;
    int day = d.day;
    return isLegalDate(year, month, day);
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
