SELECT MIN(cn.name) AS company_name,
       MIN(lt.link) AS link_type,
       MIN(t.title) AS western_follow_up
FROM title AS t 
JOIN (movie_link AS ml JOIN link_type AS lt ON lt.id = ml.link_type_id) ON ml.movie_id = t.id
JOIN (movie_keyword AS mk JOIN keyword AS k ON mk.keyword_id = k.id) ON t.id = mk.movie_id
JOIN (movie_companies AS mc JOIN company_name AS cn ON mc.company_id = cn.id JOIN company_type AS ct ON mc.company_type_id = ct.id) ON t.id = mc.movie_id
JOIN movie_info AS mi ON mi.movie_id = t.id
WHERE cn.country_code !='[pl]'
  AND (cn.name LIKE '%Film%'
       OR cn.name LIKE '%Warner%')
  AND ct.kind ='production companies'
  AND k.keyword ='sequel'
  AND lt.link LIKE '%follow%'
  AND mc.note IS NULL
  AND mi.info IN ('Sweden',
                  'Norway',
                  'Germany',
                  'Denmark',
                  'Swedish',
                  'Denish',
                  'Norwegian',
                  'German')
  AND t.production_year BETWEEN 1950 AND 2000;

