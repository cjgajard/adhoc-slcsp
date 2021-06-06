#include "slcsp.h"

int main ()
{
	struct slcsp s; /* second lowest plan */
	FILE *f; /* file */
	int err;

	if ((err = plan_init("plans.csv")))
		return 1;
	if ((err = zip_init("zips.csv"))) {
		err = 2;
		goto plan_end;
	}

	if ((f = fopen("slcsp.csv", "r")) == NULL) {
		perror("fopen slcsp.csv");
		err = 2;
		goto zip_end;
	}

	printf("zipcode,rate\n");
	while (!slcsp_parse(f, &s)) {
		slcsp_write(&s);
		slcsp_print(&s);
	}
	fclose(f);
zip_end:
	zip_close();
plan_end:
	plan_close();
	return err;
}

int eatline (FILE *f)
{
	int byte;
	while ((byte = fgetc(f))) {
		if (byte == '\n')
			return 0;
		if (byte == EOF)
			return 1;
	}
	return 2;
}
