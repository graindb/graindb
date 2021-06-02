SELECT MIN(an.name) AS alternative_name,
       MIN(chn.name) AS voiced_character,
       MIN(n.name) AS voicing_actress,
       MIN(t.title) AS american_movie
FROM title AS t
JOIN (movie_companies AS mc JOIN company_name AS cn ON mc.company_id = cn.id) ON t.id = mc.movie_id
JOIN (cast_info AS ci JOIN (name AS n JOIN aka_name AS an ON an.person_id = n.id) ON n.id = ci.person_id
    JOIN role_type AS rt ON ci.role_id = rt.id
    JOIN char_name AS chn ON chn.id = ci.person_role_id) 
ON ci.movie_id = t.id
WHERE ci.note = '(voice)'
  AND cn.country_code ='[us]'
  AND mc.note LIKE '%(200%)%'
  AND (mc.note LIKE '%(USA)%'
       OR mc.note LIKE '%(worldwide)%')
  AND n.gender ='f'
  AND n.name LIKE '%Angel%'
  AND rt.role ='actress'
  AND t.production_year BETWEEN 2007 AND 2010;

