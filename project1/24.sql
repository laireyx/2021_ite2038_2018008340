SELECT
	name
FROM
	Pokemon
WHERE
	name REGEXP '^[AEIOUaeiou]'
ORDER BY
	name