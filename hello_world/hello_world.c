#include "postgres.h"
#include "fmgr.h"
#include "utils/builtins.h"

PG_MODULE_MAGIC;

PG_FUNCTION_INFO_V1(hello_world);

Datum hello_world(PG_FUNCTION_ARGS)
{
	char *result = "Hello World!";
	text *result_text = (text *) cstring_to_text(result);

	PG_RETURN_TEXT_P(result_text);
}
