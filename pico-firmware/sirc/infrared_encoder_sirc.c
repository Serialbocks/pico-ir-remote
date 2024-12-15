#include "infrared_protocol_sirc_i.h"

void infrared_encoder_sirc_reset(void* encoder_ptr, const InfraredMessage* message) {

    InfraredCommonEncoder* encoder = encoder_ptr;
    infrared_common_encoder_reset(encoder);

    uint32_t* data = (void*)encoder->data;

    if(message->protocol == InfraredProtocolSIRC) {
        *data = (message->command & 0x7F);
        *data |= (message->address & 0x1F) << 7;
        encoder->bits_to_encode = 12;
    } else if(message->protocol == InfraredProtocolSIRC15) {
        *data = (message->command & 0x7F);
        *data |= (message->address & 0xFF) << 7;
        encoder->bits_to_encode = 15;
    } else if(message->protocol == InfraredProtocolSIRC20) {
        *data = (message->command & 0x7F);
        *data |= (message->address & 0x1FFF) << 7;
        encoder->bits_to_encode = 20;
    }
}

InfraredStatus infrared_encoder_sirc_encode_repeat(
    InfraredCommonEncoder* encoder,
    uint32_t* duration,
    bool* level) {

    *duration = INFRARED_SIRC_REPEAT_PERIOD - encoder->timings_sum;
    *level = false;

    encoder->timings_sum = 0;
    encoder->timings_encoded = 1;
    encoder->bits_encoded = 0;
    encoder->state = InfraredCommonEncoderStatePreamble;

    return InfraredStatusOk;
}

void* infrared_encoder_sirc_alloc(void) {
    return infrared_common_encoder_alloc(&infrared_protocol_sirc);
}

void infrared_encoder_sirc_free(void* encoder_ptr) {
    infrared_common_encoder_free(encoder_ptr);
}

InfraredStatus infrared_encoder_sirc_encode(void* encoder_ptr, uint32_t* duration, bool* level) {
    InfraredCommonEncoder* encoder = encoder_ptr;

    InfraredStatus status = infrared_common_encode(encoder, duration, level);
    if((status == InfraredStatusOk) && (encoder->bits_encoded == encoder->bits_to_encode)) {
        status = InfraredStatusDone;
        encoder->state = InfraredCommonEncoderStateEncodeRepeat;
    }
    return status;
}
