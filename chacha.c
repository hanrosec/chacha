#include "chacha.h"

void init_chacha_ctx(CHACHA_CTX *ctx, uint32_t *key, uint32_t *nonce, uint32_t counter) {
    ctx->const_words[0] = 0x61707865;
    ctx->const_words[1] = 0x3320646e;
    ctx->const_words[2] = 0x79622d32;
    ctx->const_words[3] = 0x6b206574;
    ctx->counter = counter;
    memcpy(ctx->key, key, 8 * sizeof(uint32_t));
    memcpy(ctx->nonce, nonce, 3 * sizeof(uint32_t));
    
    // Adding const words to state
    size_t i = 0;
    for (size_t j = 0; j < CONST_WORDS_SIZE; j++)
        ctx->state[i++] = ctx->const_words[j];

    // Adding key to state
    for (size_t j = 0; j < KEY_SIZE; j++)
        ctx->state[i++] = ctx->key[j];
    
    // Adding counter to state
    ctx->state[i++] = ctx->counter;

    // adding nonce to state
    for (size_t j = 0; j < NONCE_SIZE; j++)
        ctx->state[i++] = ctx->nonce[j];
}

void serialize(uint32_t *in, uint8_t *out) {
    /**
     * @brief Function to convert uint32_t keystream to uint8_t keystream
     * 
     */
    for (size_t i = 0; i < CHACHA_BLOCK_SIZE; i++) {
        *out++ = (uint8_t)(in[i] & 0xFF);
        *out++ = (uint8_t)((in[i] >> 8) & 0xFF);
        *out++ = (uint8_t)((in[i] >> 16) & 0xFF);
        *out++ = (uint8_t)((in[i] >> 24) & 0xFF);
    }
}

void next_state(CHACHA_CTX *ctx) {
    init_chacha_ctx(ctx, ctx->key, ctx->nonce, ++ctx->counter);
}

void chacha_block(CHACHA_CTX *ctx) {
    for (size_t i = 0; i < CHACHA_BLOCK_SIZE; i++)
        ctx->keystream[i] = ctx->state[i];
    
    for (size_t i = 0; i < NUM_ROUNDS/2; i++) {
        QR(ctx->keystream[0], ctx->keystream[4], ctx->keystream[ 8], ctx->keystream[12]);
		QR(ctx->keystream[1], ctx->keystream[5], ctx->keystream[ 9], ctx->keystream[13]);
		QR(ctx->keystream[2], ctx->keystream[6], ctx->keystream[10], ctx->keystream[14]);
		QR(ctx->keystream[3], ctx->keystream[7], ctx->keystream[11], ctx->keystream[15]);
		QR(ctx->keystream[0], ctx->keystream[5], ctx->keystream[10], ctx->keystream[15]);
		QR(ctx->keystream[1], ctx->keystream[6], ctx->keystream[11], ctx->keystream[12]);
		QR(ctx->keystream[2], ctx->keystream[7], ctx->keystream[ 8], ctx->keystream[13]);
		QR(ctx->keystream[3], ctx->keystream[4], ctx->keystream[ 9], ctx->keystream[14]);
    }

    for (size_t i = 0; i < CHACHA_BLOCK_SIZE; i++)
        ctx->keystream[i] += ctx->state[i];
}

void chacha_next_block(CHACHA_CTX *ctx) {
    chacha_block(ctx);
    ctx->state[12]++;
    if (ctx->state[12] == 0) {
        ctx->state[13]++;
    }
}

void chacha_generate_keystream(CHACHA_CTX *ctx, size_t plaintext_len, uint32_t keystream[]) {
    size_t keystream_len = (plaintext_len + CHACHA_BLOCK_SIZE - 1) / CHACHA_BLOCK_SIZE; 
    // How many blocks of keystream are needed to cover whole plaintext
    for (size_t i = 0; i < keystream_len; i++) {
        chacha_next_block(ctx);
        for (size_t j = 0; j < CHACHA_BLOCK_SIZE; j++) {
            keystream[(i * CHACHA_BLOCK_SIZE) + j] = ctx->keystream[j];
        }
    }
}

void encrypt(uint8_t *plaintext, size_t plaintext_len, uint32_t *key, uint32_t *nonce, uint8_t *ciphertext) {
    /**
     * 1. Initalizing ctx
     * 2. Generating whole keystream of length n, where n is first multiple of 16 greater than plaintext length
     * 3. Encrypting:
     *      for i where i is length of plaintext:
     *          ciphertext[i] = plaintext[i] ^ keystream[i]
     */

    /* Initializing ctx */
    CHACHA_CTX ctx;
    init_chacha_ctx(&ctx, key, nonce, 1);
    /* END initializing ctx */

    /* Generating keystream */
    size_t keystream_len = (plaintext_len + CHACHA_BLOCK_SIZE - 1) / CHACHA_BLOCK_SIZE; 
    uint32_t keystream[keystream_len * CHACHA_BLOCK_SIZE];

    chacha_generate_keystream(&ctx, plaintext_len, keystream);

    uint8_t keystream8[keystream_len * CHACHA_BLOCK_SIZE_UINT8_t];
    for (size_t i = 0; i < keystream_len; i++) {
        serialize(keystream + i*CHACHA_BLOCK_SIZE, keystream8 + i * CHACHA_BLOCK_SIZE_UINT8_t);
    }
    /* END Generating keystream */

    /* Encrypting */
    for (size_t i = 0; i < plaintext_len; i++) {
        ciphertext[i] = keystream8[i] ^ plaintext[i];
    }
    /* END encrypting */
}

void decrypt(uint8_t *ciphertext, size_t ciphertext_len, uint32_t *key, uint32_t *nonce, uint8_t *plaintext) {
    encrypt(ciphertext, ciphertext_len, key, nonce, plaintext);
}
