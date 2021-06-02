SELECT MIN(t.title) AS movie_title
FROM keyword AS k JOIN (
  movie_keyword AS mk JOIN title AS t 
  ON t.id = mk.movie_id)
ON k.id = mk.keyword_id
WHERE k.keyword = 'character-name-in-title'
  AND t.production_year > 1990;