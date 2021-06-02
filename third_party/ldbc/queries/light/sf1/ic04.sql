/* interactive-complex-4 */
select t_name
from person p1 JOIN (knows k1
    JOIN (knows k2
        JOIN (person p2
            JOIN (message
                JOIN (message_tag
                    JOIN tag ON mt_tagid=t_tagid)
                ON m_messageid=mt_messageid)
            ON p2.p_personid=m_creatorid)
        ON k2.k_person2id=p2.p_personid)
    ON k1.k_person2id=k2.k_person1id)
    ON p1.p_personid=k1.k_person1id
where p1.id=13194139539415
  and m_c_replyof IS NULL -- post, not comment
  and m_creationdate >= '2011-02-01 00:00:00'
  and m_creationdate < '2011-03-01 00:00:00'
;
