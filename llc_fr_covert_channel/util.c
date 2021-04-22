#include "util.h"

/* Measure the time it takes to access a block with virtual address addr. */
extern inline __attribute__((always_inline))
uint64_t measure_one_block_access_time(ADDR_PTR addr) {
    uint64_t cycles;

    asm volatile("mov %1, %%r8\n\t"
            "lfence\n\t"
            "rdtsc\n\t"
            "lfence\n\t"
            "mov %%eax, %%edi\n\t"
            "mov (%%r8), %%r8\n\t"
            "lfence\n\t"
            "rdtsc\n\t"
            "lfence\n\t"
            "sub %%edi, %%eax\n\t"
    : "=a"(cycles) /*output*/
    : "r"(addr)
    : "r8", "edi");

    return cycles;
}

/*
 * CLFlushes the given address.
 */
extern inline __attribute__((always_inline))
void clflush(ADDR_PTR addr) {
    asm volatile ("clflush (%0)"::"r"(addr));
}

extern inline __attribute__((always_inline))
uint64_t rdtsc() {
    uint64_t a, d;
    asm volatile ("lfence");
    asm volatile ("rdtsc" : "=a" (a), "=d" (d));
    asm volatile ("lfence");
    return (d << 32) | a;
}


inline uint64_t get_time() {
    return rdtsc();  //It might be possible to use someother timing mech
}

extern inline __attribute__((always_inline))
uint64_t cc_sync() {
    while((get_time() & CHANNEL_SYNC_TIMEMASK) > CHANNEL_SYNC_JITTER) {}
    return get_time();
}

/*
 * Computes base to the exp.
 */
int ipow(int base, int exp)
{
    int result = 1;
    while (exp) {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        base *= base;
    }

    return result;
}


/*
 * Convert a given ASCII string to a binary string.
 * From:
 * https://stackoverflow.com/questions/41384262/convert-string-to-binary-in-c
 */
char *string_to_binary(char *s)
{
    if (s == NULL) return 0; /* no input string */

    size_t len = strlen(s) - 1;

    // Each char is one byte (8 bits) and + 1 at the end for null terminator
    char *binary = malloc(len * 8 + 1);
    binary[0] = '\0';

    for (size_t i = 0; i < len; ++i) {
        char ch = s[i];
        for (int j = 7; j >= 0; --j) {
            if (ch & (1 << j)) {
                strcat(binary, "1");
            } else {
                strcat(binary, "0");
            }
        }
    }

    return binary;
}

/*
 * Convert 8 bit data stream into character and return
 */
char *conv_msg(char *data, int size, char *msg) {
    for (int i = 0; i < size; i++) {
        char _tmp = 0;

        for (int j = i * 8; j < ((i + 1) * 8); j++) {
            _tmp = (_tmp << 1) + data[j] - '0';
        }

        msg[i] = _tmp;
    }

    msg[size] = '\0';

    return msg;
}



void init_config(struct config *config) {
    config->buffer = NULL;

    config->cache_line = CHANNEL_DEFAULT_LINE; //4K has 64 cache lines. Which cache line are we watching?
    config->interval = CHANNEL_DEFAULT_INTERVAL;

    // Flush+Reload specific paramters:
    config->shared_filename = "shared.txt";


    config->access_period = CHANNEL_FR_DEFAULT_INTERVAL;
    config->access_period = CHANNEL_FR_DEFAULT_PERIOD;
    config->miss_threshold = CHANNEL_L1_MISS_THRESHOLD;
    if (config->cache_line > 63) {
        fprintf(stderr, "ERROR: F+R channel region should be within a 4K page (64lines)!\n");
        exit(-1);
    }

    int inFile = open(config->shared_filename, O_RDONLY);
    if (inFile == -1) {
        fprintf(stderr, "ERROR: Failed to Open File\n");
        exit(-1);
    }

    size_t size = 4096; //Only 4K is shared
    config->buffer = mmap(NULL, size, PROT_READ, MAP_SHARED, inFile, 0);
    if (config->buffer == (void*) -1 ) {
        fprintf(stderr, "ERROR: Failed to Map Address\n");
        exit(-1);
    }

    ADDR_PTR addr = (ADDR_PTR) config->buffer + config->cache_line * 64;
    printf("File mapped at %p and monitoring line %lx\n", config->buffer, addr);
}