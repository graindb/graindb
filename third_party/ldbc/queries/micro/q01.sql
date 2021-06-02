SELECT p.id,
       p.p_gender,
       p.p_creationdate,
       p.p_browserused,
       m.id,
       m.c_creationdate,
       m.c_browserused,
       m.c_length
FROM person p
         JOIN comment m ON p.p_personid = m.c_creatorid
WHERE p.rowid <=
