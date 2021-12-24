select f.f_title
from p1k1
         JOIN (knows k2 JOIN (fpp
    JOIN (forum f
        JOIN post m ON f.f_forumid = m.ps_forumid)
    ON fpp.fp_forumid = f.f_forumid)
    ON k2.k_person2id = fpp.p_personid)
              ON p1k1.k_person2id = k2.k_person1id
where p1k1.p1_id = 933
  and fpp.fp_joindate >= 1353819600;
