/* interactive-short-4 */
select COALESCE(m_ps_imagefile, '') || COALESCE(m_content, ''), m_creationdate
from message
where m_messageid = 1236950581248;
