create table m1m2 as (
    select m1.id             as m1_id,
           m2.c_creatorid    as m2_c_creatorid,
           m2.c_id           as m2_c_id,
           m2.c_content      as m2_c_content,
           m2.c_creationdate as m2_c_creationdate
    from comment m1,
         comment m2
    where m2.c_replyof_comment = m1.c_id
);

create
index m1m2_idx on m1m2(m1_id);
