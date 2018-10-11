#include <unabto/unabto_hmac_sha256.h>

#include <unabto/unabto_aes_cbc.h>

#include <crypto/crypto.h>
#include <string.h>

bool unabto_aes128_cbc_encrypt(const uint8_t* key, uint8_t* input, uint16_t inputLength)
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

bool unabto_aes128_cbc_decrypt(const uint8_t* key, uint8_t* input, uint16_t inputLength)
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

#define BUFFER_SIZE 2048

static uint8_t buffer[BUFFER_SIZE];
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
        if ((ptr - buffer) + keys[i].size > BUFFER_SIZE) {
            return;
        }
        memcpy(ptr, keys[i].data, keys[i].size); ptr += keys[i].size;
    }
    
    int status = CRYPT_HMAC_SetKey(&ctx, CRYPT_HMAC_SHA256, buffer, ptr - buffer);
    if (status != 0) {
        NABTO_LOG_ERROR(("could not set hmac key %i", status));
        return;
    }

 
    for (i = 0; i < messagesSize; i++) {
        uint16_t size = messages[i].size;
        uint8_t* data = messages[i].data;
        if (size > BUFFER_SIZE) {
            return;
        }
        memcpy(buffer, data, size);
        status = CRYPT_HMAC_DataAdd(&ctx, buffer, size);
        if (status != 0) {
            return;
        }
        // workaround for bug, if the data is not copied over to the buffer the algorithm fails.
    }
    
    uint8_t hmac[CRYPT_SHA256_DIGEST_SIZE];
    status = CRYPT_HMAC_Finalize(&ctx, hmac);
    if (status != 0) {
        NABTO_LOG_ERROR(("could not finalize hmac %i", status));
        return;
    }

    memcpy(mac, hmac, macSize);
}
