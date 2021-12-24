select p2_id, p2_p_firstname, p2_p_lastname, p2_p_gender, pl_pl_name
from p1k1
         JOIN (knows k2 JOIN p2pl ON k2.k_person2id = p2pl.p2_p_personid) ON p1k1.k_person2id = k2.k_person1id
where p1_id = 933;
