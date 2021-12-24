create table p_post as (
    select ps_id, p1.p_personid as p1_p_personid
    from person p1,
         post
    where p1.p_personid = ps_creatorid
);

create
index p_post_idx on p_post(p1_p_personid);
