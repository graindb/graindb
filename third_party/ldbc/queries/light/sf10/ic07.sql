/* interactive-complex-7 */
select p2.p_personid, p2.p_firstname, p2.p_lastname, l_creationdate, c_content
from person p1 JOIN (comment JOIN (likes_comment JOIN person p2 ON p2.p_personid = l_personid) ON c_id = l_messageid) ON c_creatorid = p1.p_personid
where p1.id=933
;
