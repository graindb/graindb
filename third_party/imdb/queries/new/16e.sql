SELECT MIN(an.name) AS cool_actor_pseudonym,
       MIN(t.title) AS series_named_after_char
FROM title AS t 
JOIN (movie_companies AS mc JOIN company_name AS cn ON mc.company_id = cn.id) ON t.id = mc.movie_id
JOIN (movie_keyword AS mk JOIN keyword AS k ON mk.keyword_id = k.id) ON t.id = mk.movie_id
JOIN (cast_info AS ci JOIN (name AS n JOIN aka_name AS an ON an.person_id = n.id) ON n.id = ci.person_id) ON ci.movie_id = t.id
WHERE cn.country_code ='[us]'
  AND k.keyword ='character-name-in-title';
