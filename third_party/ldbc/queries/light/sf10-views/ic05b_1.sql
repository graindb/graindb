select f.f_title
from p1k1
         JOIN (knows k2 JOIN (person p2
    JOIN (forum_person fp
        JOIN (forum f
            JOIN post m ON f.f_forumid = m.ps_forumid)
        ON fp.fp_forumid = f.f_forumid)
    ON p2.p_personid = fp.fp_personid) ON k2.k_person2id = p2.p_personid)
              ON p1k1.k_person2id = k2.k_person1id
where p1k1.p1_id = 933
  and fp.fp_joindate >= 1353819600;
