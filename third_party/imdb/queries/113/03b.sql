SELECT MIN(t.title) AS movie_title
FROM title AS t JOIN movie_info AS mi ON t.id = mi.movie_id 
JOIN (movie_keyword AS mk 
  JOIN keyword AS k 
  ON k.id = mk.keyword_id) 
ON t.id = mk.movie_id
WHERE k.keyword LIKE '%sequel%'
  AND mi.info IN ('Bulgaria')
  AND t.production_year > 2010;

