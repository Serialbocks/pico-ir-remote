#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "infrared.pio.h"

#include "infrared.h"

#define CMD_PIN 13
#define LED_PIN 25

enum STATE
{
    WAIT_USER_INPUT,
    WAIT_READY
};

int state = WAIT_USER_INPUT;
uint32_t prev_frequency = 0;

void init_pio(PIO pio, uint sm, uint offset, float freq);
void stop_pio(PIO pio, uint sm);

inline void set_frequency(uint32_t frequency, PIO pio, uint sm, uint offset) {
    if(frequency != prev_frequency) {
        if(prev_frequency > 0)
            stop_pio(pio, sm);
        init_pio(pio, sm, offset, frequency);
        prev_frequency = frequency;
    }
}

inline uint32_t usb_read_int() {
    uint32_t result;
    result = (getchar() << 24) + (getchar() << 16) + (getchar() << 8) + (getchar());
    return result;
}

inline void send_raw_frame(PIO pio, uint sm, uint offset) {
    uint32_t frequency;
    uint32_t data_len;
    float period_microseconds;
    uint32_t delay_microseconds;
    uint32_t i;

    frequency = usb_read_int();
    period_microseconds = 1000000.0f / frequency;

    set_frequency(frequency, pio, sm, offset);

    data_len = usb_read_int();

    //printf("freq: %d data_len: %d\n", frequency, data_len);

    init_pio(pio, sm, offset, frequency);

    for(i = 0; i < data_len; i++) {
        delay_microseconds = usb_read_int();
        pio_sm_put_blocking(pio, sm, (uint32_t)(delay_microseconds / period_microseconds));
    }
}

inline void send_generic_frame(PIO pio, uint sm, uint offset, InfraredProtocol protocol) {
    uint32_t frequency = infrared_get_protocol_frequency(protocol);
    float period_microseconds = 1000000.0f / frequency;
    uint32_t delay_microseconds;
    InfraredMessage message;
    uint8_t address_length_bits, address_length_bytes;
    uint8_t command_length_bits, command_length_bytes;
    uint8_t i;
    InfraredEncoderHandler* encoder;
    InfraredStatus status;
    bool level;

    set_frequency(frequency, pio, sm, offset);

    //printf("Sending frame\n");

    message.protocol = protocol;
    message.repeat = false;
    
    address_length_bits = infrared_get_protocol_address_length(protocol);
    address_length_bytes = (address_length_bits + 7) / 8; // round up to get enough bytes from uart
    command_length_bits = infrared_get_protocol_command_length(protocol);
    command_length_bytes = (command_length_bits + 7) / 8; // round up to get enough bytes from uart

    i = 1;
    message.address = getchar();
    while(i < address_length_bytes) {
        message.address = message.address << 8;
        message.address += getchar();
        i++;
    }

    i = 1;
    message.command = getchar();
    while(i < command_length_bytes) {
        message.command = message.command << 8;
        message.command += getchar();
        i++;
    }

    init_pio(pio, sm, offset, frequency);
    encoder = infrared_alloc_encoder();
    infrared_reset_encoder(encoder, &message);

    status = infrared_encode(encoder, &delay_microseconds, &level);
    if(level) {
        // always start with level 1
        pio_sm_put_blocking(pio, sm, (uint32_t)(delay_microseconds / period_microseconds));
    }
    while(status != InfraredStatusDone) {
        status = infrared_encode(encoder, &delay_microseconds, &level);
        pio_sm_put_blocking(pio, sm, (uint32_t)(delay_microseconds / period_microseconds));
    }
    
    infrared_free_encoder(encoder);
}

inline void process_command(uint8_t command, PIO pio, uint sm, uint offset)
{

    switch(command) {
        case InfraredProtocolNEC:
            {
                send_generic_frame(pio, sm, offset, InfraredProtocolNEC);
            }
            break;
        case InfraredProtocolNECext:
            {
                send_generic_frame(pio, sm, offset, InfraredProtocolNECext);
            }
            break;
        case InfraredProtocolMAX:
            send_raw_frame(pio, sm, offset);
            break;
        default:
            if(command >= 0 && command < InfraredProtocolMAX) {
                send_generic_frame(pio, sm, offset, (InfraredProtocol)command);
            }
            break;
    }
    
}

inline void state_machine(PIO pio, uint sm, uint offset)
{
    uint8_t c;
    switch(state)
    {
        case WAIT_USER_INPUT:
            c = getchar();
            process_command(c, pio, sm, offset);
            break;
        default:
            break;
    }
    
}

int main()
{
    PIO pio;
    uint sm;
    uint offset;

    stdio_init_all();
    
    bool rc = pio_claim_free_sm_and_add_program_for_gpio_range(&infrared_program, &pio, &sm, &offset, CMD_PIN, 1, true);
    hard_assert(rc);

    while (true)
    {
        state_machine(pio, sm, offset);
    }
}

void init_pio(PIO pio, uint sm, uint offset, float freq) {
    infrared_program_init(pio, sm, offset, freq, CMD_PIN);
    pio_sm_set_enabled(pio, sm, true);
}

void stop_pio(PIO pio, uint sm) {
    pio_sm_set_enabled(pio, sm, false);
}

