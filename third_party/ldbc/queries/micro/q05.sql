SELECT count(*)
FROM post ps
         JOIN forum f ON ps.ps_forumid = f.f_forumid
WHERE f.id >= 0
  AND ps.ps_forumid <=
