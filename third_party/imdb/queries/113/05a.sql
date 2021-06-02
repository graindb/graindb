SELECT MIN(t.title) AS typical_european_movie
FROM title AS t JOIN 
(movie_companies AS mc 
  JOIN company_type AS ct ON ct.id = mc.company_type_id) 
ON t.id = mc.movie_id JOIN 
(movie_info AS mi 
  JOIN info_type AS it ON it.id = mi.info_type_id) 
ON t.id = mi.movie_id
WHERE ct.kind = 'production companies'
  AND mc.note LIKE '%(theatrical)%'
  AND mc.note LIKE '%(France)%'
  AND mi.info IN ('Sweden',
                  'Norway',
                  'Germany',
                  'Denmark',
                  'Swedish',
                  'Denish',
                  'Norwegian',
                  'German')
  AND t.production_year > 2005;