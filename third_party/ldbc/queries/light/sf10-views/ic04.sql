select t_name
from tag
         JOIN (ps_tag_ps JOIN (person p2 JOIN (knows k2 JOIN pknows p1k1 ON k2.k_person1id = p1k1.p1_id)
    ON k2.k_person2id = p2.p_personid) ON p2.p_personid = ps_creatorid)
              ON mt_tagid = t_tagid
where p1k1.p1_id = 933
  and ps_creationdate >= 1313591219
  and ps_creationdate
    < 1513591219;
