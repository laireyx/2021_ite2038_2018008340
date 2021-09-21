SELECT
	City.name
FROM
    City
    JOIN Trainer
		ON City.name = Trainer.hometown
GROUP BY
	City.name
HAVING
	COUNT(Trainer.id) = (
      SELECT MAX(cnt) FROM (
      	SELECT COUNT(OtherTrainer.id) AS cnt
    		FROM City OtherCity
        	JOIN Trainer OtherTrainer
        		ON OtherTrainer.hometown = OtherCity.name
        	GROUP BY OtherCity.name
		) TrainerCount
    );