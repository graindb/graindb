/* interactive-short-6 */
WITH RECURSIVE chain as (
    SELECT m_c_replyof as parent, m_messageid as child
    FROM message
    where m_messageid = 1236950581248
    UNION ALL
    SELECT p.m_c_replyof, p.m_messageid
    FROM message p,
         chain c
    where p.m_messageid = c.parent
)
select f_forumid, f_title, p_personid, p_firstname, p_lastname
from message,
     person,
     forum
where m_messageid = (select coalesce(min(parent), 1236950581248) from chain)
  and m_ps_forumid = f_forumid
  and f_moderatorid = p_personid;
