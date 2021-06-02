SELECT MIN(cn.name) AS producing_company,
       MIN(lt.link) AS link_type,
       MIN(t.title) AS complete_western_sequel
FROM title AS t
JOIN (movie_link AS ml JOIN link_type AS lt ON lt.id = ml.link_type_id) ON ml.movie_id = t.id
JOIN (movie_keyword AS mk JOIN keyword AS k ON mk.keyword_id = k.id) ON t.id = mk.movie_id
JOIN (movie_companies AS mc JOIN company_type AS ct ON mc.company_type_id = ct.id JOIN company_name AS cn ON mc.company_id = cn.id) ON t.id = mc.movie_id
JOIN movie_info AS mi ON mi.movie_id = t.id
JOIN (complete_cast AS cc JOIN comp_cast_type AS cct1 ON cct1.id = cc.subject_id JOIN comp_cast_type AS cct2 ON cct2.id = cc.status_id) ON t.id = cc.movie_id
WHERE cct1.kind IN ('cast',
                    'crew')
  AND cct2.kind = 'complete'
  AND cn.country_code !='[pl]'
  AND (cn.name LIKE '%Film%'
       OR cn.name LIKE '%Warner%')
  AND ct.kind ='production companies'
  AND k.keyword ='sequel'
  AND lt.link LIKE '%follow%'
  AND mc.note IS NULL
  AND mi.info IN ('Sweden',
                  'Germany',
                  'Swedish',
                  'German')
  AND t.production_year BETWEEN 1950 AND 2000;

