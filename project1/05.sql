SELECT
	AVG(level)
FROM
	Trainer, CatchedPokemon
WHERE
	Trainer.name = 'Red' AND
    Trainer.id = CatchedPokemon.owner_id