/* interactive-complex-2 */
select p_personid, p_firstname, p_lastname, m_messageid, COALESCE(m_ps_imagefile, m_content, ''), m_creationdate
from person,
     message,
     knows
where p_personid = m_creatorid
  and m_creationdate <= '2011-07-24 08:00:00'
  and k_person1id = 15393162793684
  and k_person2id = p_personid
order by m_creationdate desc, m_messageid asc limit 20;
