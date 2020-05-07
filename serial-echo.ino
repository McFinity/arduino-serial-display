struct Message
{
    uint8_t bufferSize;
    int *buffer;
    Message () {
        bufferSize = 0;
        buffer = (int *) malloc(0);
    }
};

const struct Message NULL_MESSAGE;

void setup () {
    Serial.begin(9600);
}

void loop () {
    Message message = getSerialMessage();
    for (int i = 0; i < message.bufferSize; i++) {
        Serial.write(message.buffer[i]);
    }
    if (message.bufferSize > 0) {
        deleteMessage(message);
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

void deleteMessage (Message message) {
    free(message.buffer);
}