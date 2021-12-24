create table m1m2 as (
    select m1.id as m1_id, m2.ps_forumid as m2_ps_forumid
    from comment m1,
         post m2
    where m1.c_replyof_post = m2.ps_id
);

create table pf as (
    select f_forumid, f_title, p_personid, p_firstname, p_lastname
    from person,
         forum
    where f_moderatorid = p_personid
);

create
index m1m2_idx on m1m2(m1_id);
