create table p2k as (
    select p2.p_personid  as p2_p_personid,
           p2.p_firstname as p2_p_firstname,
           p2.p_lastname  as p2_p_lastname,
           k.k_person1id  as k_person1id,
           k_creationdate
    from person p2,
         knows k
    where p2.p_personid = k.k_person2id
);

create
index person_idx on person(id);
