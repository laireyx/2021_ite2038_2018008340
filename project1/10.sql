SELECT name
FROM
	Pokemon
    LEFT JOIN CatchedPokemon
    	ON Pokemon.id = CatchedPokemon.pid
WHERE
	CatchedPokemon.pid IS NULL
ORDER BY
	name