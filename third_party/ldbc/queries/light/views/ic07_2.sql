create table p1c as (
    select p1.id as p1_id, c_content, c_id
    from person p1,
         comment
    where c_creatorid = p1.p_personid
);

create
index p1c_idx on p1c(p1_id);
