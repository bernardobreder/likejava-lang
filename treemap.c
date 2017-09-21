#include <stdlib.h>
#include <string.h>
#include "treemap.h"

#define b_treemap_entry_red  0
#define b_treemap_entry_black 1
#define b_treemap_entry_set_color(p,c) if (p) { p->color = c; }
#define b_treemap_entry_color(p) (!p ? b_treemap_entry_black: p->color)
#define b_treemap_entry_right(p) (!p ? 0: p->right)
#define b_treemap_entry_left(p) (!p ? 0: p->left)
#define b_treemap_entry_parent(p) (!p ? 0: p->parent)

struct b_treemap_t* b_treemap_new(int (*compareFunc)(void*, void*), void (*freeKeyFunc)(void*), void (*freeValueFunc)(void*)) {
	struct b_treemap_t* data = (struct b_treemap_t*) calloc(1, sizeof(struct b_treemap_t));
	if (!data) {
		return 0;
	}
	data->compareFunc = compareFunc;
	data->freeKeyFunc = freeKeyFunc;
	data->freeValueFunc = freeValueFunc;
	return data;
}

void b_treemap_free(struct b_treemap_t* data) {
	b_treemap_clear(data);
	free(data);
}

static int b_treemap_int_compare(void* o1, void* o2) {
	return (int) (o1 - o2);
}

struct b_treemap_t* b_treemap_new_int(void (*freeValueFunc)(void*)) {
	return b_treemap_new(b_treemap_int_compare, 0, freeValueFunc);
}

static int b_treemap_vchar_compare(void* o1, void* o2) {
	return strcmp(o1, o2);
}

struct b_treemap_t* b_treemap_new_vchar(unsigned char freeKey, void (*freeValueFunc)(void*)) {
	return b_treemap_new(b_treemap_vchar_compare, freeKey ? free : 0, freeValueFunc);
}

static void b_treemap_rotate_left(struct b_treemap_t* data, struct b_treemap_entry_t* p) {
	if (p) {
		struct b_treemap_entry_t* r = p->right;
		p->right = r->left;
		if (r->left) {
			r->left->parent = p;
		}
		r->parent = p->parent;
		if (!p->parent) {
			data->root = r;
		} else if (p->parent->left == p) {
			p->parent->left = r;
		} else {
			p->parent->right = r;
		}
		r->left = p;
		p->parent = r;
	}
}

static void b_treemap_rotate_right(struct b_treemap_t* data, struct b_treemap_entry_t* p) {
	if (p) {
		struct b_treemap_entry_t* l = p->left;
		p->left = l->right;
		if (l->right) {
			l->right->parent = p;
		}
		l->parent = p->parent;
		if (!p->parent) {
			data->root = l;
		} else if (p->parent->right == p) {
			p->parent->right = l;
		} else {
			p->parent->left = l;
		}
		l->right = p;
		p->parent = l;
	}
}

static void b_treemap_fix_after_insertion(struct b_treemap_t* data, struct b_treemap_entry_t* x) {
	x->color = b_treemap_entry_red;
	while (x && x != data->root && x->parent->color == b_treemap_entry_red) {
		if (b_treemap_entry_parent(x) == b_treemap_entry_left(b_treemap_entry_parent(b_treemap_entry_parent(x)))) {
			struct b_treemap_entry_t* y = b_treemap_entry_right(b_treemap_entry_parent(b_treemap_entry_parent(x)));
			if (b_treemap_entry_color(y) == b_treemap_entry_red) {
				b_treemap_entry_set_color(b_treemap_entry_parent(x), b_treemap_entry_black);
				b_treemap_entry_set_color(y, b_treemap_entry_black);
				b_treemap_entry_set_color(b_treemap_entry_parent(b_treemap_entry_parent(x)), b_treemap_entry_red);
				x = b_treemap_entry_parent(b_treemap_entry_parent(x));
			} else {
				if (x == b_treemap_entry_right(b_treemap_entry_parent(x))) {
					x = b_treemap_entry_parent(x);
					b_treemap_rotate_left(data, x);
				}
				b_treemap_entry_set_color(b_treemap_entry_parent(x), b_treemap_entry_black);
				b_treemap_entry_set_color(b_treemap_entry_parent(b_treemap_entry_parent(x)), b_treemap_entry_red);
				b_treemap_rotate_right(data, b_treemap_entry_parent(b_treemap_entry_parent(x)));
			}
		} else {
			struct b_treemap_entry_t* y = b_treemap_entry_left(b_treemap_entry_parent(b_treemap_entry_parent(x)));
			if (b_treemap_entry_color(y) == b_treemap_entry_red) {
				b_treemap_entry_set_color(b_treemap_entry_parent(x), b_treemap_entry_black);
				b_treemap_entry_set_color(y, b_treemap_entry_black);
				b_treemap_entry_set_color(b_treemap_entry_parent(b_treemap_entry_parent(x)), b_treemap_entry_red);
				x = b_treemap_entry_parent(b_treemap_entry_parent(x));
			} else {
				if (x == b_treemap_entry_left(b_treemap_entry_parent(x))) {
					x = b_treemap_entry_parent(x);
					b_treemap_rotate_right(data, x);
				}
				b_treemap_entry_set_color(b_treemap_entry_parent(x), b_treemap_entry_black);
				b_treemap_entry_set_color(b_treemap_entry_parent(b_treemap_entry_parent(x)), b_treemap_entry_red);
				b_treemap_rotate_left(data, b_treemap_entry_parent(b_treemap_entry_parent(x)));
			}
		}
	}
	data->root->color = b_treemap_entry_black;
}

