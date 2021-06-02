SELECT MIN(n.name) AS voicing_actress,
       MIN(t.title) AS jap_engl_voiced_movie
FROM title AS t
JOIN (cast_info AS ci JOIN role_type AS rt ON rt.id = ci.role_id JOIN char_name AS chn ON chn.id = ci.person_role_id JOIN (name AS n JOIN aka_name AS an ON n.id = an.person_id) 
  ON n.id = ci.person_id) ON t.id = ci.movie_id
JOIN (movie_info AS mi JOIN info_type AS it ON it.id = mi.info_type_id) ON t.id = mi.movie_id
JOIN (movie_companies AS mc JOIN company_name AS cn ON cn.id = mc.company_id) ON t.id = mc.movie_id
WHERE ci.note IN ('(voice)',
                  '(voice: Japanese version)',
                  '(voice) (uncredited)',
                  '(voice: English version)')
  AND cn.country_code ='[us]'
  AND it.info = 'release dates'
  AND mi.info IS NOT NULL
  AND (mi.info LIKE 'Japan:%200%'
       OR mi.info LIKE 'USA:%200%')
  AND n.gender ='f'
  AND n.name LIKE '%An%'
  AND rt.role ='actress'
  AND t.production_year > 2000;

