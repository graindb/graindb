create table p1k1 as (
    select k_person2id, p1.id as p1_id
    from person p1,
         knows k1
    where p1.p_personid = k1.k_person1id
);

create table p2pl as (
    select p2.id          as p2_id,
           p2.p_firstname as p2_p_firstname,
           p2.p_lastname  as p2_p_lastname,
           p2.p_gender    as p2_p_gender,
           p2.p_personid  as p2_p_personid,
           pl.pl_name     as pl_pl_name
    from person p2,
         place pl
    where p2.p_placeid = pl.pl_placeid
);
