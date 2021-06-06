#include "slcsp.h"
#include <stdbool.h>

int slcsp_parse (FILE *f, struct slcsp *this)
{
	static bool header_parsed = false;
	if (!this)
		return 1;
	if (!header_parsed) {
		if (eatline(f))
			return 2;
		header_parsed = true;
	}
	fscanf(f, "%[^,],", (char *)&this->code);
	if (eatline(f))
		return 1;
	return 0;
}

int slcsp_write (struct slcsp *this)
{
	struct zip *z;
	struct plan *p;
	struct listnode *plan_node = planlist.head;
	struct listnode *zip_node = ziplist.head;

	location_t l = 0;
	this->lowest = 0;
	this->second = 0;

	while ((z = ziplist_find(this->code, &zip_node))) {
		if (l && l != z->location)
			return 1;
		l = z->location;
	}

	while ((p = planlist_find(l, &plan_node))) {
		if (this->lowest < 10e-6) {
			this->lowest = p->rate;
			continue;
		}
		if (this->second < 10e-6 || p->rate < this->second) {
			this->second = p->rate;
		}
		if (p->rate < this->lowest) {
			this->second = this->lowest;
			this->lowest = p->rate;
		}
	}
	return 0;
}

void slcsp_print (struct slcsp *this)
{
	printf("%s,", this->code);
	if (this->second > 10e-6)
		printf("%.2f", this->second);
	printf("\n");
}
