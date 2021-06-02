SELECT MIN(n.name) AS member_in_charnamed_movie
FROM title AS t 
JOIN (cast_info AS ci JOIN name AS n ON n.id = ci.person_id) ON ci.movie_id = t.id
JOIN (movie_companies AS mc JOIN company_name AS cn ON mc.company_id = cn.id) ON t.id = mc.movie_id
JOIN (movie_keyword AS mk JOIN keyword AS k ON mk.keyword_id = k.id) ON t.id = mk.movie_id
WHERE cn.country_code ='[us]'
  AND k.keyword ='character-name-in-title';
