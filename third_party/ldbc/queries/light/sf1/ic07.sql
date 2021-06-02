/* interactive-complex-7 */
select p2.p_personid, p2.p_firstname, p2.p_lastname, l_creationdate, m_messageid
from likes, message, person p1, person p2
where
m_creatorid = p1.p_personid and 
m_messageid = l_messageid and
p2.p_personid = l_personid and
p1.id=5132
;