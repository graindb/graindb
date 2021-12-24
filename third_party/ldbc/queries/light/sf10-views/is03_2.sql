select p2k.p2_p_personid, p2k.p2_p_firstname, p2k.p2_p_lastname, p2k.k_creationdate
from person p1
         JOIN p2k ON p1.p_personid = p2k.k_person1id
where p1.id = 933;
