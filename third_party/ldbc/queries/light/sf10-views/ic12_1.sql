SELECT friend.p_personid, friend.p_firstname, friend.p_lastname
FROM p1k1
         JOIN
     (person friend JOIN (comment JOIN (post JOIN
         (post_tag JOIN (tag JOIN (tagclass tc1 JOIN tagclass tc2 ON tc1.tc_subclassoftagclassid = tc2.tc_tagclassid)
             ON t_tagclassid = tc1.tc_tagclassid) ON mt_tagid = tag.t_tagid) ON ps_id = mt_messageid)
         ON c_replyof_post = ps_id) ON c_creatorid = friend.p_personid)
     ON p1k1.k_person2id = friend.p_personid
WHERE p1k1.p1_id = 933
  AND tc2.tc_name = 'Person';
