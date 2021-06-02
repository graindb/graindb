SELECT MIN(mi_idx.info) AS rating,
       MIN(t.title) AS north_european_dark_production
FROM info_type AS it1 JOIN 
(movie_info AS mi JOIN 
  (title AS t 
    JOIN (movie_info_idx AS mi_idx JOIN info_type AS it2 ON it2.id = mi_idx.info_type_id) ON t.id = mi_idx.movie_id
    JOIN (movie_keyword AS mk JOIN keyword AS k ON k.id = mk.keyword_id) ON t.id = mk.movie_id
    JOIN kind_type AS kt ON kt.id = t.kind_id) 
  ON t.id = mi.movie_id) 
ON it1.id = mi.info_type_id
WHERE it1.info = 'countries'
  AND it2.info = 'rating'
  AND k.keyword IS NOT NULL
  AND k.keyword IN ('murder',
                    'murder-in-title',
                    'blood',
                    'violence')
  AND kt.kind IN ('movie',
                  'episode')
  AND mi.info IN ('Sweden',
                  'Norway',
                  'Germany',
                  'Denmark',
                  'Swedish',
                  'Danish',
                  'Norwegian',
                  'German',
                  'USA',
                  'American')
  AND mi_idx.info < '8.5'
  AND t.production_year > 2005;

