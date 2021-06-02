select p_personid, p_firstname, p_lastname
from comment, person
where comment.id = 4947802324993 and c_creatorid = p_personid;
