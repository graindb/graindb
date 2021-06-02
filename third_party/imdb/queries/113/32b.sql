SELECT MIN(lt.link) AS link_type,
       MIN(t1.title) AS first_movie,
       MIN(t2.title) AS second_movie
FROM movie_link AS ml JOIN link_type AS lt ON lt.id = ml.link_type_id
JOIN title AS t2 ON ml.linked_movie_id = t2.id
JOIN (title AS t1 JOIN (movie_keyword AS mk JOIN keyword AS k ON mk.keyword_id = k.id) 
  ON t1.id = mk.movie_id) ON ml.movie_id = t1.id
WHERE k.keyword ='character-name-in-title';

