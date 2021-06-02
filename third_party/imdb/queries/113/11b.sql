SELECT MIN(cn.name) AS from_company,
       MIN(lt.link) AS movie_link_type,
       MIN(t.title) AS sequel_movie
FROM title t
JOIN (movie_keyword AS mk JOIN keyword AS k ON mk.keyword_id = k.id) ON t.id = mk.movie_id
JOIN (movie_link AS ml JOIN link_type AS lt ON lt.id = ml.link_type_id) ON ml.movie_id = t.id
JOIN (movie_companies AS mc JOIN company_type AS ct ON mc.company_type_id = ct.id 
  JOIN company_name AS cn ON mc.company_id = cn.id) ON t.id = mc.movie_id
WHERE cn.country_code !='[pl]'
  AND (cn.name LIKE '%Film%'
       OR cn.name LIKE '%Warner%')
  AND ct.kind ='production companies'
  AND k.keyword ='sequel'
  AND lt.link LIKE '%follows%'
  AND mc.note IS NULL
  AND t.production_year = 1998
  AND t.title LIKE '%Money%';

