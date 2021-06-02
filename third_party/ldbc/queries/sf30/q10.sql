/* interactive-complex-3 */
select p_personid, p_firstname, p_lastname, ct1, ct2, total
from
 ( select k_person2id
   from knows
   where
   k_person1id = 21990232558393
   union
   select k2.k_person2id
   from knows k1, knows k2
   where
   k1.k_person1id = 21990232558393 and k1.k_person2id = k2.k_person1id and k2.k_person2id <> 21990232558393
 ) f,  person, place p1, place p2,
 (
  select chn.m_c_creatorid, ct1, ct2, ct1 + ct2 as total
  from
   (
      select m_creatorid as m_c_creatorid, count(*) as ct1 from message, place
      where
        m_locationid = pl_placeid and pl_name = 'Honduras' and
        m_creationdate >= '2010-12-01 08:00:00' and  m_creationdate < '2010-12-31 08:00:00'
      group by m_c_creatorid
   ) chn,
   (
      select m_creatorid as m_c_creatorid, count(*) as ct2 from message, place
      where
        m_locationid = pl_placeid and pl_name = 'Norway' and
        m_creationdate >= '2010-12-01 08:00:00' and  m_creationdate < '2010-12-31 08:00:00'
      group by m_creatorid
   ) ind
  where CHN.m_c_creatorid = IND.m_c_creatorid
 ) cpc
where
f.k_person2id = p_personid and p_placeid = p1.pl_placeid and
p1.pl_containerplaceid = p2.pl_placeid and p2.pl_name <> 'Honduras' and p2.pl_name <> 'Norway' and
f.k_person2id = cpc.m_c_creatorid
order by 6 desc, 1
limit 20;
