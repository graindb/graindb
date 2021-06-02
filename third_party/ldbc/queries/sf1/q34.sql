/* bi-13 */
-- WITH detail AS (
SELECT
--     extract('YEAR' FROM DATE m.m_creationdate) as year
--    , extract ('MONTH' FROM DATE m.m_creationdate) as month
    t.t_name                         as tagName
--    , m.m_creationdate
     , count(DISTINCT m.m_messageid) as popularity
--    , row_number() OVER (PARTITION BY extract ('YEAR' FROM DATE m.m_creationdate)
--    , extract ('MONTH' FROM DATE m.m_creationdate)
--     ORDER BY t.t_name IS NULL -- sort order is: false, true i.e. first the given, then missing tags
--    , count (DISTINCT m.m_messageid) DESC
--    , t.t_name
--     ) as rn
FROM place c -- country
   , message m
         LEFT JOIN message_tag pt
                   ON (m.m_messageid = pt.mt_messageid)
         LEFT JOIN tag t ON (pt.mt_tagid = t.t_tagid)
WHERE 1 = 1
  -- join
  AND c.pl_placeid = m.m_locationid
  -- filter
  AND c.pl_name = 'Egypt'
GROUP BY t.t_name
--     )
-- SELECT year, month
         --, CASE
         --WHEN count(tagName)=0 THEN ARRAY[]::varchar[][] -- for the given month, no messages have any tags, so we return an empty array
         -- we have non-missing tags, and also the missing tag might show up, so we filter it out
         --ELSE array_agg(ARRAY[tagName, cast(popularity AS VARCHAR)] ORDER BY popularity DESC, tagName) FILTER (WHERE tagName IS NOT NULL)
         --END AS popularTags
-- FROM detail
-- WHERE rn <= 5
-- GROUP BY year, month
-- ORDER BY year DESC, month
    LIMIT 100
;
