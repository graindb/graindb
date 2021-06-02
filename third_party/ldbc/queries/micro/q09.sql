SELECT p.id, k.k_creationdate
FROM person p
         JOIN knows k ON k.k_person1id = p.p_personid
WHERE p.rowid >= 0
  AND k.k_creationdate <=
