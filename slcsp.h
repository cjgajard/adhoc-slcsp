#ifndef SLCSP_h
#define SLCSP_h 1
#include <stdio.h>

typedef char zipcode_t[16];
typedef int location_t;

struct slcsp {
	double lowest;
	double second;
	zipcode_t code;
};

enum level {
	None,
	Bronze,
	Silver,
	Gold,
	Platinum,
	Catastrophic
};

struct plan {
	double rate;
	location_t location;
	enum level level;
};

struct zip {
	zipcode_t code;
	location_t location;
};

struct listnode {
	void *data;
	struct listnode *next;
};

struct list {
	struct listnode *head;
	struct listnode *tail;
	size_t size;
};

extern struct list ziplist;
extern struct list planlist;

int eatline (FILE *f);

int plan_init (const char *path);
void plan_close (void);
void plan_print (struct plan *this);
struct plan *planlist_find (location_t l, struct listnode **n);

int zip_init (const char *path);
void zip_close (void);
struct zip *ziplist_find (zipcode_t z, struct listnode **n);

void slcsp_print ();
int slcsp_parse (FILE *f, struct slcsp *this);
int slcsp_write (struct slcsp *this);

int list_add (struct list *this, void *v);
void list_destroy (struct list *this);
int list_len (struct list *this);
#endif
