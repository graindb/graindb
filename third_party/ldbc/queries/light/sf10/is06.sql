select f_forumid, f_title, p_personid, p_firstname, p_lastname
from comment m1, post m2, person, forum
where m1.id = 4947802324993 and m1.c_replyof_post = m2.ps_id
  and m2.ps_forumid = f_forumid and f_moderatorid = p_personid;
