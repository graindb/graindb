SELECT MIN(ps.ps_messageid) FROM person p JOIN (pslikes psl JOIN post ps ON psl.psl_messageid=ps.ps_messageid) ON p.p_personid=psl.psl_personid
WHERE p.rowid<=128986;
