select mt2t2.t_name
from pk p1k1
         JOIN (person p2 JOIN (post m JOIN mtt mt1t1 ON m.ps_id = mt1t1.mt_messageid JOIN mtt mt2t2 ON m.ps_id = mt2t2.mt_messageid) ON m.ps_creatorid = p2.p_personid)
              ON p1k1.k_person2id = p2.p_personid
where p1k1.p_id = 933
  and mt1t1.t_name = 'Rumi'
  and mt2t2.t_name != 'Rumi';
