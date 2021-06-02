SELECT MIN(kt.kind) AS movie_kind,
       MIN(t.title) AS complete_us_internet_movie
FROM title AS t
JOIN (movie_keyword AS mk JOIN keyword AS k ON k.id = mk.keyword_id) ON t.id = mk.movie_id
JOIN (movie_info AS mi JOIN info_type AS it1 ON it1.id = mi.info_type_id) ON t.id = mi.movie_id
JOIN (complete_cast AS cc JOIN comp_cast_type AS cct1 ON cct1.id = cc.status_id) ON t.id = cc.movie_id
JOIN (movie_companies AS mc JOIN company_type AS ct ON ct.id = mc.company_type_id JOIN company_name AS cn ON cn.id = mc.company_id) ON t.id = mc.movie_id
JOIN kind_type AS kt ON kt.id = t.kind_id  
WHERE cct1.kind = 'complete+verified'
  AND cn.country_code = '[us]'
  AND it1.info = 'release dates'
  AND kt.kind IN ('movie',
                  'tv movie',
                  'video movie',
                  'video game')
  AND mi.note LIKE '%internet%'
  AND mi.info IS NOT NULL
  AND (mi.info LIKE 'USA:% 199%'
       OR mi.info LIKE 'USA:% 200%')
  AND t.production_year > 1990;

