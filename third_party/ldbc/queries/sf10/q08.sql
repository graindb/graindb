/* interactive-complex-1 */
select id,
       p_lastname,
       min(dist) as dist,
       p_birthday,
       p_creationdate,
       p_gender,
       p_browserused,
       p_locationip,
       p1.pl_name
from (
         select k_person2id as id, 1 as dist
         from knows,
              person
         where k_person1id = 8796093028953
           and p_personid = k_person2id
           and p_firstname = 'John'
         union all
         select b.k_person2id as id, 2 as dist
         from knows a,
              knows b,
              person
         where a.k_person1id = 8796093028953
           and b.k_person1id = a.k_person2id
           and p_personid = b.k_person2id
           and p_firstname = 'John'
           and p_personid != 8796093028953
         union all
         select c.k_person2id as id, 3 as dist
         from knows a, knows b, knows c, person
         where
             a.k_person1id = 8796093028953
           and b.k_person1id = a.k_person2id
           and b.k_person2id = c.k_person1id
           and p_personid = c.k_person2id
           and p_firstname = 'John'
           and p_personid != 8796093028953
     ) tmp,
     person,
     place p1
where p_personid = id
  and p_placeid = p1.pl_placeid
group by id, p_lastname, p_birthday, p_creationdate, p_gender, p_browserused, p_locationip, p1.pl_name
order by dist, p_lastname, id LIMIT 20
;
