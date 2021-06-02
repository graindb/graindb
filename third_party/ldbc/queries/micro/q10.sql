SELECT p2.id, p2.p_firstname, p2.p_lastname, p2.p_birthday, k.k_creationdate
FROM person p1 JOIN (knows k JOIN person p2 ON k.k_person2id=p2.p_personid) ON p1.p_personid=k.k_person1id
WHERE p1.rowid<=129100;
