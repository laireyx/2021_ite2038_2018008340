SELECT
	Trainer.name
FROM
	Trainer
    JOIN Gym
    	ON Gym.leader_id = Trainer.id
    JOIN City
    	ON City.description = 'Amazon'
        AND Gym.city = City.name
GROUP BY
	Trainer.name;