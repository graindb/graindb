/* bi-18 */
WITH RECURSIVE message_all AS (
    SELECT m_messageid       AS ma_postid
         , m_ps_language     AS ma_language
         , m_creatorid       AS ma_creatorid
         , 'Post'            AS ma_posttype
         , m_content IS NULL AS ma_content_isempty
         , m_length          AS ma_length
         , m_creationdate    AS ma_creationday--date_trunc('day', m_creationdate) AS ma_creationday
    FROM message
    WHERE 1 = 1
      AND m_c_replyof IS NULL -- post, not comment
    UNION ALL
    SELECT m_messageid
         , ma.ma_language
         , m_creatorid
         , 'Comment'
         , m_content IS NULL AS ma_content_isempty
         , m_length
         , m_creationdate --date_trunc('day', m_creationdate) AS ma_creationday
    FROM message p,
         message_all ma
    WHERE 1 = 1
      AND p.m_c_replyof = ma.ma_postid
)
   , person_w_posts AS (
    SELECT p.p_personid, count(ma_postid) as messageCount
    FROM person p
             left join message_all ma on (
            1 = 1
            AND p.p_personid = ma.ma_creatorid
            AND NOT ma.ma_content_isempty
            AND ma_length < 20
            AND ma_creationday > epoch_ms(1321333200000)
            AND ma_language = ANY ('ar')
        )
    GROUP BY p.p_personid
)
   , message_count_distribution AS (
    SELECT pp.messageCount, count(*) as personCount
    FROM person_w_posts pp
    GROUP BY pp.messageCount
    ORDER BY personCount DESC, messageCount DESC
)
SELECT *
FROM message_count_distribution
;
