select p2_id, p2_p_firstname, p2_p_lastname, c_id, c_content, c_creationdate
from p1k
         JOIN p2c ON k_person2id = p2_p_personid
where c_creationdate < 1338552000
  and p1_id = 933;
