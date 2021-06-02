/* interactive-complex-13 */
WITH RECURSIVE search_graph AS (
    SELECT 26388279067527::bigint AS link, 0 AS depth
           --, ARRAY[26388279067527::bigint]::bigint[] AS path
    UNION ALL
    (WITH sg as (select link, depth from search_graph) -- Note: sg is only the diff produced in the previous iteration
     SELECT distinct k_person2id,
                     x.depth + 1
                     --, array_append(path, k_person2id)
     FROM knows,
          sg x
     WHERE 1 = 1
       and x.link = k_person1id
       --and k_person2id <> ALL (path)
       -- stop if we have reached person2 in the previous iteration
       and not exists(select * from sg y where y.link = 13194139540791::bigint)
       -- skip reaching persons reached in the previous iteration
       and not exists(select * from sg y where y.link = k_person2id)
    )
)
select max(depth)
from (
         select depth
         from search_graph
         where link = 13194139540791::bigint
         union
         select -1) tmp;
