/* interactive-complex-2 */
select p2.id, p2.p_firstname, p2.p_lastname, c_id, c_content, c_creationdate
from person p1 JOIN (
    knows JOIN (person p2 JOIN comment ON p2.p_personid=c_creatorid) ON knows.k_person2id=p2.p_personid) ON p1.p_personid=k_person1id 
where c_creationdate < 1338552000
  and p1.id = 933
;
