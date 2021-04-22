#include "util.h"

// receiver function pointer
bool (*detect_bit)(const struct config*);

bool detect_bit_fr(const struct config *config) {
    int misses = 0;
    int hits = 0;
    int total_measurements = 0;

    // This is high because the misses caused by clflush
    // usually cause an access time larger than 150 cycles

    uint64_t start_t = rdtsc();
    while ((rdtsc() - start_t) < config->interval) {
        ADDR_PTR addr = (ADDR_PTR) config->buffer + config->cache_line * 64;
        uint64_t time = measure_one_block_access_time(addr);

        // When the access time is larger than 1000 cycles,
        // it is usually due to a disk miss. We exclude such misses
        // because they are not caused by clflush.
        if (time < 1000) {
            total_measurements++;
            if (time > config->miss_threshold) {
                misses++;
            } else {
                hits++;
            }
        }

        // Busy loop to give time to the sender to flush the cache
        uint64_t wait_t = rdtsc();
        while((rdtsc() - wait_t) < config->access_period &&
                   (rdtsc() - start_t) < config->interval);
    }


    bool ret =  misses > (float) total_measurements / 2.0;
    return ret;
}


int main()
{
    // Initialize config and local variables
    struct config config;

    init_config(&config);
    detect_bit = detect_bit_fr;

    char msg_ch[MAX_BUFFER_LEN + 1];
    int flip_sequence = 4;
    bool current;
    bool previous = true;


    printf("Press enter to begin listening ");
    getchar();
    while (1) {

        // cc_sync on clock edge
        cc_sync();
        current = detect_bit(&config);

        // Receiver sync protocol
        if (flip_sequence == 0 && current == 1 && previous == 1) {

            uint32_t msg_len = 0, strike_zeros = 0;
            cc_sync();
            for (msg_len = 0; msg_len < MAX_BUFFER_LEN; msg_len++) {

                // uint32_t bit = detect_bit(&config, start_t);
                uint32_t bit = detect_bit(&config);
                msg_ch[msg_len] = '0' + bit;
                strike_zeros = (strike_zeros + (1-bit)) & (bit-1);
                if (strike_zeros >= 8 && ((msg_len & 0x7) == 0)) {
                    break;
                }

            }

            msg_ch[msg_len - 8] = '\0';
            printf("message %s received\n", msg_ch);

            uint32_t ascii_msg_len = msg_len / 8;
            char msg[ascii_msg_len];
            printf("> %s\n", conv_msg(msg_ch, ascii_msg_len, msg));

        } else if (flip_sequence > 0 && current != previous) {
            flip_sequence--;

        } else if (current == previous) {
            flip_sequence = 4;
        }

        previous = current;
    }

    printf("Receiver finished\n");
    return 0;
}
