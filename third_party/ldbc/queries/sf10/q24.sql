/* bi-3 */
-- WITH detail AS (
SELECT t.t_name,
       count(DISTINCT m.m_creationdate)
--          , count(DISTINCT CASE
--                               WHEN extract('MONTH' FROM DATE m.m_creationdate) = 11 THEN m.m_messageid
--                               ELSE NULL END) AS countMonth1
--          , count(DISTINCT CASE
--                               WHEN extract('MONTH' FROM DATE m.m_creationdate) != 11 THEN m.m_messageid
--                               ELSE NULL END) AS countMonth2
FROM message m
   , message_tag mt
   , tag t
WHERE 1 = 1
  -- join
  AND m.m_messageid = mt.mt_messageid
  AND mt.mt_tagid = t.t_tagid
  -- filter
  AND m.m_creationdate >= '2010-11-01'
  AND m.m_creationdate < '2011-01-01'
GROUP BY t.t_name
-- )
-- SELECT t_name as tag_name m_creationdate
--      , countMonth1
--      , countMonth2
--      , abs(countMonth1 - countMonth2) AS diff
-- FROM detail d
-- ORDER BY t_name
    LIMIT 100
;
