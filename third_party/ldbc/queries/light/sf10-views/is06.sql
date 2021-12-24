select f_forumid, f_title, p_personid, p_firstname, p_lastname
from m1m2
         JOIN pf ON m2_ps_forumid = f_forumid
where m1_id = 4947802324993;
