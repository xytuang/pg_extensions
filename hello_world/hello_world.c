#include "postgres.h"
#include "fmgr.h"
#include "string.h"
#include "utils/array.h"
#include "utils/builtins.h"
#include "utils/lsyscache.h"

PG_MODULE_MAGIC;

PG_FUNCTION_INFO_V1(hello_world);
PG_FUNCTION_INFO_V1(hello_args);

Datum hello_world(PG_FUNCTION_ARGS)
{
	char *result = "Hello World!";
	text *result_text = (text *) cstring_to_text(result);

	PG_RETURN_TEXT_P(result_text);
}
Datum
hello_args(PG_FUNCTION_ARGS)
{
    ArrayType *arr;
    Datum *elements;
    bool *nulls;
    int nelems;

    arr = PG_GETARG_ARRAYTYPE_P(0);

    if (ARR_NDIM(arr) != 1 || ARR_HASNULL(arr))
        ereport(ERROR, (errmsg("Invalid array argument!")));

    Oid typid = ARR_ELEMTYPE(arr);

    int16 typlen;
    bool typbyval;
    char typalign;

    get_typlenbyvalalign(typid, &typlen, &typbyval, &typalign);
    deconstruct_array(arr, typid, typlen, typbyval, typalign,
                      &elements, &nulls, &nelems);

    // Calculate total size needed
    int size = 0;
    text *hello_txt = cstring_to_text("You said: ");
    int start_sz = VARSIZE(hello_txt) - VARHDRSZ;
    size += start_sz;

    for (int i = 0; i < nelems; i++)
    {
        if (!nulls[i])
        {
            text *txt = DatumGetTextP(elements[i]);
            size += VARSIZE(txt) - VARHDRSZ;

            if (i < nelems - 1)
                size += 1;  // for space separator
        }
    }

    size += VARHDRSZ;
    text *out = palloc(size);
    SET_VARSIZE(out, size);

    // Copy prefix "You said: "
    memcpy(VARDATA(out), VARDATA(hello_txt), start_sz);

    int offset = start_sz;

    // Copy elements with spaces
    for (int i = 0; i < nelems; i++)
    {
        if (!nulls[i])
        {
            text *txt = DatumGetTextP(elements[i]);
            int txt_size = VARSIZE(txt) - VARHDRSZ;

            memcpy(VARDATA(out) + offset, VARDATA(txt), txt_size);
            offset += txt_size;

            if (i < nelems - 1)
            {
                char space = ' ';
                memcpy(VARDATA(out) + offset, &space, 1);
                offset += 1;
            }
        }
    }

    PG_RETURN_TEXT_P(out);
}
