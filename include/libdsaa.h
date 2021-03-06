#ifndef LIBSDAA_H
#define LIBSDAH_H

typedef enum {list_item_first, list_item_last} list_item_position;

struct list_item {
    void* data;
    struct list_item* next;
    struct list_item* previous;
};

struct list_function {
	int (*compare)(void*, void*);
	void (*print)(int, void*);
	void (*release)(void*);
	int (*find)(void*, void*);
	void (*update)(void*, void*, void*);
};

struct list {
	struct list_item* head;
	struct list_item* tail;
	struct list_function* function;
	int size;
};

#ifdef DEBUG
void list_debug(int);
void list_debug_log(const char*, ...);
#endif

void list_init(struct list*, struct list_function*);
int list_add(struct list*, void*);
int list_add_sort(struct list*, void*);
int list_find(struct list*, void*);
int list_move(struct list*, void* i, list_item_position, void*, void*);
int list_remove_find(struct list*, void*);
int list_remove(struct list*, list_item_position);
int list_print(struct list*, list_item_position);
int list_release(struct list*);
int list_get_find(struct list*, void*, void**);
int list_get(struct list*, list_item_position, void**);
int list_update(struct list*, void*, void*, void*);

#endif
