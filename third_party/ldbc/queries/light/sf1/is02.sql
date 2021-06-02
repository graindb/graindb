select m1.m_messageid, m1.m_creationdate,
       m2.m_messageid, p2.p_personid, p2.p_firstname, p2.p_lastname
from person p1, person p2, message m1, message m2
where p1.p_personid=8796093028953 and m1.m_creatorid=p1.p_personid and m1.m_c_replyof = m2.m_messageid and m2.m_creatorid=p2.p_personid;
