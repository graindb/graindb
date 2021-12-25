create table p1c as (
    select p1.id as p1_id, c_content, c_id
    from person p1,
         comment
    where c_creatorid = p1.p_personid
);

create
index p1c_idx on p1c(p1_id);

create table p2lc as (
    select p2.p_personid  as p2_p_personid,
           p2.p_firstname as p2_p_firstname,
           p2.p_lastname  as p2_p_lastname,
           l_creationdate,
           l_messageid
    from person p2,
         likes_comment
    where p2.p_personid = l_personid
);
