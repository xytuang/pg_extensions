CREATE FUNCTION complex_add(integer[]) RETURNS integer
	LANGUAGE SQL
	IMMUTABLE
	RETURNS NULL ON NULL INPUT
	AS $$
		SELECT COALESCE(SUM(val), 0) -- COALESCE returns first non-null value. ie. if SUM(VAL) is null because the array is empty, return 0
		FROM unnest($1) as t(val) -- unnest turns the array into a set of rows. as t(val) turns that set into a table called t with a column called val
	$$
;
