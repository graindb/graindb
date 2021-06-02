/* interactive-complex-6 */
select t2.t_name
from person p1 JOIN 
(knows k1 JOIN (knows k2 JOIN (person p2 JOIN (post m JOIN (post_tag mt1 JOIN tag t1 ON mt1.mt_tagid = t1.t_tagid) ON m.ps_id = mt1.mt_messageid JOIN (post_tag mt2 JOIN tag t2 ON mt2.mt_tagid = t2.t_tagid ) ON m.ps_id = mt2.mt_messageid) ON m.ps_creatorid = p2.p_personid) ON k2.k_person2id=p2.p_personid) ON k1.k_person2id=k2.k_person1id) ON p1.p_personid=k1.k_person1id
where p1.id=933 and t1.t_name = 'Rumi' and t2.t_name != 'Rumi';
