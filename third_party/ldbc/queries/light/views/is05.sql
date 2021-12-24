create table cp as (
    select p_personid, p_firstname, p_lastname, comment.id as comment_id
    from comment,
         person
    where c_creatorid = p_personid
);

create
index cp_idx on cp(comment_id);
