SELECT MIN(t.title) AS movie_title
FROM company_name AS cn JOIN 
(movie_companies AS mc JOIN 
  (movie_keyword AS mk JOIN 
    title AS t ON t.id = mk.movie_id JOIN keyword AS k ON mk.keyword_id = k.id) 
  ON mc.movie_id = t.id)
ON cn.id = mc.company_id
WHERE cn.country_code ='[nl]'
  AND k.keyword ='character-name-in-title';

