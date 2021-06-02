/* bi-20 */
WITH RECURSIVE tagclass_tree AS (
    SELECT tc.tc_tagclassid AS root_tagclassid
         , tc.tc_name       AS root_tagclassname
         , tc.tc_tagclassid AS tagclassid
    FROM tagclass tc
    WHERE 1 = 1
      AND tc.tc_name = ANY ('{Writer, Single, Country}')
    UNION ALL
    SELECT tt.root_tagclassid
         , tt.root_tagclassname
         , tc.tc_tagclassid AS tagclassid
    FROM tagclass tc
       , tagclass_tree tt
    WHERE 1 = 1
      -- join
      AND tt.tagclassid = tc.tc_subclassoftagclassid
)
SELECT tt.root_tagclassname            AS "tagClass.name"
     , count(DISTINCT mt.mt_messageid) AS messageCount
FROM tagclass_tree tt
         INNER JOIN tag t ON (tt.tagclassid = t.t_tagclassid)
         LEFT JOIN message_tag mt ON (t.t_tagid = mt.mt_tagid)
WHERE 1 = 1
GROUP BY tt.root_tagclassname
ORDER BY messageCount DESC, tt.root_tagclassname LIMIT 100
;
