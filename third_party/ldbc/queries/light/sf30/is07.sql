select m2.m_messageid, m2.m_content, m2.m_creationdate, p_personid, p_firstname, p_lastname
from message m1, message m2, person
where
  m1.m_messageid = 13726982 and m2.m_c_replyof = m1.m_messageid and m2.m_creatorid = p_personid;
