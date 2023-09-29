#include <unistd.h>

#include "utils.h"
#include "ncurses.h"
#include "main.h"

MessageData createMessageData (std::string message, std::string author, int colorIndex) {
    MessageData newMessage;
    strcpy(newMessage.message, message.c_str());
    strcpy(newMessage.author, author.c_str());
    strcpy(newMessage.time, "");
    newMessage.colorIndex = colorIndex;

    return newMessage;
}