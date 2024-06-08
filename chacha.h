/**
 * References:
 * https://datatracker.ietf.org/doc/html/rfc8439
 * https://en.wikipedia.org/wiki/Salsa20#ChaCha_variant
 * https://github.com/Ginurx/chacha20-c/blob/master/chacha20.c
 */
#ifndef CHACHA_H
#define CHACHA_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <stdio.h>

#define CONST_WORDS_SIZE 4
#define KEY_SIZE 8
#define COUNTER_SIZE 1
#define NONCE_SIZE 3
#define NUM_ROUNDS 20 // 20 rounds because they use 20 rounds in specification

#define CHACHA_BLOCK_SIZE (CONST_WORDS_SIZE + KEY_SIZE + COUNTER_SIZE + NONCE_SIZE)
#define CHACHA_BLOCK_SIZE_UINT8_t CHACHA_BLOCK_SIZE*4

#define ROTL32(x, n) ((x << n) | (x >> (32 - n)))
#define QR(a, b, c, d) (             \
	a += b, d ^= a, d = ROTL32(d, 16), \
	c += d, b ^= c, b = ROTL32(b, 12), \
	a += b, d ^= a, d = ROTL32(d,  8), \
	c += d, b ^= c, b = ROTL32(b,  7))

typedef struct chacha_ctx {
    uint32_t const_words[CONST_WORDS_SIZE]; // {0x61707865, 0x3320646e, 0x79622d32, 0x6b206574}
    uint32_t key[KEY_SIZE];
    uint32_t counter;
    uint32_t nonce[NONCE_SIZE];
    uint32_t state[CHACHA_BLOCK_SIZE];
    uint32_t keystream[CHACHA_BLOCK_SIZE];
} CHACHA_CTX;

void init_chacha_ctx(CHACHA_CTX *ctx, uint32_t *key, uint32_t *nonce, uint32_t counter);
void chacha_block(CHACHA_CTX *ctx);
void next_state(CHACHA_CTX *ctx);
void chacha_next_block(CHACHA_CTX *ctx);

void serialize(uint32_t *in, uint8_t *out);

void encrypt(uint8_t *plaintext, size_t plaintext_len, uint32_t *key, uint32_t *nonce, uint8_t *ciphertext);
void decrypt(uint8_t *ciphertext, size_t ciphertext_len, uint32_t *key, uint32_t *nonce, uint8_t *plaintext);

#endif /* CHACHA_H */