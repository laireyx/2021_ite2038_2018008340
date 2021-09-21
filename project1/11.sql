SELECT nickname
FROM
	CatchedPokemon
    JOIN Trainer
    	ON CatchedPokemon.owner_id = Trainer.id
    JOIN Gym
    	ON Gym.leader_id = Trainer.id
        AND Gym.city = 'Sangnok City'
    JOIN Pokemon
    	ON Pokemon.id = CatchedPokemon.pid
WHERE
	Pokemon.type = 'Water'
ORDER BY
	nickname;