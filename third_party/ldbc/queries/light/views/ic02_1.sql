create table p1k as (
    select p1.id as p1_id, k_person2id
    from person p1,
         knows
    where p1.p_personid = k_person1id
);

create
index p1k_idx on p1k(p1_id);

create table p2c as (
    select p2.id          as p2_id,
           p2.p_personid  as p2_p_personid,
           p2.p_firstname as p2_p_firstname,
           p2.p_lastname  as p2_p_lastname,
           c_id,
           c_content,
           c_creationdate
    from person p2,
         comment
    where p2.p_personid = c_creatorid
);
