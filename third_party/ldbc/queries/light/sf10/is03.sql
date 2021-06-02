select p2.p_personid, p2.p_firstname, p2.p_lastname, k_creationdate
from knows, person p1, person p2
where p1.id=933 and p1.p_personid=k_person1id and k_person2id=p2.p_personid;
