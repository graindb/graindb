select p2_p_personid, p2_p_firstname, p2_p_lastname, l_creationdate, c_content
from p1c
         JOIN p2lc ON c_id = l_messageid
where p1_id = 933;
