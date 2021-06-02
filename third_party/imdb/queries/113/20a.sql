SELECT MIN(t.title) AS complete_downey_ironman_movie
FROM title AS t
JOIN (movie_keyword AS mk JOIN keyword AS k ON k.id = mk.keyword_id) ON t.id = mk.movie_id
JOIN (cast_info AS ci JOIN name AS n ON n.id = ci.person_id JOIN char_name AS chn ON chn.id = ci.person_role_id) ON t.id = ci.movie_id
JOIN (complete_cast AS cc JOIN comp_cast_type AS cct1 ON cct1.id = cc.subject_id JOIN comp_cast_type AS cct2 ON cct2.id = cc.status_id) ON t.id = cc.movie_id
JOIN kind_type AS kt ON kt.id = t.kind_id
WHERE cct1.kind = 'cast'
  AND cct2.kind LIKE '%complete%'
  AND chn.name NOT LIKE '%Sherlock%'
  AND (chn.name LIKE '%Tony%Stark%'
       OR chn.name LIKE '%Iron%Man%')
  AND k.keyword IN ('superhero',
                    'sequel',
                    'second-part',
                    'marvel-comics',
                    'based-on-comic',
                    'tv-special',
                    'fight',
                    'violence')
  AND kt.kind = 'movie'
  AND t.production_year > 1950;

