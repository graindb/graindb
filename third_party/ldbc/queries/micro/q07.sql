SELECT COUNT(*)
FROM person p,
     likes_comment l
WHERE p.p_personid = l.l_personid
  AND p.id >= 0
  AND l.l_creationdate <=
