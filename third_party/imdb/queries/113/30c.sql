SELECT MIN(mi.info) AS movie_budget,
       MIN(mi_idx.info) AS movie_votes,
       MIN(n.name) AS writer,
       MIN(t.title) AS complete_violent_movie
FROM title AS t
JOIN (movie_info AS mi JOIN info_type AS it1 ON it1.id = mi.info_type_id) ON t.id = mi.movie_id
JOIN (movie_info_idx AS mi_idx JOIN info_type AS it2 ON it2.id = mi_idx.info_type_id) ON t.id = mi_idx.movie_id
JOIN (cast_info AS ci JOIN name AS n ON n.id = ci.person_id) ON t.id = ci.movie_id
JOIN (movie_keyword AS mk JOIN keyword AS k ON k.id = mk.keyword_id) ON t.id = mk.movie_id
JOIN (complete_cast AS cc JOIN comp_cast_type AS cct1 ON cct1.id = cc.subject_id JOIN comp_cast_type AS cct2 ON cct2.id = cc.status_id)
 ON t.id = cc.movie_id
WHERE cct1.kind = 'cast'
  AND cct2.kind ='complete+verified'
  AND ci.note IN ('(writer)',
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

