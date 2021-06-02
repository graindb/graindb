SELECT p.id,
       p.p_gender,
       p.p_creationdate,
       p.p_browserused,
       c.id,
       c.c_creationdate,
       c.c_browserused,
       c.c_length
FROM person p
         JOIN (likes_comment l JOIN comment c ON l_messageid = c.c_id) ON p.p_personid = l.l_personid
WHERE p.rowid <=
