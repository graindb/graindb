/* bi-12 */
SELECT m.m_messageid    AS "message.id"
     , m.m_creationdate AS "message.creationDate"
     , c.p_firstname    AS "creator.firstName"
     , c.p_lastname     AS "creator.lastName"
     , count(*)         as likeCount
FROM message m
   , person c -- creator
   , likes l
WHERE 1 = 1
  -- join
  AND m.m_creatorid = c.p_personid
  AND m.m_messageid = l.l_messageid
  -- filter
  AND m.m_creationdate > '2011-11-15 00:00:00'
GROUP BY m.m_messageid
       , m.m_creationdate
       , c.p_firstname
       , c.p_lastname
HAVING count(*) > 400
ORDER BY likeCount DESC, m.m_messageid LIMIT 100
;
