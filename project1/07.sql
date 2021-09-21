SELECT
	City.name, CatchedPokemon.nickname
FROM
	City
    JOIN Trainer
    	ON Trainer.hometown = City.name
    JOIN CatchedPokemon
    	ON CatchedPokemon.owner_id = Trainer.id
	--Other trainer in hometown
    JOIN Trainer OtherTrainer
    	ON OtherTrainer.hometown = Trainer.hometown
	--And join that trainer's pokemon whose level is higher than this pokemon
    LEFT JOIN CatchedPokemon OtherCatchedPokemon
        ON OtherCatchedPokemon.level > CatchedPokemon.level
        AND OtherCatchedPokemon.owner_id = OtherTrainer.id
GROUP BY
	City.name, CatchedPokemon.id, CatchedPokemon.nickname
--If sum of level of higher pokemon IS NULL, it means higher level pokemon is not exists.
--So this pokemon is the highest level in the town.
HAVING
	SUM(OtherCatchedPokemon.level) IS NULL
ORDER BY
	City.name;