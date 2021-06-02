select p2.id, p2.p_firstname, p2.p_lastname, p2.p_gender, pl.pl_name
from
person p1 JOIN (
        knows k1 JOIN (
            knows k2 JOIN (
                person p2 JOIN place pl ON p2.p_placeid=pl.pl_placeid)
            ON k2.k_person2id=p2.p_personid)
        ON k1.k_person2id=k2.k_person1id)
    ON p1.p_personid=k1.k_person1id
where p1.id = 21990232569319
;