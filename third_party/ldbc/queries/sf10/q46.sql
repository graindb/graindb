/* bi-25 */
WITH RECURSIVE reply_scores AS (
    SELECT p.m_messageid AS r_threadid
         , p.m_creatorid AS r_orig_personid
         , c.m_creatorid AS r_reply_personid
         , c.m_messageid AS r_reply_messageid
         , 1.0           AS r_score
    FROM forum f
       , message p
       , message c -- comment
    WHERE 1 = 1
      -- join
      AND f.f_forumid = p.m_ps_forumid
      AND p.m_messageid = c.m_c_replyof
      -- filter
      AND f.f_creationdate BETWEEN epoch_ms(1288584000000) AND epoch_ms(1291179600000)
      AND p.m_c_replyof IS NULL -- post, not comment
    UNION ALL
    SELECT r.r_threadid       AS r_threadid
         , r.r_reply_personid AS r_orig_personid
         , c.m_creatorid      AS r_reply_personid
         , c.m_messageid      AS r_reply_messageid
         , 0.5                AS r_score
    FROM reply_scores r
       , message c
    WHERE 1 = 1
      -- join
      AND r.r_reply_messageid = c.m_c_replyof
)
   , person_pair_scores_directed AS (
    SELECT r_orig_personid  AS orig_personid
         , r_reply_personid AS reply_personid
         , sum(r_score)     AS score
    FROM reply_scores
    WHERE 1 = 1
      -- discard self replies from the score earned
      AND r_orig_personid
    != r_reply_personid
GROUP BY r_orig_personid, r_reply_personid
    )
        , person_pair_scores AS (
    -- note: this should already have both (A, B, score) and (B, A, score)
SELECT coalesce (s1.orig_personid, s2.reply_personid) AS person1id
        , coalesce (s1.reply_personid, s2.orig_personid) AS person2id
        , coalesce (s1.score, 0.0) + coalesce (s2.score, 0.0) AS score
FROM person_pair_scores_directed s1
    FULL JOIN person_pair_scores_directed s2
ON (s1.orig_personid = s2.reply_personid AND s1.reply_personid = s2.orig_personid)
    )
    , wknows AS (
    -- weighted knows
SELECT k_person1id
        , k_person2id
        , coalesce (score, 0.0) AS score
FROM knows k
    LEFT JOIN person_pair_scores pps
ON (k.k_person1id = pps.person1id AND k.k_person2id = pps.person2id)
    )
    , paths AS (
SELECT k_person1id AS startPerson
        , k_person2id AS endPerson
    --, ARRAY[k_person1id, k_person2id]::bigint[] AS path
        , score AS weight
        , 1 AS hopCount
        , max (CASE WHEN k_person2id = 15393162793286 THEN 1 ELSE 0 END) OVER () as person2Reached
FROM wknows
WHERE 1=1
  AND k_person1id = 35184372095998
UNION ALL
SELECT p.startPerson AS startPerson
        , k_person2id AS endPerson
    --, array_append(path, k_person2id) AS path
        , weight + score AS weight
        , hopCount + 1 AS hopCount
        , max (CASE WHEN k_person2id = 15393162793286 THEN 1 ELSE 0 END) OVER () as person2Reached
FROM paths p
        , wknows k
WHERE 1=1
    -- join
  AND p.endPerson = k.k_person1id
    --AND NOT p.path && ARRAY[k.k_person2id] -- person2id is not in the path
    -- stop condition
  AND p.person2Reached = 0
    )
SELECT --path,
       weight
FROM paths
WHERE 1 = 1
  AND endPerson = 15393162793286
ORDER BY weight DESC--, path
;
