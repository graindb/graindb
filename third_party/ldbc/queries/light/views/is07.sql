create table pm2 as (
    select m2.c_id              as m2_c_id,
           m2.c_content         as m2_c_content,
           m2.c_creationdate    as m2_c_creationdate,
           m2.c_replyof_comment as m2_c_replyof_comment,
           p_personid,
           p_firstname,
           p_lastname
    from comment m2,
         person
    where m2.c_creatorid = p_personid
);

create
index comment_idx on comment(id);
