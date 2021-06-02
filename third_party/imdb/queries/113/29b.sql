SELECT MIN(chn.name) AS voiced_char,
       MIN(n.name) AS voicing_actress,
       MIN(t.title) AS voiced_animation
FROM title AS t
JOIN (movie_info AS mi JOIN info_type AS it1 ON it1.id = mi.info_type_id) ON t.id = mi.movie_id
JOIN (movie_companies AS mc JOIN company_name AS cn ON cn.id = mc.company_id) ON t.id = mc.movie_id
JOIN (cast_info AS ci JOIN role_type AS rt ON rt.id = ci.role_id JOIN (name AS n JOIN aka_name AS an ON n.id = an.person_id JOIN (person_info AS pi JOIN info_type AS it2 ON it2.id = pi.info_type_id) ON n.id = pi.person_id) ON n.id = ci.person_id JOIN char_name AS chn ON chn.id = ci.person_role_id) ON t.id = ci.movie_id
JOIN (movie_keyword AS mk JOIN keyword AS k ON k.id = mk.keyword_id) ON t.id = mk.movie_id
JOIN (complete_cast AS cc JOIN comp_cast_type AS cct1 ON cct1.id = cc.subject_id JOIN comp_cast_type AS cct2 ON cct2.id = cc.status_id) ON t.id = cc.movie_id
WHERE cct1.kind ='cast'
  AND cct2.kind ='complete+verified'
  AND chn.name = 'Queen'
  AND ci.note IN ('(voice)',
                  '(voice) (uncredited)',
                  '(voice: English version)')
  AND cn.country_code ='[us]'
  AND it1.info = 'release dates'
  AND it2.info = 'height'
  AND k.keyword = 'computer-animation'
  AND mi.info LIKE 'USA:%200%'
  AND n.gender ='f'
  AND n.name LIKE '%An%'
  AND rt.role ='actress'
  AND t.title = 'Shrek 2'
  AND t.production_year BETWEEN 2000 AND 2005;

