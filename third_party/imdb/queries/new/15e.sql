SELECT MIN(mi.info) AS release_date,
       MIN(t.title) AS youtube_movie
FROM title AS t JOIN aka_title AS at ON t.id = at.movie_id
JOIN (movie_info AS mi JOIN info_type AS it ON it.id = mi.info_type_id) ON t.id = mi.movie_id
JOIN (movie_keyword AS mk JOIN keyword AS k ON k.id = mk.keyword_id) ON t.id = mk.movie_id
JOIN (movie_companies AS mc JOIN company_name AS cn ON cn.id = mc.company_id JOIN company_type AS ct ON ct.id = mc.company_type_id) ON t.id = mc.movie_id
WHERE cn.country_code = '[us]'
  AND cn.name = 'YouTube'
  AND it.info = 'release dates'
  AND mc.note LIKE '%(200%)%'
  AND mc.note LIKE '%(worldwide)%'
  AND mi.note LIKE '%internet%'
  AND mi.info LIKE 'USA:% 200%'
  AND t.production_year BETWEEN 2005 AND 2010;
