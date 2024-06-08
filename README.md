# ChaCha

C implementation of ChaCha cipher

```C
void encrypt(uint8_t *plaintext, size_t plaintext_len, uint32_t *key, uint32_t *nonce, uint8_t *ciphertext);
void decrypt(uint8_t *ciphertext, size_t ciphertext_len, uint32_t *key, uint32_t *nonce, uint8_t *plaintext);
```

## References

* <https://datatracker.ietf.org/doc/html/rfc8439>
* <https://en.wikipedia.org/wiki/Salsa20#ChaCha_variant>
* <https://github.com/Ginurx/chacha20-c/blob/master/chacha20.c>
