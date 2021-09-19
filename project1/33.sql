SELECT
	Trainer.name
FROM
	Trainer
    JOIN Pokemon
    	ON Pokemon.type = 'Psychic'
    JOIN CatchedPokemon
    	ON CatchedPokemon.owner_id = Trainer.id
    	AND CatchedPokemon.pid = Pokemon.id
GROUP BY
	Trainer.name
ORDER BY
	Trainer.name