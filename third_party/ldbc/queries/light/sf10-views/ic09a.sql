select p2.p_firstname, p2.p_lastname, c_creationdate
from p1k1
         JOIN (person p2 JOIN comment ON p2.p_personid = c_creatorid) ON k_person2id = p2.p_personid
where p1_id = 933
  and c_creationdate < 1342840042;
