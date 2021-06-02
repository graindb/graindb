select f_forumid, f_title, p_personid, p_firstname, p_lastname
from message m1, message m2, person, forum
where m1.m_messageid = 13726982 and m2.m_c_replyof = m1.m_messageid
  and m2.m_ps_forumid = f_forumid and f_moderatorid = p_personid;
