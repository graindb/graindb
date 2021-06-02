select p_firstname, p_lastname, p_birthday, p_locationip, p_browserused, pl_placeid, p_gender,  p_creationdate
from person, place
where pid = 8796093028953 and person.p_placeid=place.pl_placeid;
