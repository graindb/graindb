select p2_id,
       p2_p_lastname,
       p2_p_birthday,
       p2_p_creationdate,
       p2_p_gender,
       p2_p_browserused,
       p2_p_locationip,
       pl_pl_name
from p2pl
         JOIN p1k ON k_person2id = p2_p_personid
where p1_id = 933
  and p2_p_firstname = 'Rahul';
