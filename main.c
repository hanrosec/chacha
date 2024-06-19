#include <stdio.h>

#include "chacha.h"

int main() {
    // Test case (https://datatracker.ietf.org/doc/html/rfc8439)
    uint32_t key[KEY_SIZE] = {
        0x03020100, 0x07060504, 0x0b0a0908, 0x0f0e0d0c,
        0x13121110, 0x17161514, 0x1b1a1918, 0x1f1e1d1c};
    uint32_t nonce[NONCE_SIZE] = {0x00000000, 0x4a000000, 0x00000000};
    uint8_t plaintext[] = {0x23, 0x4e, 0x50, 0xf2};

    uint8_t ciphertext[sizeof(plaintext)];

    printf("Encrypted: ");
    encrypt(plaintext, sizeof(plaintext), key, nonce, ciphertext);
    for (size_t i = 1; i < sizeof(plaintext)+1; i++) {
        // printf("%02x ", ciphertext[i-1]);
        printf("%d ", ciphertext[i-1]);
    }
    printf("\n");

    uint8_t plaintext2[sizeof(plaintext)];
    
    printf("Decrypted: ");
    decrypt(ciphertext, sizeof(ciphertext), key, nonce, plaintext2);
    for (size_t i = 1; i < sizeof(plaintext)+1; i++) {
        printf("%d ", plaintext[i-1]);
        // printf("%02x ", plaintext[i-1]);
    }
    printf("\n");
}