SELECT f.id, f.f_creationdate, p.id, p.p_gender, p.p_creationDate, p.p_browserused
FROM forum f
         JOIN (forum_person fp JOIN person p ON p.p_personid = fp.fp_personid) ON f.f_forumid = fp.fp_forumid
WHERE f.rowid <=
