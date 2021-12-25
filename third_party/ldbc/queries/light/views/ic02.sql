create table p1k as (
    select p1.id as p1_id, k_person2id
    from person p1,
         knows
    where p1.p_personid = k_person1id
);

create
index p1k_idx on p1k(p1_id);
