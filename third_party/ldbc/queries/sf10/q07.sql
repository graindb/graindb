/* interactive-short-7 */
select m2.m_messageid,
       m2.m_content,
       m2.m_creationdate,
       p2.p_personid,
       p2.p_firstname,
       p2.p_lastname,
       (case
            when exists(
                    select 1
                    from knows
                    where p1.p_personid = k_person1id
                      and p2.p_personid = k_person2id)
                then TRUE
            else FALSE
           end)
from message m1,
     message m2,
     person p1,
     person p2
where m1.m_messageid = 1236950581248
  and m2.m_c_replyof = m1.m_messageid
  and m2.m_creatorid = p2.p_personid
  and m1.m_creatorid = p1.p_personid
order by m2.m_creationdate desc, p2.p_personid asc;
