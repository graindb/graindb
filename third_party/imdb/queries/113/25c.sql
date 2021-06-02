SELECT MIN(mi.info) AS movie_budget,
       MIN(mi_idx.info) AS movie_votes,
       MIN(n.name) AS male_writer,
       MIN(t.title) AS violent_movie_title
FROM title AS t
JOIN (movie_info AS mi JOIN info_type AS it1 ON it1.id = mi.info_type_id) ON t.id = mi.movie_id
JOIN (cast_info AS ci JOIN name AS n ON n.id = ci.person_id) ON t.id = ci.movie_id
JOIN (movie_keyword AS mk JOIN keyword AS k ON k.id = mk.keyword_id) ON t.id = mk.movie_id
JOIN (movie_info_idx AS mi_idx JOIN info_type AS it2 ON it2.id = mi_idx.info_type_id) ON t.id = mi_idx.movie_id
WHERE ci.note IN ('(writer)',
                  '(head writer)',
                  '(written by)',
                  '(story)',
                  '(story editor)')
  AND it1.info = 'genres'
  AND it2.info = 'votes'
  AND k.keyword IN ('murder',
                    'violence',
                    'blood',
                    'gore',
                    'death',
                    'female-nudity',
                    'hospital')
  AND mi.info IN ('Horror',
                  'Action',
                  'Sci-Fi',
                  'Thriller',
                  'Crime',
                  'War')
  AND n.gender = 'm';

