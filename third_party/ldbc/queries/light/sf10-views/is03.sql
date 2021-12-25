select p2.p_personid, p2.p_firstname, p2.p_lastname, p1k.k_creationdate
from p1k
         JOIN person p2 ON p1k.k_person2id = p2.p_personid
where p1k.p1_id = 933;
