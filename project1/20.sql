SELECT
	Trainer.name
FROM
	Trainer
    JOIN Gym
    	ON Gym.leader_id = Trainer.id
ORDER BY
	Trainer.name