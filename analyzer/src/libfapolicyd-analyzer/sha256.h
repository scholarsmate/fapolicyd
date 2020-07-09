#ifndef SHA256_H
#define SHA256_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <unistd.h>

#define SHA256_DIGEST_SIZE 32 /* SHA256 outputs a 32 byte digest */

typedef struct sha256_ctx_struct sha256_ctx_t;

sha256_ctx_t *sha256_create();
void sha256_destroy(sha256_ctx_t *ctx);

void sha256_update(sha256_ctx_t *ctx, const unsigned char *data, ssize_t len);
void sha256_final(
    sha256_ctx_t *ctx,
    unsigned char *digest); /* digest must be at least 32-bytes in length */

/**
 * @brief Computes a SHA256 hash of the contents of the file pointed to by the
 * given path
 *
 * @param hash_buf character buffer to store the hexidecimal representation of
 * the hash
 * @param hash_buf_len the size of the character buffer
 * @param path path to the file whose contents will be hashed
 * @return int returns zero (OK) on success and non-zero otherwise
 */
int sha256_file(char *hash_buf, size_t hash_buf_len, const char *path);

#ifdef __cplusplus
}
#endif

#endif /* SHA256_H */
