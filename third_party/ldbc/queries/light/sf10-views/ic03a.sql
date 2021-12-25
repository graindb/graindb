select p2.id, p2.p_firstname, p2.p_lastname
from p1k1
         JOIN (person p2 JOIN mpl m1pl1 ON m1pl1.c_creatorid = p2.p_personid JOIN mpl m2pl2 ON m2pl2.c_creatorid = p2.p_personid)
              ON p1k1.k_person2id = p2.p_personid
where p1_id = 933
  and m1pl1.pl_name = 'India'
  and m2pl2.pl_name = 'China'
  and m1pl1.c_creationdate >= 1313591219
  and m1pl1.c_creationdate
    < 1513591219
  and m2pl2.c_creationdate >= 1313591219
  and m2pl2.c_creationdate
    < 1513591219;
