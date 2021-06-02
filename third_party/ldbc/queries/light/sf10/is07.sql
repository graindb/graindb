select m2.c_id, m2.c_content, m2.c_creationdate, p_personid, p_firstname, p_lastname
from comment m1, comment m2, person
where
  m1.id = 8246337208329 and m2.c_replyof_comment = m1.c_id and m2.c_creatorid = p_personid;
