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
	void (*update)(void*, void*);
};

struct list {
	struct list_item* head;
	struct list_item* tail;
	struct list_function* function;
	int size;
};

#endif
