/* interactive-complex-4 */
select t_name
from knows k1 JOIN (person p1
    JOIN (knows k2 JOIN (person p2 JOIN (post JOIN (post_tag JOIN tag ON mt_tagid=t_tagid) ON ps_id=mt_messageid) ON p2.p_personid=ps_creatorid) ON k2.k_person2id=p2.p_personid) ON p1.p_personid=k2.k_person1id) ON p1.p_personid=k1.k_person1id 
where p1.id=933
  and ps_creationdate >= 1313591219
  and ps_creationdate < 1513591219
;
