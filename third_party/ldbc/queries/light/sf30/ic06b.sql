/* interactive-complex-6 */
select t2.t_name
from person p1, knows k1, knows k2, person p2, message m, tag t1, tag t2, message_tag mt1, message_tag mt2
where 
p1.id=2199023262559 and p1.p_personid=k1.k_person1id and
k1.k_person2id=k2.k_person1id and k2.k_person2id=p2.p_personid and
m.m_creatorid = p2.p_personid and 
m.m_c_replyof IS NULL and
m.m_messageid = mt1.mt_messageid and 
mt1.mt_tagid = t1.t_tagid and 
m.m_messageid = mt2.mt_messageid and 
mt2.mt_tagid = t2.t_tagid and 
t1.t_name = 'Mark_Twain' and t2.t_name != 'Mark_Twain';
