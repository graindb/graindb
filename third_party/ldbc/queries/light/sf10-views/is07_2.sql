select m2_c_id, m2_c_content, m2_c_creationdate, p_personid, p_firstname, p_lastname
from m1m2
         JOIN person ON m2_c_creatorid = p_personid
where m1_id = 8246337208329;
