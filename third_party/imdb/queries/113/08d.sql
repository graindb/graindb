SELECT MIN(an.name) AS costume_designer_pseudo,
       MIN(t.title) AS movie_with_costumes
FROM company_name AS cn JOIN 
(movie_companies AS mc JOIN 
  (title AS t JOIN 
    (cast_info AS ci JOIN 
      role_type AS rt ON ci.role_id = rt.id JOIN (name AS n JOIN aka_name AS an ON an.person_id = n.id)
      ON n.id = ci.person_id) 
    ON ci.movie_id = t.id) 
  ON t.id = mc.movie_id) 
ON mc.company_id = cn.id
WHERE cn.country_code ='[us]'
  AND rt.role ='costume designer';

