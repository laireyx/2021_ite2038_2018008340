SELECT
	Trainer.name
FROM
	Trainer
    JOIN Pokemon
    JOIN CatchedPokemon
    	ON CatchedPokemon.owner_id = Trainer.id
    	AND CatchedPokemon.pid = Pokemon.id
    LEFT JOIN Pokemon OtherPokemon
    	ON Pokemon.type != OtherPokemon.type
    LEFT JOIN CatchedPokemon OtherCatchedPokemon
    	ON OtherCatchedPokemon.owner_id = Trainer.id
        AND OtherCatchedPokemon.pid = OtherPokemon.id
        AND OtherCatchedPokemon.id != CatchedPokemon.id
GROUP BY
	Trainer.name
HAVING
	SUM(OtherCatchedPokemon.level) IS NULL
ORDER BY
	Trainer.name;