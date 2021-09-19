SELECT Trainer.name, CatchedPokemon.nickname
FROM
	Trainer
    JOIN CatchedPokemon
    	ON CatchedPokemon.owner_id = Trainer.id
    JOIN CatchedPokemon SecondPokemon
    	ON SecondPokemon.owner_id = CatchedPokemon.owner_id
        AND SecondPokemon.id != CatchedPokemon.id
    JOIN CatchedPokemon ThirdPokemon
    	ON ThirdPokemon.owner_id = CatchedPokemon.owner_id
        AND ThirdPokemon.id != CatchedPokemon.id
        AND ThirdPokemon.id != SecondPokemon.id
    JOIN CatchedPokemon FourthPokemon
    	ON FourthPokemon.owner_id = CatchedPokemon.owner_id
        AND FourthPokemon.id != CatchedPokemon.id
        AND FourthPokemon.id != SecondPokemon.id
        AND FourthPokemon.id != ThirdPokemon.id
    LEFT JOIN CatchedPokemon HigherLevelPokemon
    	ON HigherLevelPokemon.owner_id = CatchedPokemon.owner_id
        AND HigherLevelPokemon.level > CatchedPokemon.level
WHERE
	HigherLevelPokemon.id IS NULL
GROUP BY
	Trainer.name, CatchedPokemon.nickname
ORDER BY
    CatchedPokemon.nickname