SELECT MIN(cn.name) AS from_company,
       MIN(mc.note) AS production_note,
       MIN(t.title) AS movie_based_on_book
FROM title t
JOIN (movie_keyword AS mk JOIN keyword AS k ON mk.keyword_id = k.id) ON t.id = mk.movie_id
JOIN (movie_link AS ml JOIN link_type AS lt ON lt.id = ml.link_type_id) ON ml.movie_id = t.id
JOIN (movie_companies AS mc JOIN company_type AS ct ON mc.company_type_id = ct.id 
  JOIN company_name AS cn ON mc.company_id = cn.id) ON t.id = mc.movie_id
WHERE cn.country_code !='[pl]'
  AND (cn.name LIKE '20th Century Fox%'
       OR cn.name LIKE 'Twentieth Century Fox%')
  AND ct.kind != 'production companies'
  AND ct.kind IS NOT NULL
  AND k.keyword IN ('sequel',
                    'revenge',
                    'based-on-novel')
  AND mc.note IS NOT NULL
  AND t.production_year > 1950;

