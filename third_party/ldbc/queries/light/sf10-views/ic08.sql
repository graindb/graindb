select c_creatorid, p2.p_firstname, p2.p_lastname, c_creationdate, c_id, c_content
from p_post
         JOIN (comment JOIN person p2 ON c_creatorid = p2.p_personid) ON c_replyof_post = ps_id
where p1_p_personid = 933;
