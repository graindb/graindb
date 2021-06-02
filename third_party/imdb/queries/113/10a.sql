SELECT MIN(chn.name) AS uncredited_voiced_character,
       MIN(t.title) AS russian_movie
FROM movie_companies AS mc JOIN company_name AS cn ON cn.id = mc.company_id JOIN company_type AS ct ON ct.id = mc.company_type_id
JOIN (title AS t JOIN 
  (cast_info AS ci JOIN role_type AS rt ON rt.id = ci.role_id 
    JOIN char_name AS chn ON chn.id = ci.person_role_id) 
  ON t.id = ci.movie_id) 
ON t.id = mc.movie_id
WHERE ci.note LIKE '%(voice)%'
  AND ci.note LIKE '%(uncredited)%'
  AND cn.country_code = '[ru]'
  AND rt.role = 'actor'
  AND t.production_year > 2005;

