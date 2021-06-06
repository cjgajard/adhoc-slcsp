#include <stdint.h>
#include <string.h>
#include "slcsp.h"

struct list ziplist = { NULL, NULL, sizeof(struct zip) };
static int zip_parse (FILE *f, struct zip *z);

int zip_init (const char *path)
{
	struct zip z;
	FILE *f;
	f = fopen(path, "r");
	if (!f) {
		perror(path);
		return 1;
	}

	if (eatline(f)) /* eat headers */
		return 2;

	while (!zip_parse(f, &z)) {
		if (list_add(&ziplist, &z)) {
			return 2;
		}
	}

	fclose(f);
	return 0;
}

void zip_close ()
{
	list_destroy(&ziplist);
}

static int zip_parse (FILE *f, struct zip *z)
{
	uint16_t area;
	char *loc = (char*)&z->location;
	fscanf(f, "%[^,],%2s,%*[^,],%*[^,],%hu", z->code, loc, &area);
	memcpy(loc + 2, &area, 2);
	if (eatline(f))
		return 1;
	return 0;
}

struct zip *ziplist_find (zipcode_t code, struct listnode **n)
{
	struct listnode *now = n ? *n : NULL;
	while (now) {
		struct zip *z = now->data;
		if (!z) return NULL;

		if (!strcmp(z->code, code)) {
			if (n) *n = now->next;
			return z;
		}
		now = now->next;
	}
	return 0;
}
