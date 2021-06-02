/* bi-21 */
WITH zombies AS (
    SELECT p.p_personid AS zombieid
    FROM place co -- country
       , place ci -- city
       , person p
             LEFT JOIN message m ON (p.p_personid = m.m_creatorid AND
                                     m.m_creationdate BETWEEN p.p_creationdate AND epoch_ms(1357016400000))
    WHERE 1 = 1
      -- join
      AND co.pl_placeid = ci.pl_containerplaceid
      AND ci.pl_placeid = p.p_placeid
      -- filter
      AND co.pl_name = 'Egypt'
      AND p.p_creationdate < '2013-01-01'
    GROUP BY p.p_personid
             -- average of [0, 1) messages per month is equivalent with having less messages than the month span between person creationDate and parameter 1357016400000
    HAVING count(m_messageid) <
           12 * extract('YEAR' FROM DATE epoch_ms(1357016400000)) + extract('MONTH' FROM DATE epoch_ms(1357016400000)) -
           (12 * extract('YEAR' FROM DATE p.p_creationdate) + extract('MONTH' FROM DATE p.p_creationdate)) + 1
)
SELECT z.zombieid          AS "zombie.id"
     , count(zl.zombieid)  AS zombieLikeCount
     , count(l.l_personid) AS totalLikeCount
     , CASE WHEN count(l.l_personid) = 0 THEN 0 ELSE count(zl.zombieid)::float/count(l.l_personid) END AS zombieScore
FROM message m
    INNER JOIN likes l
ON (m.m_messageid = l.l_messageid)
    INNER JOIN person p ON (l.l_personid = p.p_personid AND p.p_creationdate < epoch_ms(1357016400000))
    LEFT JOIN zombies zl ON (p.p_personid = zl.zombieid) -- see if the like was given by a zombie
    RIGHT JOIN zombies z ON (z.zombieid = m.m_creatorid)
GROUP BY z.zombieid
ORDER BY zombieScore DESC, z.zombieid
    LIMIT 100
;
