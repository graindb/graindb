create table p1k1 as (
    select p1.id as p1_id, k_person2id
    from person p1,
         knows
    where p1.p_personid = k_person1id
);

create table fpf as (
    select f_title, f_forumid, fp_personid, fp_joindate
    from forum f,
         forum_person fp
    where fp.fp_forumid = f.f_forumid
);

create
index p1k1_idx on p1k1(p1_id);
