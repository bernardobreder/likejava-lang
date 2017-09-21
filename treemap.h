#ifndef BREDER_TREEMAP_H
#define BREDER_TREEMAP_H

/**
 * Define the type of treemap
 */
//typedef struct b_treemap_t b_treemap_t;

/**
 * Define the type of treemap
 */
//typedef struct b_treemap_entry_t b_treemap_entry_t;

/**
 * Define the type of error
 */
typedef unsigned char b_treemap_error_t;

struct b_treemap_entry_t {
	void* key;
	void* value;
	struct b_treemap_entry_t* left;
	struct b_treemap_entry_t* right;
	struct b_treemap_entry_t* parent;
	unsigned char color;
};

struct b_treemap_t {
	int (*compareFunc)(void*, void*);
	void (*freeKeyFunc)(void*);
	void (*freeValueFunc)(void*);
	struct b_treemap_entry_t* root;
	int size;
};

/**
 * Create a new treemap
 */
struct b_treemap_t* b_treemap_new(int (*compareFunc)(void*, void*), void (*freeKeyFunc)(void*), void (*freeValueFunc)(void*));

/**
 * Free the treemap struct
 */
void b_treemap_free(struct b_treemap_t* data);

/**
 * Create a new treemap with int key
 */
struct b_treemap_t* b_treemap_new_int(void (*freeValueFunc)(void*));

/**
 * Create a new treemap with char* key
 */
struct b_treemap_t* b_treemap_new_vchar(unsigned char freeKey, void (*freeValueFunc)(void*));

/**
 * Add a new element.
 */
b_treemap_error_t b_treemap_put(struct b_treemap_t* data, void* key, void* value);

/**
 * Add a new element.
 * The key is a int.
 */
b_treemap_error_t b_treemap_put_int(struct b_treemap_t* data, int key, void* value);

/**
 * Return the first entry
 */
struct b_treemap_entry_t* b_treemap_first(struct b_treemap_t* data);

/**
 * Return a element of treemap
 */
void* b_treemap_get(struct b_treemap_t* data, void* key);

/**
 * Return the next entry or null if not found
 */
struct b_treemap_entry_t* b_treemap_successor(struct b_treemap_entry_t* t);

/**
 * Return a element of treemap
 */
void* b_treemap_get_int(struct b_treemap_t* data, int key);

/**
 * Return the size of treemap
 */
int b_treemap_size(struct b_treemap_t* data);

/**
 * Remove a element of treemap.
 */
void b_treemap_remove(struct b_treemap_t* data, void* key);

/**
 * Remove a element of treemap.
 */
void b_treemap_remove_int(struct b_treemap_t* data, int key);

/**
 * Remove and Return a element of treemap
 */
void b_treemap_clear(struct b_treemap_t* data);

#endif
