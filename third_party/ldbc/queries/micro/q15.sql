SELECT MIN(p2.p_firstname) FROM person p1 JOIN (knows k JOIN person p2 ON k.k_person2id=p2.p_personid) ON p1.p_personid=k.k_person1id
WHERE p1.rowid<=110673;
