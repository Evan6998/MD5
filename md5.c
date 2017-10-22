#include "md5.h"

void fillWithVal(uint32_t val, uint8_t *bytes) {
    bytes[0] = (uint8_t)val;
    bytes[1] = (uint8_t)(val>>8);
    bytes[2] = (uint8_t)(val>>16);
    bytes[3] = (uint8_t)(val>>24);
}

uint32_t extract32bits(const uint8_t *bytes) {
    return (uint32_t) bytes[0]
    | ((uint32_t) bytes[1] << 8)
    | ((uint32_t) bytes[2] << 16)
    | ((uint32_t) bytes[3] << 24);
}

void md5(const uint8_t *message, size_t len, uint8_t *digest) {
    uint8_t *msg;
    size_t newLen;

    // Step 1. Append Padding Bits
    // append len till congruent to 448, modulo 512
    for(newLen = len+1; newLen % (512/8) != (448/8); newLen++);

    // +8 bytes to append len
    msg = (uint8_t*)malloc(newLen+8);
    memcpy(msg, message, len);
    // 0x80 represent 128, that is, 10000000
    msg[len] = 0x80;
    for(size_t i = len+1; i < newLen; i++) {
        msg[i] = 0;
    }

    // Step 2. Append Length (represent by bits)
    // append low 32 bits
    fillWithVal(len*8, msg+newLen);
    // append high 32 bits 
    fillWithVal(len*8>>32, msg+newLen+4);

    // Step 3. Initialize MD Buffer
    uint32_t A = 0x67452301;
    uint32_t B = 0xefcdab89;
    uint32_t C = 0x98badcfe;
    uint32_t D = 0x10325476;

    // Step 4. Process Message in 16-Word Blocks
    uint32_t a, b, c, d, f, g, temp;

    for(size_t offset = 0; offset < newLen; offset += (512/8)) {
        uint32_t w[16];
        for(size_t i = 0; i < 16; i++) {
            // i*4 represent 4 bytes per w[i]
            w[i] = extract32bits(msg + offset + i*4);
        }

        a = A;
        b = B;
        c = C;
        d = D;

        for(int i = 0; i < 64; i++) {
            if(i < 16) {
                f = F(b, c, d);
                g = i;
            } else if (i < 32) {
                f = F(d, b, c);
                g = (5*i+1) % 16;
            } else if (i < 48) {
                f = H(b, c, d);
                g = (3*i+5) % 16;
            } else if (i < 64) {
                f = I(b, c, d);
                g = (7*i) % 16;
            }
            temp = d;
            d = c;
            c = b;
            b = ROTATE_LEFT((a+f+T[i]+w[g]), r[i]) + b;
            a = temp;
        }

        A += a;
        B += b;
        C += c;
        D += d;
    }

    free(msg);
    
    fillWithVal(A, digest);
    fillWithVal(B, digest+4);
    fillWithVal(C, digest+8);
    fillWithVal(D, digest+12);
}