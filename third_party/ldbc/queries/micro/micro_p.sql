SELECT p.id, k.k_creationdate
FROM person p
         JOIN knows k ON k.k_person1id = p.p_personid
WHERE k.k_creationdate >= 0
  AND p.rowid <=
