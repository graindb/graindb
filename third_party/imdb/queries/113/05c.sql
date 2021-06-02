SELECT MIN(t.title) AS american_movie
FROM title AS t JOIN 
(movie_companies AS mc 
  JOIN company_type AS ct ON ct.id = mc.company_type_id) 
ON t.id = mc.movie_id JOIN 
(movie_info AS mi 
  JOIN info_type AS it ON it.id = mi.info_type_id) 
ON t.id = mi.movie_id
WHERE ct.kind = 'production companies'
  AND mc.note NOT LIKE '%(TV)%'
  AND mc.note LIKE '%(USA)%'
  AND mi.info IN ('Sweden',
                  'Norway',
                  'Germany',
                  'Denmark',
                  'Swedish',
                  'Denish',
                  'Norwegian',
                  'German',
                  'USA',
                  'American')
  AND t.production_year > 1990;