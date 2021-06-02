/* bi-2 */
SELECT co.pl_name       AS country_name
     , p.m_creationdate as messageMonth
     , cr.p_gender      AS person_gender
     , cr.p_birthday    AS ageGroup
     , t.t_name         AS tag_name
     , count(*)         AS messageCount
FROM message p
   , message_tag pt
   , tag t
   , person cr -- creator
   , place ci  -- city
   , place co  -- country
WHERE 1 = 1
  -- join
  AND p.m_messageid = pt.mt_messageid
  AND pt.mt_tagid = t.t_tagid
  AND p.m_creatorid = cr.p_personid
  AND cr.p_placeid = ci.pl_placeid
  AND ci.pl_containerplaceid = co.pl_placeid
  -- filter
  AND co.pl_name in ('Egypt', 'Spain')
  AND p.m_creationdate >= '2010-01-01'
  AND p.m_creationdate <= '2010-11-22'
GROUP BY co.pl_name, messageMonth, cr.p_gender, t.t_name, ageGroup
HAVING count(*) > 100 LIMIT 100
;
