create table m1p1 as (
    select m1.c_id           as m1_c_id,
           m1.c_creationdate as m1_c_creationdate,
           m1.c_replyof_post as m1_c_replyof_post,
           p1.id             as p1_id
    from person p1,
         comment m1
    where m1.c_creatorid = p1.p_personid
);

create
index m1p1_idx on m1p1(p1_id);
