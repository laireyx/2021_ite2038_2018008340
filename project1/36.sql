SELECT
	Pokemon.name
FROM
	Pokemon
    LEFT JOIN Evolution
    	ON Evolution.before_id = Pokemon.id
WHERE
	Pokemon.type = 'Water' AND
    Evolution.after_id IS NULL