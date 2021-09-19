SELECT
	SUM(CatchedPokemon.level)
FROM
	CatchedPokemon
    JOIN Trainer
    	ON CatchedPokemon.owner_id = Trainer.id
        AND Trainer.name = 'Matis'