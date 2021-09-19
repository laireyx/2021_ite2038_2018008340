SELECT
	Trainer.name, SUM(CatchedPokemon.level)
FROM
    Trainer
    JOIN CatchedPokemon
		ON CatchedPokemon.owner_id = Trainer.id
GROUP BY
	Trainer.name
HAVING
	SUM(CatchedPokemon.level) = (
      SELECT MAX(total) FROM (
      	SELECT SUM(OtherCatchedPokemon.level) AS total
    		FROM Trainer OtherTrainer
        	JOIN CatchedPokemon OtherCatchedPokemon
        		ON OtherCatchedPokemon.owner_id = OtherTrainer.id
        	GROUP BY OtherTrainer.name
		) CatchedTable
    )