create table pknows as (
    select p1.id as p1_id, k_person2id
    from person p1,
         knows
    where p1.p_personid = k_person1id
);

create table ps_tag_ps as (
    select ps_creationdate, mt_tagid, ps_creatorid
    from post_tag,
         post
    where ps_id = mt_messageid
);
