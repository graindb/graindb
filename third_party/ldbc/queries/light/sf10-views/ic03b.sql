/* interactive-complex-3 */
select p2.id, p2.p_firstname, p2.p_lastname
from p1k1
         JOIN (knows k2 JOIN (person p2 JOIN (comment m1 JOIN place pl1 ON m1.c_locationid = pl1.pl_placeid)
    ON m1.c_creatorid = p2.p_personid JOIN (comment m2 JOIN place pl2 ON m2.c_locationid = pl2.pl_placeid)
    ON m2.c_creatorid = p2.p_personid ) ON k2.k_person2id = p2.p_personid) ON k2.k_person1id = p1k1.k_person2id
WHERE p1_id = 933
  AND pl1.pl_name = 'India' -- 'Norway'
  AND pl2.pl_name = 'China'
  AND m1.c_creationdate >= 1313591219
  AND m1.c_creationdate < 1513591219
  AND m2.c_creationdate >= 1313591219
  AND m2.c_creationdate < 1513591219
;
