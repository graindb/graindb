SELECT MIN(n.name) AS voicing_actress,
       MIN(t.title) AS kung_fu_panda
FROM title AS t
JOIN (cast_info AS ci JOIN role_type AS rt ON rt.id = ci.role_id JOIN char_name AS chn ON chn.id = ci.person_role_id JOIN (name AS n JOIN aka_name AS an ON n.id = an.person_id) 
  ON n.id = ci.person_id) ON t.id = ci.movie_id
JOIN (movie_info AS mi JOIN info_type AS it ON it.id = mi.info_type_id) ON t.id = mi.movie_id
JOIN (movie_companies AS mc JOIN company_name AS cn ON cn.id = mc.company_id) ON t.id = mc.movie_id
WHERE ci.note = '(voice)'
  AND cn.country_code ='[us]'
  AND it.info = 'release dates'
  AND mc.note LIKE '%(200%)%'
  AND (mc.note LIKE '%(USA)%'
       OR mc.note LIKE '%(worldwide)%')
  AND mi.info IS NOT NULL
  AND (mi.info LIKE 'Japan:%2007%'
       OR mi.info LIKE 'USA:%2008%')
  AND n.gender ='f'
  AND n.name LIKE '%Angel%'
  AND rt.role ='actress'
  AND t.production_year BETWEEN 2007 AND 2008
  AND t.title LIKE '%Kung%Fu%Panda%';