SELECT MIN(an.name) AS actress_pseudonym,
       MIN(t.title) AS japanese_movie_dubbed
FROM company_name AS cn JOIN 
(movie_companies AS mc JOIN 
  (title AS t JOIN 
    (cast_info AS ci JOIN 
      role_type AS rt ON ci.role_id = rt.id JOIN (name AS n JOIN aka_name AS an ON an.person_id = n.id)
      ON n.id = ci.person_id) 
    ON ci.movie_id = t.id) 
  ON t.id = mc.movie_id) 
ON mc.company_id = cn.id
WHERE ci.note ='(voice: English version)'
  AND cn.country_code ='[jp]'
  AND mc.note LIKE '%(Japan)%'
  AND mc.note NOT LIKE '%(USA)%'
  AND n.name LIKE '%Yo%'
  AND n.name NOT LIKE '%Yu%'
  AND rt.role ='actress';