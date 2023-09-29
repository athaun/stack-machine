#include <curses.h>
#include <iostream>
#include <string>
#include <regex>

#include "messageData.h"
#include "ncurses.h"
#include "main.h"
#include "utils.h"
#include "stack.h"

std::string input_string;
int last_num_index = 0;

int terminal_width;
int terminal_height;

Stack<double> stack;

void setup_ncurses () {
    initscr();
    curs_set(FALSE);
    clear();
    noecho();

    // Save the terminal width and height to variables
    getmaxyx(stdscr, terminal_height, terminal_width);

    // Define the color pairs for common colors
    start_color();
    init_pair(1, COLOR_YELLOW, DEFAULT_BACKGROUND);
    init_pair(2, COLOR_CYAN, DEFAULT_BACKGROUND);
    init_pair(3, COLOR_MAGENTA, DEFAULT_BACKGROUND);
    init_pair(4, COLOR_RED, DEFAULT_BACKGROUND);
    init_pair(5, COLOR_BLUE, DEFAULT_BACKGROUND);
    init_pair(6, COLOR_GREEN, DEFAULT_BACKGROUND);
    init_pair(7, COLOR_WHITE, DEFAULT_BACKGROUND);
}

void handle_input () {

    std::string last_input = input_string.substr(last_num_index, -1);

    // Increase last_num_index to the new length after the the last input
    last_num_index += last_input.length() + 1;

    if (std::regex_match(last_input, std::regex("[0-9]*\\.?[0-9]+"))) {
        // If the input is a number (int or float), push it

        print_message("Pushed number %s", last_input.c_str());
        stack.push(std::stod(last_input));
    } else {
        
        if (!std::regex_match(last_input, std::regex("[\\+\\-\\*\\/\\^]"))) {
            // check if the input is a valid operator using regex

            print_message("Can't munch that operator! %s", last_input.c_str());
            input_string = input_string.substr(0, input_string.size() - last_input.length() - 1);
            last_num_index -= last_input.length() + 1;
            
            return;
        }        
        
        if (stack.has_two()) {
            // If the input is an operator, pop two numbers and perform the operation
            const auto operand_2 = stack.pop();
            const auto operand_1 = stack.pop();

            print_message("Munched operator %c | pop %f and %f", last_input[0], operand_1, operand_2);

            double result = 0.0;

            // Extract the operator from input_string
            char op = last_input[0];

            switch (op) {
                case '+':
                    result = operand_1 + operand_2;
                    break;
                case '-':
                    result = operand_1 - operand_2;
                    break;
                case '*':
                    result = operand_1 * operand_2;
                    break;
                case '/':
                    if (operand_2 != 0) {
                        result = operand_1 / operand_2;
                    } else {
                        print_message("Error: Division by zero.");
                    }
                    break;
            }
            
            // Push the result back onto the stack
            stack.push(result);

            // Print the result
            print_message("%.1f %.1f %c = %.1f", operand_1, operand_2, op, result);
            print_message("Pushed number %.1f", result);
        } else {
            print_message("Error: Not enough operands to munch for '%c'", last_input[0]);
        }
    }
}

void event_listener () {
    static int lastterminal_width = terminal_height, lastterminal_height = terminal_width;
    getmaxyx(stdscr, terminal_height, terminal_width); // Update to latest screen resolution
    
    if (lastterminal_width != terminal_width || lastterminal_height != terminal_height) {
        // If the terminal size has changed, redraw the screen
        lastterminal_width = terminal_width;
        lastterminal_height = terminal_height;

        messages.clear();
    }
}

void input () {
    char input_character = getch();

    if (input_character > 0) {
        if (input_character == ' ') {
            // Space key
            handle_input();
            input_string += ' ';
            
        } else if (input_character == '\n') {
            // Return key
            if (input_string == "clear") {
                messages.clear();

                input_string = "";
                last_num_index = 0;

                stack.clear();
                return;
            }

            // Print a new line
            if (input_string != "") { 
                print_message("Expression: %s", input_string.c_str());
                print_message("Result: %f", stack.stack[stack.top]);
                print_message(" ");
                print_message("--------------------");
                print_message(" ");
                
                input_string = "";
                last_num_index = 0;
            }
        } else if (input_character >= 32 && input_character <= 126) {

            // Acceptable ASCII range (see man ascii)
            input_string += input_character;
        }
    }
}

void output () {
    // Draw the messages at the top left and increase down
    int j = messages.size() - 1;
    for (int i = terminal_height - 2; i >= 0; i --) {
        move(i, 0);
        if (j >= 0) {
            printw("%s", messages.at(j).message);
        }
        j --;
    }

    // Input prompt
    mvprintw(terminal_height - 1, 0, "$ %s", input_string.c_str());

    // Draw the stack at the bottom right and increase up
    j = 0;
    for (int i = terminal_height - 2; i >= 0; i --) {
        move(i, terminal_width - 20 - (stack.stack[j] < 0 ? 1 : 0));
        if (j <= stack.top) {
            printw("%f", stack.stack[j]);
            if (j == stack.top) {
                // Draw an arrow pointing to the top of the stack
                move(i, terminal_width - 20 - (stack.stack[j] < 0 ? 1 : 0) - 10);
                printw("(top) --> ");
            }
        }
        j ++;
    }
}

void print_message (const char* format, ...) {
    va_list va_pointer;
    va_start(va_pointer, format); // Initialize va_pointer to the variable arguements after format
    
    char temp[2048];
    vsnprintf(temp, sizeof(temp), format, va_pointer); // Using variable arguements to print to a string

    va_end(va_pointer); // Cleaning the variable arguements
    MessageData data = createMessageData(temp, "noprint", WHITE_COLOR);
    if (strcmp(data.message, "") == 0) return;
    messages.push_back(data);
}