create table p1k1 as (
    select k_person2id, p1.id as p1_id
    from person p1,
         knows k1
    where p1.p_personid = k1.k_person1id
);

create
index p1k1_idx on p1k1(p1_id);
