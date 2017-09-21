#ifndef SHA1_T
#define SHA1_T

#define SHA1_HASH_LENGTH 20
#define SHA1_BLOCK_LENGTH 64

union sha1_buffer_t {
	unsigned char b[SHA1_BLOCK_LENGTH];
	unsigned int w[SHA1_BLOCK_LENGTH / 4];
};

union sha1_state_t {
	unsigned char b[SHA1_HASH_LENGTH];
	unsigned int w[SHA1_HASH_LENGTH / 4];
};

typedef struct sha1_t {
	union sha1_buffer_t buffer;
	unsigned char bufferOffset;
	union sha1_state_t state;
	unsigned int byteCount;
	unsigned char keyBuffer[SHA1_BLOCK_LENGTH];
	unsigned char innerHash[SHA1_HASH_LENGTH];
} sha1_t;

void sha1_new(sha1_t *s);

void sha1_writebyte(sha1_t *s, unsigned char data);

void sha1_write(sha1_t *s, const char *data, size_t len);

unsigned char* sha1_result(sha1_t *s);

#endif
