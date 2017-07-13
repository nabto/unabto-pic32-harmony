#include <unabto/unabto_stream.h>
#include <unabto/unabto_memory.h>
#include <stream_echo.h>

/**
 * Stream echo server
 *
 * The echo server first receives an ``echo'' command, then it sends a
 * ``+'' if it accepts the request. When the command is accepted the
 * state is switches to an echo state.
 *
 * recv: echo\n
 * send: +\n
 * echo data until close
 */

typedef enum {
    ECHO_STATE_IDLE,
    ECHO_STATE_READ_COMMAND,
    ECHO_STATE_COMMAND_FAIL,
    ECHO_STATE_COMMAND_OK,
    ECHO_STATE_FORWARDING,
    ECHO_STATE_CLOSING
} echo_state;

const char* echoString = "echo";

typedef struct {
    uint16_t commandLength;
    bool commandOk;
    echo_state state;
} echo_stream;

echo_stream echo_streams[NABTO_MEMORY_STREAM_MAX_STREAMS];

// Function prototypes
uint8_t setLed(uint8_t led_id, uint8_t led_on);
uint8_t readLed(uint8_t led_id);


void stream_echo_init() {
    memset(echo_streams, 0, sizeof(echo_streams));
}

void unabto_stream_accept(unabto_stream* stream) {
    echo_stream* echo = &echo_streams[unabto_stream_index(stream)];
    UNABTO_ASSERT(echo->state == ECHO_STATE_IDLE);
    memset(echo, 0, sizeof(echo_stream));
    echo->state = ECHO_STATE_READ_COMMAND;
}

void unabto_stream_event(unabto_stream* stream, unabto_stream_event_type type) {
    echo_stream* echo = &echo_streams[unabto_stream_index(stream)];

    if (echo->state == ECHO_STATE_IDLE) {
        return;
    }
    
    if (echo->state == ECHO_STATE_READ_COMMAND) {
        const uint8_t* buf;
        unabto_stream_hint hint;
        size_t readLength = unabto_stream_read(stream, &buf, &hint);
        

        if (readLength > 0) {
            size_t i;
            size_t ackLength = 0;
            
            for (i = 0; i < readLength; i++) {
                ackLength++;
                if (echo->commandLength == strlen(echoString)) {
                    if (buf[i] == '\n') {
                        echo->state = ECHO_STATE_COMMAND_OK;
                        break;
                    }
                } else {
                    if (buf[i] != echoString[echo->commandLength] || echo->commandLength > strlen(echoString)) {
                        echo->state = ECHO_STATE_COMMAND_FAIL;
                        ackLength = readLength;
                        break;
                    }
                    echo->commandLength++;
                }
                
            }
            if (!unabto_stream_ack(stream, buf, ackLength, &hint)) {
                echo->state = ECHO_STATE_CLOSING;
            }
        } else {
            if (hint != UNABTO_STREAM_HINT_OK) {
                echo->state = ECHO_STATE_CLOSING;
            }
        }
    }
    
    if (echo->state == ECHO_STATE_COMMAND_FAIL) {
        const char* failString = "-\n";
        unabto_stream_hint hint;
        unabto_stream_write(stream, (uint8_t*)failString, strlen(failString), &hint);
        echo->state = ECHO_STATE_CLOSING;
    }

    if (echo->state == ECHO_STATE_COMMAND_OK) {
        const char* okString = "+\n";
        unabto_stream_hint hint;
        size_t wrote = unabto_stream_write(stream, (uint8_t*)okString, strlen(okString), &hint);
        if (wrote != strlen(okString)) {
            echo->state = ECHO_STATE_CLOSING;
        } else {
            echo->state = ECHO_STATE_FORWARDING;
        }

    }

    if (echo->state == ECHO_STATE_FORWARDING) {
        const uint8_t* buf;
        unabto_stream_hint hint;
        size_t readLength = unabto_stream_read(stream, &buf, &hint);
        
        if (readLength > 0) {
            size_t writeLength = unabto_stream_write(stream, buf, readLength, &hint);
            if (writeLength > 0) {
                if (!unabto_stream_ack(stream, buf, writeLength, &hint)) {
                    echo->state = ECHO_STATE_CLOSING;
                }
            } else {
                if (hint != UNABTO_STREAM_HINT_OK) {
                    echo->state = ECHO_STATE_CLOSING;
                }
            }
            uint8_t led = readLed(1);
            if(led == 0){
                setLed(1, 1);
            } else {
                setLed(1, 0);
            }
        } else {
            if (hint !=  UNABTO_STREAM_HINT_OK) {
                echo->state = ECHO_STATE_CLOSING;
            }
        }
    }

    if (echo->state == ECHO_STATE_CLOSING) {
        if (unabto_stream_close(stream)) {
            unabto_stream_release(stream);
            echo->state = ECHO_STATE_IDLE;
        }
    }
}




// Set first onboard LED and return state,
// only using ID #1 in this simple example
uint8_t setLed(uint8_t led_id, uint8_t led_on)
{
    if (led_id == 1) {
        BSP_LED_STATE state = BSP_LED_STATE_OFF;
        if (led_on) {
            state = BSP_LED_STATE_ON;
        }
        BSP_LEDStateSet(BSP_LED_1, state);
        return readLed(led_id);
    }
    return -1;
}

// Return first onboard LED state,
// only using ID #1 in this simple example
uint8_t readLed(uint8_t led_id) {
    if (led_id == 1) {
        BSP_LED_STATE state = BSP_LEDStateGet(BSP_LED_1);
        if (state == BSP_LED_STATE_ON) {
            return 1;
        } else {
            return 0;
        }
    }
    return -1;
}
