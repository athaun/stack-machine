// Asher Haun
// CSCI 4335
// Computer Architecture
// Stack Machine

#include <curses.h>
#include <sys/fcntl.h>

#include "messageData.h"
#include "main.h"
#include "utils.h"
#include "ncurses.h"

bool running = true;

std::vector<MessageData> messages; // Local message memory for display

void sigint_handler(int sig_num) {
    endwin();
    exit(0);
}

void set_non_blocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    flags |= O_NONBLOCK;
    flags = fcntl(fd, F_SETFL, flags);
}

int main () {

    setup_ncurses();

    // Set input to nonBlocking (raw input mode)
    set_non_blocking(fileno(stdin));

    // Catching ^C
    signal(SIGINT, sigint_handler); 
    
    // Main loop
    while (running) {
        erase();

        event_listener();

        // Display the graphics        
        output();

        input();
    }

    endwin();

    return 0;
}