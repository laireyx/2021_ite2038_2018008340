SELECT
	name, AVG(CatchedPokemon.level)
FROM
	Trainer, Gym, CatchedPokemon
WHERE
	Trainer.id = Gym.leader_id AND
    Trainer.id = CatchedPokemon.owner_id
GROUP BY
	Trainer.id
ORDER BY
	Trainer.name;