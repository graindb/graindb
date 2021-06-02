/* interactive-complex-8 */
select m1.m_creatorid, p1.p_firstname, p1.p_lastname, m1.m_creationdate, m1.m_messageid, m1.m_content
  from message m1, message m2, person p1, person p2
  where
      p1.p_personid = m1.m_creatorid and
      m1.m_c_replyof = m2.m_messageid and
      m2.m_creatorid = p2.p_personid and
      p2.p_personid = 30786325584797
;
