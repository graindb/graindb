SELECT MIN(cn.name) AS producing_company,
       MIN(miidx.info) AS rating,
       MIN(t.title) AS movie_about_winning
FROM kind_type AS kt JOIN 
(title AS t JOIN (movie_info_idx AS miidx JOIN info_type AS it ON it.id = miidx.info_type_id) ON miidx.movie_id = t.id 
  JOIN (movie_info AS mi JOIN info_type AS it2 ON it2.id = mi.info_type_id) ON mi.movie_id = t.id
  JOIN (movie_companies AS mc JOIN company_name AS cn ON cn.id = mc.company_id JOIN company_type AS ct ON ct.id = mc.company_type_id) ON mc.movie_id = t.id)
ON kt.id = t.kind_id
WHERE cn.country_code ='[us]'
  AND ct.kind ='production companies'
  AND it.info ='rating'
  AND it2.info ='release dates'
  AND kt.kind ='movie'
  AND t.title != ''
  AND (t.title LIKE '%Champion%'
       OR t.title LIKE '%Loser%');

