SELECT MIN(t.title) AS movie_title
FROM keyword AS k JOIN (
  movie_keyword AS mk JOIN (
  	title AS t JOIN (
  		movie_info AS mi JOIN info_type AS it 
  		ON it.id = mi.info_type_id)
  	ON t.id = mi.movie_id)
  ON t.id = mk.movie_id)
ON k.id = mk.keyword_id
WHERE k.keyword LIKE '%sequel%'
  AND t.production_year > 1990;
