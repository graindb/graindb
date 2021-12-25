select p2.id, p2.p_firstname, p2.p_lastname, c_id, c_content, c_creationdate
from p1k
         JOIN (person p2 JOIN comment ON p2.p_personid = c_creatorid) ON k_person2id = p2.p_personid
where c_creationdate < 1338552000
  and p1_id = 933;
