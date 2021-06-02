SELECT COUNT(*)
FROM person p,
     likes_comment l
WHERE p.p_personid = l.l_personid
  AND l.l_creationdate >= 0
  AND p.rowid <=
