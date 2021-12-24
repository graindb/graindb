create table p1k1 as (
    select p1.id as p1_id, k_person2id
    from person p1,
         knows
    where p1.p_personid = k_person1id
);

create
index p1k1_idx on p1k1(p1_id);

create table fpp as (
    select p_personid, fp_joindate, fp_forumid
    from forum_person fp
             JOIN person p ON fp.fp_personid = p.p_personid
);
