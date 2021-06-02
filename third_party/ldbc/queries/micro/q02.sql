SELECT count(*)
FROM post ps
         JOIN forum f ON ps.ps_forumid = f.f_forumid
WHERE ps.ps_id >= 0
  AND f.rowid <=
