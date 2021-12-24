select f_title
from p1k1
         JOIN (person p2 JOIN (fpf JOIN post m ON fpf.f_forumid = m.ps_forumid) ON p2.p_personid = fpf.fp_personid)
              ON p1k1.k_person2id = p2.p_personid
where p1k1.p1_id = 933
  and fpf.fp_joindate >= 1353819600;
