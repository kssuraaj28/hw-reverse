#include "util.h"


// sender function pointer
void (*send_bit)(bool, const struct config*);

void send_bit_fr(bool one, const struct config *config) {
    uint64_t start_t = rdtsc();

    if (one) {
        ADDR_PTR addr = (ADDR_PTR) config->buffer + config->cache_line * 64;
        while ((rdtsc() - start_t) < config->interval) {
            clflush(addr);
        }

    } else {
        start_t = rdtsc();
        while (rdtsc() - start_t < config->interval) {}
    }
}
/*
 * Sends a bit to the receiver by repeatedly flushing the addresses of the addr_set
 * for the clock length of config->interval when we are sending a one, or by doing nothing
 * for the clock length of config->interval when we are sending a zero.
 */


int main()
{
    // Initialize config and local variables
    struct config config;
    init_config(&config);

    send_bit = send_bit_fr;

    printf("Please type a message.\n");

    while (1) {
        // Get a message to send from the user
        printf("< ");
        char text_buf[128];
        if(!fgets(text_buf, sizeof(text_buf), stdin)) 
		{
			fprintf(stderr,"Cannot read from stream");
			return 1;
		}

        char *msg = string_to_binary(text_buf);

        size_t msg_len = strlen(msg);

        // cc_sync on clock edge
        uint64_t start_t =  cc_sync();

        for (int i = 0; i < 10; i++) {
            start_t = cc_sync();
            // send_bit(i % 2 == 0, &config, start_t);
            send_bit(i % 2 == 0, &config);
        }
        start_t = cc_sync();
        // send_bit(true, &config, start_t);
        send_bit(true, &config);
        start_t = cc_sync();
        // send_bit(true, &config, start_t);
        send_bit(true, &config);

        // Send the message bit by bit
        start_t = cc_sync();
        // TODO: for longer messages it is recommended to re-sync every X bits
        for (uint32_t ind = 0; ind < msg_len; ind++) {
            if (msg[ind] == '0') {
                // send_bit(false, &config, start_t);
                send_bit(false, &config);
            } else {
                // send_bit(true, &config, start_t);
                send_bit(true, &config);
            }
            start_t += config.interval;
        }

        printf("message %s sent\n", msg);
    }


    printf("Sender finished\n");
    return 0;
}
