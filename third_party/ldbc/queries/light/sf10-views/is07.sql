select m2_c_id, m2_c_content, m2_c_creationdate, p_personid, p_firstname, p_lastname
from comment m1
         JOIN pm2 ON pm2.m2_c_replyof_comment = m1.c_id
where m1.id = 8246337208329;
