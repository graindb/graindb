select m1p1.m1_c_id,
       m1p1.m1_c_creationdate,
       m2p2.m2_ps_id,
       m2p2.p2_p_personid,
       m2p2.p2_p_firstname,
       m2p2.p2_p_lastname
from m1p1
         JOIN m2p2 ON m1p1.m1_c_replyof_post = m2p2.m2_ps_id
where m1p1.p1_id = 933;
