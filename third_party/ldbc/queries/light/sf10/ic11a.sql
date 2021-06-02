/* interactive-complex-11 */
select p2.id, p2.p_firstname, p2.p_lastname, o.o_name, pc.pc_workfrom
from person p1 JOIN (
    knows k1 JOIN (
            person p2 JOIN (
                person_company pc JOIN (
                    organisation o JOIN place pl ON o.o_placeid=pl.pl_placeid)
                    ON pc.pc_organisationid=o.o_organisationid)
                ON p2.p_personid=pc.pc_personid)
            ON k1.k_person2id=p2.p_personid)
    ON p1.p_personid=k1.k_person1id
where p1.id=933
    and pc_workfrom < 2016
    and pl_name = 'China'
;
