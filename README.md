There is an account [@ProgressBar2017](https://twitter.com/ProgressBar2017 "Twitter@ProgressBar2017") in Twitter that will post the progress of 2017 every 1% automatically.

Since I started to learn Data Struct lesson this semester, I've learned the linked list more systematically. So I decided to have a try.

This is my first C++ project (means it's not a homework or something else very simple), and my largest project ever (more than 750 lines).

This project is NOT finished yet. Although it could run normally, but there are still many bugs to fix. If it crashes, don't panic.

# Main Functions
* File Open & Save:  
    Save all events in a file.
* Display events:  
    Display all events with progress bars.
* Event list edit:  
    Edit every single event, add new events and delete events.

# To Windows OS Users
This program is using a header file `sys/ioctl.h` that seems only provided in UNIX OS. I use it to get the width of Terminal window and make the menu and progress bar be fit for the window.

If you want to compile and run the program in Windows OS successfully, please follow these steps below:

1. Delete the `line 8`:  

	```c
	#include <sys/ioctl.h>
	```

2. Replace the `line 13`:

	```c
	int WINDOW_WIDTH;
	```

	with

	```c
	int WINDOW_WIDTH = 80;
	```

	or any width you want.

3. Delete the line 113~115:

	```c
	struct winsize windowSize;
    ioctl(0, TIOCGWINSZ, &windowSize);
    WINDOW_WIDTH = windowSize.ws_col;
	```

You can also find the direction comments in the source code.
