#include <unabto/unabto_hmac_sha256.h>

#include <unabto/unabto_aes_cbc.h>

#include <crypto/crypto.h>

bool aes128_cbc_encrypt(const uint8_t* key, uint8_t* input, uint16_t inputLength)
{
    CRYPT_AES_CTX ctx;
    int status = CRYPT_AES_KeySet(&ctx, key, 16, input, CRYPT_AES_ENCRYPTION);
    if (status != 0) {
        NABTO_LOG_ERROR(("encrytion key set failed status %i", status));
        return false;
    }
    status = CRYPT_AES_CBC_Encrypt(&ctx, input+16, input+16, inputLength - 16);
    if (status != 0) {
        NABTO_LOG_ERROR(("aes encryption failed %i", status));
        return false;
    }
    return true;
}

bool aes128_cbc_decrypt(const uint8_t* key, uint8_t* input, uint16_t inputLength)
{
    CRYPT_AES_CTX ctx;
    int status = CRYPT_AES_KeySet(&ctx, key, 16, input, CRYPT_AES_DECRYPTION);
    if (status != 0) {
        NABTO_LOG_ERROR(("encrytion key set failed status %i", status));
        return false;
    }

    status = CRYPT_AES_CBC_Decrypt(&ctx, input+16, input+16, inputLength - 16);
    if (status != 0) {
        NABTO_LOG_ERROR(("aes decryption failed %i", status));
        return false;
    }
    return true;
}

static uint8_t buffer[2048];
void unabto_hmac_sha256_buffers(const unabto_buffer keys[], uint8_t keysSize,
                                const unabto_buffer messages[], uint8_t messagesSize,
                                uint8_t* mac, uint16_t macSize)
{
    CRYPT_HMAC_CTX ctx;
    uint8_t* ptr;
    uint8_t i;
    
    // merge key bits together
    ptr = buffer;
    for (i = 0; i < keysSize; i++) {
        memcpy(ptr, keys[i].data, keys[i].size); ptr += keys[i].size;
    }
    
    int status = CRYPT_HMAC_SetKey(&ctx, CRYPT_HMAC_SHA256, buffer, ptr - buffer);
    if (status != 0) {
        NABTO_LOG_ERROR(("could not set hmac key %i", status));
        return;
    }

    // workaround for bug
    ptr = buffer;
    int dataSize = 0;
    for (i = 0; i < messagesSize; i++) {
        uint8_t size = messages[i].size;
        uint8_t* data = messages[i].data;
        memcpy(ptr, data, size); ptr += size;
        dataSize += size;
    }
    status = 42;
    status = CRYPT_HMAC_DataAdd(&ctx, buffer, dataSize);
        
    if (status != 0) {
        NABTO_LOG_ERROR(("could not add data to hmac %i", status));
        return;
    }
    
    uint8_t hmac[CRYPT_SHA256_DIGEST_SIZE];
    status = CRYPT_HMAC_Finalize(&ctx, hmac);
    if (status != 0) {
        NABTO_LOG_ERROR(("could not finalize hmac %i", status));
        return;
    }

    memcpy(mac, hmac, macSize);
}
