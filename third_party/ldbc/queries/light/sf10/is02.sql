select m1.c_id, m1.c_creationdate,
       m2.ps_id, p2.p_personid, p2.p_firstname, p2.p_lastname
from person p1 JOIN (comment m1 JOIN (post m2 JOIN person p2 ON m2.ps_creatorid=p2.p_personid) ON m1.c_replyof_post = m2.ps_id) ON m1.c_creatorid=p1.p_personid 
where p1.id=933;
