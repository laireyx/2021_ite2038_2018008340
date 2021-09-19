SELECT
	Pokemon.name
FROM
	Pokemon
    JOIN CatchedPokemon
    	ON CatchedPokemon.pid = Pokemon.id
    JOIN Trainer
    	ON Trainer.id = CatchedPokemon.owner_id
        AND Trainer.hometown = 'Sangnok City'
    JOIN CatchedPokemon OtherCatchedPokemon
    	ON OtherCatchedPokemon.pid = Pokemon.id
    JOIN Trainer OtherTrainer
    	ON OtherTrainer.id = OtherCatchedPokemon.owner_id
        AND OtherTrainer.hometown = 'Blue City'
GROUP BY
	Pokemon.name
ORDER BY
	Pokemon.name