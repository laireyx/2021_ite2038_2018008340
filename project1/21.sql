SELECT
	Trainer.name
FROM
	Trainer
    JOIN CatchedPokemon
    	ON CatchedPokemon.owner_id = Trainer.id
    JOIN CatchedPokemon OtherCatchedPokemon
    	ON OtherCatchedPokemon.id != CatchedPokemon.id
        AND OtherCatchedPokemon.pid = CatchedPokemon.pid
        AND OtherCatchedPokemon.owner_id = Trainer.id
GROUP BY
	Trainer.name
ORDER BY
	Trainer.name