SELECT
	Trainer.name
FROM
	Trainer
    JOIN Gym
    	ON Gym.leader_id = Trainer.id
        AND Gym.city = 'Brown City'