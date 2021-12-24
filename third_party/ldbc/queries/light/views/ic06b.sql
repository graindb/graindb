create table pk as (
    select p.id as p_id, k_person2id
    from person p,
         knows
    where p.p_personid = k_person1id
);

create
index pk_idx on pk(p_id);
