/* bi-6 */
WITH detail AS (
SELECT cr.p_personid AS person_id
     , count(DISTINCT r.m_messageid)  AS replyCount
     , count(DISTINCT l.l_messageid||' '||l.l_personid) AS likeCount
     , count(DISTINCT m.m_messageid)  AS messageCount
     , null as score
  FROM tag t
     , message_tag pt
     , message m LEFT JOIN message  r ON (m.m_messageid = r.m_c_replyof)
              LEFT JOIN likes l ON (m.m_messageid = l.l_messageid)
     , person cr
 WHERE 1=1
   AND t.t_tagid = pt.mt_tagid
   AND pt.mt_messageid = m.m_messageid
   AND m.m_creatorid = cr.p_personid
   AND t.t_name = 'Garry_Kasparov'
 GROUP BY cr.p_personid
)
SELECT person_id AS "person.id"
     , replyCount
     , likeCount
     , messageCount
     , 1*messageCount + 2*replyCount + 10*likeCount AS score
  FROM detail
 ORDER BY score DESC, person_id
 LIMIT 100
;
