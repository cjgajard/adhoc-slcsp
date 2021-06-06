#include "slcsp.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

struct list planlist = { NULL, NULL, sizeof(struct plan) };
static int plan_parse (FILE *f, struct plan *p);

int plan_init (const char *path)
{
	FILE *f; /* file */
	int err = 0;
	struct plan p = {0}; /* plan */
	if ((f = fopen(path, "r")) == NULL) {
		perror(path);
		return 1;
	}

	if (eatline(f)) /* eat headers */
		return 2;

	while (!plan_parse(f, &p)) {
		if (p.level != Silver) {
			continue;
		}
		if (list_add(&planlist, &p)) {
			err = 2;
			goto close;
		}
	}
close:
	fclose(f);
	return err;
}

void plan_close ()
{
	list_destroy(&planlist);
}

struct plan *planlist_find (location_t l, struct listnode **n)
{
	struct listnode *now = n ? *n : NULL;
	while (now) {
		struct plan *p = now->data;
		if (!p) return NULL;

		if (p->location == l) {
			if (n) *n = now->next;
			return now->data;
		}
		now = now->next;
	}
	return NULL;
}

static enum level level_parse (const char *name)
{
	if (!strncmp(name, "Bronze", 6))
		return Bronze;
	if (!strncmp(name, "Silver", 6))
		return Silver;
	if (!strncmp(name, "Gold", 4))
		return Gold;
	if (!strncmp(name, "Platinum", 8))
		return Platinum;
	if (!strncmp(name, "Catastrophic", 8))
		return Catastrophic;
	return None;
}

static int plan_parse (FILE *f, struct plan *p)
{
	uint16_t area;
	char metal_str[16];
	char *loc = (char*)&p->location;
	if (!p)
		return 1;
	fscanf(f, "%*[^,],%2s,%[^,],%lf,%hu", loc, metal_str, &p->rate, &area);
	memcpy(loc + 2, &area, 2);
	p->level = level_parse(metal_str);
	if (eatline(f))
		return 2;
	return 0;
}

void plan_print (struct plan *this)
{
	printf("%.2f %06x %d\n", this->rate, this->location, this->level);
}
