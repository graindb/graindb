select p2.p_personid, p2.p_firstname, p2.p_lastname, l_creationdate, c_content
from p1c
         JOIN (person p2 JOIN likes_comment ON p2.p_personid = l_personid) ON c_id = l_messageid
where p1_id = 933;