b_treemap_error_t b_treemap_put(struct b_treemap_t* data, void* key, void* value) {
	struct b_treemap_entry_t* aux = data->root;
	if (!aux) {
		struct b_treemap_entry_t* entry = (struct b_treemap_entry_t*) calloc(1, sizeof(struct b_treemap_entry_t));
		if (!entry) {
			return 1;
		}
		entry->key = key;
		entry->value = value;
		data->root = entry;
		data->size = 1;
		return 0;
	}
	int cmp;
	struct b_treemap_entry_t* parent;
	do {
		parent = aux;
		cmp = data->compareFunc(key, aux->key);
		if (cmp < 0) {
			aux = aux->left;
		} else if (cmp > 0) {
			aux = aux->right;
		} else {
			void* result = aux->value;
			aux->value = value;
			if (data->freeValueFunc) {
				data->freeValueFunc(result);
			}
			return 0;
		}
	} while (aux);
	struct b_treemap_entry_t* e = (struct b_treemap_entry_t*) calloc(1, sizeof(struct b_treemap_entry_t));
	if (!e) {
		return 1;
	}
	e->key = key;
	e->value = value;
	e->parent = parent;
	if (cmp < 0) {
		parent->left = e;
	} else {
		parent->right = e;
	}
	b_treemap_fix_after_insertion(data, e);
	data->size++;
	return 0;
}

b_treemap_error_t b_treemap_put_int(struct b_treemap_t* data, int key, void* value) {
	void* p = 0;
	p += key;
	return b_treemap_put(data, p, value);
}

struct b_treemap_entry_t* b_treemap_first(struct b_treemap_t* data) {
	struct b_treemap_entry_t* p = data->root;
	if (p) {
		while (p->left) {
			p = p->left;
		}
	}
	return p;
}

static struct b_treemap_entry_t* b_treemap_get_entry(struct b_treemap_t* data, void* key) {
	struct b_treemap_entry_t* p = data->root;
	while (p) {
		int cmp = data->compareFunc(key, p->key);
		if (cmp < 0) {
			p = p->left;
		} else if (cmp > 0) {
			p = p->right;
		} else {
			return p;
		}
	}
	return 0;
}

void* b_treemap_get(struct b_treemap_t* data, void* key) {
	struct b_treemap_entry_t* p = b_treemap_get_entry(data, key);
	return !p ? 0 : p->value;
}

void* b_treemap_get_int(struct b_treemap_t* data, int key) {
	void* p = 0;
	p += key;
	return b_treemap_get(data, p);
}

int b_treemap_size(struct b_treemap_t* data) {
	return data->size;
}

struct b_treemap_entry_t* b_treemap_successor(struct b_treemap_entry_t* t) {
	if (!t) {
		return 0;
	} else if (t->right) {
		struct b_treemap_entry_t* p = t->right;
		while (p->left) {
			p = p->left;
		}
		return p;
	} else {
		struct b_treemap_entry_t* p = t->parent;
		struct b_treemap_entry_t* ch = t;
		while (p && ch == p->right) {
			ch = p;
			p = p->parent;
		}
		return p;
	}
}

