create table p1k1 as (
    select p1.id as p1_id, k_person2id
    from person p1,
         knows
    where p1.p_personid = k_person1id
);

create table ps_ps_tag as (
    select mt_tagid, mt_messageid, ps_id
    from post_tag,
         post
    where ps_id = mt_messageid
);

create
index p1k1_idx on p1k1(p1_id);
