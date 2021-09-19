SELECT
	SUM(CatchedPokemon.level)
FROM
	CatchedPokemon
    JOIN Pokemon
    	ON Pokemon.id = CatchedPokemon.pid
        AND Pokemon.type = 'Fire'