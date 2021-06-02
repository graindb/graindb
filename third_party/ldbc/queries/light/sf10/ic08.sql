/* interactive-complex-8 */
select c_creatorid, p2.p_firstname, p2.p_lastname, c_creationdate, c_id, c_content
 from person p1 JOIN (post JOIN (comment JOIN person p2 ON c_creatorid = p2.p_personid) ON c_replyof_post = ps_id) ON p1.p_personid = ps_creatorid
  where p1.p_personid=933
;
