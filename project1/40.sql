SELECT
	Trainer.name
FROM
	Trainer
    JOIN Pokemon
    	ON Pokemon.name = 'Pikachu'
	JOIN CatchedPokemon
    	ON CatchedPokemon.pid = Pokemon.id
        AND	Trainer.id = CatchedPokemon.owner_id
ORDER BY
	Trainer.name;