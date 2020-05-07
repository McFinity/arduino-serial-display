#include "command.h"

const int SHOW_COMMAND_CODE = 111222333;
const uint16_t BYTE_COUNT_FOR_BITMAP = 768;
const Message NULL_MESSAGE = Message();
const Command NULL_COMMAND = Command(NULL_MESSAGE);

Command activeCommand = NULL_COMMAND;

void setup () {
    Serial.begin(9600);
}

void loop () {
    Message message = getSerialMessage();
    if (message.bufferSize > 0) {
        if (activeCommand.commandCode == NULL_COMMAND_CODE) {
            activeCommand = Command(message);    
        }
        executeCommand(message);
        message.trash();
    }
    delay(100);
}

Message getSerialMessage () {
    uint8_t availableBytes = Serial.available();

    if (availableBytes == 0) {
        return NULL_MESSAGE;
    }

    struct Message message;
    message.bufferSize = availableBytes;
    message.buffer = (int *) malloc(sizeof(int) * availableBytes);

    for (int i = 0; i < availableBytes; i++) {
        message.buffer[i] = Serial.read();
    }

    return message;
}

void executeCommand (Message message) {
   switch (activeCommand.commandCode) {
       case SHOW_COMMAND_CODE:
            if (!activeCommand.initialized) {
                activeCommand.initData(BYTE_COUNT_FOR_BITMAP);
            }
            for (int i = 0; i < message.bufferSize; i++) {
                int currentByte = message.buffer[i];
                if (i != 0 || currentByte != SHOW_COMMAND_CODE) {
                    activeCommand.addByte(currentByte);
                }
            }
            if (activeCommand.dataIndex == BYTE_COUNT_FOR_BITMAP) {

                activeCommand.trash();
                activeCommand = NULL_COMMAND;
            }
            break;
   }
}