#ifndef ADVERSARIAL_MQ_UTILS_H
#define ADVERSARIAL_MQ_UTILS_H

bool rand_bit() {
    static std::mt19937 mt{0};
    static std::uniform_int_distribution<uint32_t> dist;
    static uint32_t state;
    static int i = 32;
    if (i == 32) {
        state = dist(mt);
        i = 0;
    }
    bool bit = state & 1;
    state >>= 1;
    i++;
    return bit;
}

#endif //ADVERSARIAL_MQ_UTILS_H
