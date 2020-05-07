struct Message
{
    uint16_t bufferSize;
    int *buffer;
    void trash () {
        free(buffer);
    }
    Message () {
        bufferSize = 0;
        buffer = (int *) malloc(0);
    }
};