void b_treemap_fix_after_deletion(struct b_treemap_t* data, struct b_treemap_entry_t* x) {
	while (x != data->root && b_treemap_entry_color(x) == b_treemap_entry_black) {
		if (x == b_treemap_entry_left(b_treemap_entry_parent(x))) {
			struct b_treemap_entry_t* sib = b_treemap_entry_right(b_treemap_entry_parent(x));
			if (b_treemap_entry_color(sib) == b_treemap_entry_red) {
				b_treemap_entry_set_color(sib, b_treemap_entry_black);
				b_treemap_entry_set_color(b_treemap_entry_parent(x), b_treemap_entry_red);
				b_treemap_rotate_left(data, b_treemap_entry_parent(x));
				sib = b_treemap_entry_right(b_treemap_entry_parent(x));
			}
			if (b_treemap_entry_color(b_treemap_entry_left(sib)) == b_treemap_entry_black && b_treemap_entry_color(b_treemap_entry_right(sib)) == b_treemap_entry_black) {
				b_treemap_entry_set_color(sib, b_treemap_entry_red);
				x = b_treemap_entry_parent(x);
			} else {
				if (b_treemap_entry_color(b_treemap_entry_right(sib)) == b_treemap_entry_black) {
					b_treemap_entry_set_color(b_treemap_entry_left(sib), b_treemap_entry_black);
					b_treemap_entry_set_color(sib, b_treemap_entry_red);
					b_treemap_rotate_right(data, sib);
					sib = b_treemap_entry_right(b_treemap_entry_parent(x));
				}
				b_treemap_entry_set_color(sib, b_treemap_entry_color(b_treemap_entry_parent(x)));
				b_treemap_entry_set_color(b_treemap_entry_parent(x), b_treemap_entry_black);
				b_treemap_entry_set_color(b_treemap_entry_right(sib), b_treemap_entry_black);
				b_treemap_rotate_left(data, b_treemap_entry_parent(x));
				x = data->root;
			}
		} else { // symmetric
			struct b_treemap_entry_t* sib = b_treemap_entry_left(b_treemap_entry_parent(x));
			if (b_treemap_entry_color(sib) == b_treemap_entry_red) {
				b_treemap_entry_set_color(sib, b_treemap_entry_black);
				b_treemap_entry_set_color(b_treemap_entry_parent(x), b_treemap_entry_red);
				b_treemap_rotate_right(data, b_treemap_entry_parent(x));
				sib = b_treemap_entry_left(b_treemap_entry_parent(x));
			}
			if (b_treemap_entry_color(b_treemap_entry_right(sib)) == b_treemap_entry_black && b_treemap_entry_color(b_treemap_entry_left(sib)) == b_treemap_entry_black) {
				b_treemap_entry_set_color(sib, b_treemap_entry_red);
				x = b_treemap_entry_parent(x);
			} else {
				if (b_treemap_entry_color(b_treemap_entry_left(sib)) == b_treemap_entry_black) {
					b_treemap_entry_set_color(b_treemap_entry_right(sib), b_treemap_entry_black);
					b_treemap_entry_set_color(sib, b_treemap_entry_red);
					b_treemap_rotate_left(data, sib);
					sib = b_treemap_entry_left(b_treemap_entry_parent(x));
				}
				b_treemap_entry_set_color(sib, b_treemap_entry_color(b_treemap_entry_parent(x)));
				b_treemap_entry_set_color(b_treemap_entry_parent(x), b_treemap_entry_black);
				b_treemap_entry_set_color(b_treemap_entry_left(sib), b_treemap_entry_black);
				b_treemap_rotate_right(data, b_treemap_entry_parent(x));
				x = data->root;
			}
		}
	}

	b_treemap_entry_set_color(x, b_treemap_entry_black);
}

static void b_treemap_delete_entry(struct b_treemap_t* data, struct b_treemap_entry_t* p) {
	data->size--;
	if (p->left && p->right) {
		struct b_treemap_entry_t* s = b_treemap_successor(p);
		p->key = s->key;
		p->value = s->value;
		p = s;
	}
	struct b_treemap_entry_t* replacement = (p->left ? p->left : p->right);
	if (replacement) {
		replacement->parent = p->parent;
		if (!p->parent) {
			data->root = replacement;
		} else if (p == p->parent->left) {
			p->parent->left = replacement;
		} else {
			p->parent->right = replacement;
		}
		p->left = p->right = p->parent = 0;
		if (p->color == b_treemap_entry_black) {
			b_treemap_fix_after_insertion(data, replacement);
		}
	} else if (!p->parent) {
		data->root = 0;
	} else {
		if (p->color == b_treemap_entry_black) {
			b_treemap_fix_after_insertion(data, p);
		}
		if (p->parent) {
			if (p == p->parent->left) {
				p->parent->left = 0;
			} else if (p == p->parent->right) {
				p->parent->right = 0;
			}
			p->parent = 0;
		}
	}
}

void b_treemap_remove(struct b_treemap_t* data, void* key) {
	struct b_treemap_entry_t* p = b_treemap_get_entry(data, key);
	if (p) {
		b_treemap_delete_entry(data, p);
		if (data->freeKeyFunc) {
			data->freeKeyFunc(p->key);
		}
		if (data->freeValueFunc) {
			data->freeValueFunc(p->value);
		}
		free(p);
	}
}

void b_treemap_remove_int(struct b_treemap_t* data, int key) {
	void* p = 0;
	p += key;
	b_treemap_remove(data, p);
}

static void b_treemap_remove_entry(struct b_treemap_t* data, struct b_treemap_entry_t* e) {
	if (data->freeKeyFunc) {
		data->freeKeyFunc(e->key);
	}
	if (data->freeValueFunc) {
		data->freeValueFunc(e->value);
	}
	if (e->left) {
		b_treemap_remove_entry(data, e->left);
	}
	if (e->right) {
		b_treemap_remove_entry(data, e->right);
	}
	free(e);
}

void b_treemap_clear(struct b_treemap_t* data) {
	if (data->root) {
		b_treemap_remove_entry(data, data->root);
	}
	data->root = 0;
	data->size = 0;
}
