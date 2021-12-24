select m1p1.m1_c_id,
       m1p1.m1_c_creationdate,
       m2.ps_id,
       p2.p_personid,
       p2.p_firstname,
       p2.p_lastname
from m1p1
         JOIN (post m2 JOIN person p2 ON m2.ps_creatorid = p2.p_personid) ON m1p1.m1_c_replyof_post = m2.ps_id
where m1p1.p1_id = 933;
