/* interactive-complex-2 */
select p2.id, p2.p_firstname, p2.p_lastname, message.m_messageid, message.m_content, message.m_creationdate
from person p1 JOIN (
    knows JOIN (
        person p2 JOIN message
            ON p2.p_personid=m_creatorid)
        ON knows.k_person2id=p2.p_personid)
    ON p1.p_personid=k_person1id
where m_creationdate <= '2012-06-01 08:00:00'
  and p1.id = 30786325734145
;