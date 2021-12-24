select p2_id,
       p2_p_lastname,
       p2_p_birthday,
       p2_p_creationdate,
       p2_p_gender,
       p2_p_browserused,
       p2_p_locationip,
       pl_pl_name
from p1k
         JOIN (knows k2 JOIN (knows k3 JOIN p2pl ON k3.k_person2id = p2_p_personid) ON k2.k_person2id = k3.k_person1id)
              ON p1k.k_person2id = k2.k_person1id
where p1_id = 933
  and p2_p_firstname = 'Rahul';
