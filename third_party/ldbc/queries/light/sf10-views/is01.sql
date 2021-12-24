select p_firstname,
       p_lastname,
       p_birthday,
       p_locationip,
       p_browserused,
       pl_placeid,
       p_gender,
       p_creationdate
from person_place
where p_personid = 933;
