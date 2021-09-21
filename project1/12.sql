SELECT Pokemon.name
FROM
	Pokemon
    JOIN Evolution
    	ON Pokemon.id = Evolution.before_id
    JOIN Pokemon EvolvedPokemon
    	ON EvolvedPokemon.id = Evolution.after_id
        AND Pokemon.id > EvolvedPokemon.id
ORDER BY
	Pokemon.name;