#include "messageData.h"

#define ARRAY_SIZE(array) ((*(&array+1)) - array)

void print_message(const char* format, ...);
MessageData createMessageData (std::string message, std::string author, int colorIndex);