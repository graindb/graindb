SELECT MIN(n.name) AS cast_member_name,
       MIN(pi.info) AS cast_member_info
FROM title t
JOIN (movie_link AS ml JOIN link_type AS lt ON lt.id = ml.link_type_id) ON ml.linked_movie_id = t.id
JOIN (cast_info AS ci JOIN (name AS n JOIN aka_name AS an ON n.id = an.person_id 
  JOIN (person_info AS pi JOIN info_type AS it ON it.id = pi.info_type_id) ON n.id = pi.person_id) 
  ON ci.person_id = n.id) ON t.id = ci.movie_id
WHERE an.name IS NOT NULL
  AND (an.name LIKE '%a%'
       OR an.name LIKE 'A%')
  AND it.info ='mini biography'
  AND lt.link IN ('references',
                  'referenced in',
                  'features',
                  'featured in')
  AND n.name_pcode_cf BETWEEN 'A' AND 'F'
  AND (n.gender='m'
       OR (n.gender = 'f'
           AND n.name LIKE 'A%'))
  AND pi.note IS NOT NULL
  AND t.production_year BETWEEN 1980 AND 2010;