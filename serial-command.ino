#include "command.h"
#include <Adafruit_NeoPixel.h>
#define LED_PIN 6
#define HEIGHT 16
#define WIDTH 16

Adafruit_NeoPixel strip = Adafruit_NeoPixel(HEIGHT * WIDTH, LED_PIN, NEO_GRB + NEO_KHZ800);

const int SHOW_COMMAND_CODE = 100;
const uint16_t BYTE_COUNT_FOR_BITMAP = 768;
const Message NULL_MESSAGE = Message();
const Command NULL_COMMAND = Command(NULL_COMMAND_CODE);

Command activeCommand = NULL_COMMAND;

void setup()
{
    Serial.begin(9600);
    strip.begin();
    strip.setBrightness(32);
    strip.setPixelColor(0, strip.Color(255, 255, 255));
    strip.show();
}

void loop()
{
    Message message = getSerialMessage();
    if (message.bufferSize > 0)
    {
        if (activeCommand.commandCode == NULL_COMMAND_CODE)
        {
            activeCommand = Command(message);
        }
        executeCommand(message);
        message.trash();
    }
    delay(100);
}

Message getSerialMessage()
{
    uint8_t availableBytes = Serial.available();

    if (availableBytes == 0)
    {
        return NULL_MESSAGE;
    }

    struct Message message;
    message.bufferSize = availableBytes;
    message.buffer = (int *) malloc(sizeof(int) * availableBytes);

    for (int i = 0; i < availableBytes; i++)
    {
        message.buffer[i] = Serial.read();
    }

    return message;
}

void executeCommand(Message message)
{
    Serial.print("Command Code ");
    Serial.println(activeCommand.commandCode);
    if (activeCommand.commandCode != NULL_COMMAND_CODE) {
        Serial.print("Command initialized ");
        Serial.println(activeCommand.initialized);
        if (!activeCommand.initialized)
        {
            activeCommand.initData(BYTE_COUNT_FOR_BITMAP);
        }
        Serial.print("Last Index ");
        Serial.println(activeCommand.lastIndex);
        Serial.println("Bytes");
        for (int i = 0; i < message.bufferSize; i++)
        {
            int currentByte = message.buffer[i];
            Serial.println(currentByte);
            if (i != 0 || currentByte != SHOW_COMMAND_CODE)
            {
                activeCommand.addByte(currentByte);
            }
        }
        Serial.print("Data Index ");
        Serial.println(activeCommand.dataIndex);
        //if (activeCommand.dataIndex == BYTE_COUNT_FOR_BITMAP) {
        int pixelIndex = 0;
        for (int i = 0; i < BYTE_COUNT_FOR_BITMAP; i = i + 3)
        {
            int r = activeCommand.data[i];
            int g = activeCommand.data[i + 1];
            int b = activeCommand.data[i + 2];
            strip.setPixelColor(pixelIndex++, strip.Color(r, g, b));
        }
        strip.show();

        activeCommand.trash();
        activeCommand = NULL_COMMAND;
        //}
    }
}