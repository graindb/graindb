SELECT MIN(chn.name) AS voiced_char_name,
       MIN(n.name) AS voicing_actress_name,
       MIN(t.title) AS voiced_action_movie_jap_eng
FROM title AS t
JOIN (movie_companies AS mc JOIN company_name AS cn ON cn.id = mc.company_id) ON t.id = mc.movie_id
JOIN (movie_keyword AS mk JOIN keyword AS k ON k.id = mk.keyword_id) ON t.id = mk.movie_id
JOIN (movie_info AS mi JOIN info_type AS it ON it.id = mi.info_type_id) ON t.id = mi.movie_id
JOIN (cast_info AS ci JOIN role_type AS rt ON rt.id = ci.role_id JOIN char_name AS chn ON chn.id = ci.person_role_id JOIN (name AS n JOIN aka_name AS an ON n.id = an.person_id) ON n.id = ci.person_id) ON t.id = ci.movie_id
WHERE ci.note IN ('(voice)',
                  '(voice: Japanese version)',
                  '(voice) (uncredited)',
                  '(voice: English version)')
  AND cn.country_code ='[us]'
  AND it.info = 'release dates'
  AND k.keyword IN ('hero',
                    'martial-arts',
                    'hand-to-hand-combat')
  AND mi.info IS NOT NULL
  AND (mi.info LIKE 'Japan:%201%'
       OR mi.info LIKE 'USA:%201%')
  AND n.gender ='f'
  AND n.name LIKE '%An%'
  AND rt.role ='actress'
  AND t.production_year > 2010;