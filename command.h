#include "message.h"

const int NULL_COMMAND_CODE = 0;

struct Command
{
    int commandCode = NULL_COMMAND_CODE;
    int *data;
    uint16_t dataIndex = 0;
    uint16_t lastIndex = 0;
    bool initialized = false;
    void initData (uint16_t expectedByteCount) {
        free(data);
        data = (int*) malloc(sizeof(int) * expectedByteCount);
        //memset(data, 0, sizeof(int) * expectedByteCount);
        dataIndex = 0;
        lastIndex = expectedByteCount - 1;
        initialized = true;
    }
    void addByte (int byte) {
        if (initialized && dataIndex <= lastIndex) {
            data[dataIndex++] = byte;
        } 
    }
    void trash () {
        if (initialized) {
            free(data);
        }
    }
    Command (Message message) {
        data = (int*) malloc(0);
        if (message.bufferSize > 0) {
            commandCode = message.buffer[0];
        }
    }
    Command (int code) {
        data = (int*) malloc(0);
        commandCode = code;
    }
};