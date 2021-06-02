SELECT MIN(mc.note) AS production_note,
       MIN(t.title) AS movie_title,
       MIN(t.production_year) AS movie_year
FROM company_type ct JOIN 
  (movie_companies mc JOIN 
    (title t JOIN 
      (movie_info_idx mi_idx JOIN info_type it ON it.id = mi_idx.info_type_id) 
      ON t.id = mi_idx.movie_id) 
    ON mc.movie_id = t.id) 
  ON ct.id = mc.company_type_id
WHERE ct.kind = 'production companies'
  AND it.info = 'bottom 10 rank'
  AND mc.note NOT LIKE '%(as Metro-Goldwyn-Mayer Pictures)%'
  AND t.production_year BETWEEN 2005 AND 2010;