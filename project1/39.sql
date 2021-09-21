SELECT
	Pokemon.name
FROM
	Pokemon
    JOIN Gym
    	ON Gym.city = 'Rainbow City'
    JOIN Trainer
    	ON Trainer.id = Gym.leader_id
    JOIN CatchedPokemon
    	ON CatchedPokemon.owner_id = Trainer.id
        AND CatchedPokemon.pid = Pokemon.id
GROUP BY
	Pokemon.name
ORDER BY
	Pokemon.name;