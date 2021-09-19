SELECT
	Trainer.name, COUNT(CatchedPokemon.id)
FROM
	Trainer
    JOIN CatchedPokemon
    	ON CatchedPokemon.owner_id = Trainer.id
GROUP BY
	Trainer.name
ORDER BY
	Trainer.name