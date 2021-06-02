/* interactive-complex-3 */
select p2.id, p2.p_firstname, p2.p_lastname
from person p1 JOIN (knows k1 
    JOIN (knows k2
        JOIN (person p2 
        	JOIN (message m1 JOIN place pl1 ON m1.m_locationid=pl1.pl_placeid) ON m1.m_creatorid=p2.p_personid 
        	JOIN (message m2 JOIN place pl2 ON m2.m_locationid=pl2.pl_placeid) ON m2.m_creatorid=p2.p_personid
        	) ON k2.k_person2id=p2.p_personid)
        ON k2.k_person1id = k1.k_person2id)
    ON p1.p_personid=k1.k_person1id
WHERE p1.id = 2199023262699
    AND pl1.pl_name = 'Papua_New_Guinea' -- 'Norway'
    AND pl2.pl_name = 'El_Salvador'
    AND m1.m_creationdate >= '2010-12-01 08:00:00'
    AND m1.m_creationdate < '2010-12-31 08:00:00'
    AND m2.m_creationdate >= '2010-12-01 08:00:00'
    AND m2.m_creationdate < '2010-12-31 08:00:00'
;
