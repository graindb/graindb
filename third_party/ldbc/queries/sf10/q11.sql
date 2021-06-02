/* interactive-complex-4 */
select t_name, count(*)
from tag,
     message,
     message_tag recent,
     knows
where m_messageid = mt_messageid
  and mt_tagid = t_tagid
  and m_creatorid = k_person2id
  and m_c_replyof IS NULL
  and -- post, not comment
    k_person1id = 13194139539415
  and m_creationdate >= '2012-06-01 08:00:00'
  and m_creationdate < '2012-07-02 08:00:00'
  and not exists(
        select *
        from (select distinct mt_tagid
              from message,
                   message_tag,
                   knows
              where k_person1id = 13194139539415
                and k_person2id = m_creatorid
                and m_c_replyof IS NULL
                and -- post, not comment
                  mt_messageid = m_messageid
                and m_creationdate < '2012-06-01 08:00:00') tags
        where tags.mt_tagid = recent.mt_tagid)
group by t_name
order by 2 desc, t_name limit 10;
