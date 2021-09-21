SELECT name
FROM Trainer, CatchedPokemon
WHERE Trainer.id = CatchedPokemon.owner_id
GROUP BY Trainer.id
HAVING COUNT(Trainer.id) >= 3
ORDER BY COUNT(Trainer.id);