/* interactive-short-5 */
select p_personid, p_firstname, p_lastname
from message,
     person
where m_messageid = 1236950581248
  and m_creatorid = p_personid;
