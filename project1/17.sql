SELECT
	AVG(CatchedPokemon.level)
FROM
	CatchedPokemon
    JOIN Pokemon
    	ON CatchedPokemon.pid = Pokemon.id
        AND Pokemon.type = 'Water'