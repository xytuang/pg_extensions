CREATE TABLE BASIC_TABLE (
	id SERIAL PRIMARY KEY,
	name VARCHAR(100) NOT NULL
);

CREATE FUNCTION ADD(a integer, b integer) RETURNS integer
	LANGUAGE SQL
	IMMUTABLE
	RETURNS NULL ON NULL INPUT
	RETURN a + b;
