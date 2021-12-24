create table person_place as
    (
        select p_personid,
               p_firstname,
               p_lastname,
               p_birthday,
               p_locationip,
               p_browserused,
               pl_placeid,
               p_gender,
               p_creationdate
        from person,
             place
        where person.p_placeid = place.pl_placeid);

create
index personid_idx on person_place(p_personid);
