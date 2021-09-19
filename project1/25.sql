SELECT
	Pokemon.name
FROM
	Pokemon
    	INNER JOIN Evolution
        	ON Pokemon.id = Evolution.after_id
        LEFT JOIN Evolution SecondEvolution
        	ON Pokemon.id = SecondEvolution.after_id
    	LEFT JOIN Evolution FirstEvolution
        	ON FirstEvolution.after_id = SecondEvolution.before_id
WHERE
	FirstEvolution.before_id IS NULL
GROUP BY
	Pokemon.name
ORDER BY
	Pokemon.name