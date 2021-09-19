SELECT
	Trainer.name
FROM
	Trainer
    JOIN CatchedPokemon
    	ON CatchedPokemon.owner_id = Trainer.id
	JOIN Pokemon
    	ON CatchedPokemon.pid = Pokemon.id
    	AND Pokemon.name LIKE 'P%'
WHERE
	Trainer.hometown = 'Sangnok City'
GROUP BY
	Trainer.name
ORDER BY
	Trainer.name