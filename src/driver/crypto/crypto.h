enum crypto_status_t {
    cStatus_Success = 0,
    cStatus_Failed = -1
};

int crypto_sha512(const byte *message, size_t size, byte *hash);

char *crypto_base64_encode(const byte *in, size_t inlen);
