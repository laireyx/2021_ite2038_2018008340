SELECT
	Pokemon.name
FROM
	Pokemon
    JOIN Evolution SecondEvolution
    	ON Pokemon.id = SecondEvolution.after_id
    JOIN Evolution FirstEvolution
    	ON FirstEvolution.after_id = SecondEvolution.before_id
    JOIN Pokemon PrimitivePokemon
    	ON PrimitivePokemon.name = 'Charmander'
        AND PrimitivePokemon.id = FirstEvolution.before_id
GROUP BY
	Pokemon.name;