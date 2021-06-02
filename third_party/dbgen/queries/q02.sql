select
 s_acctbal,
 s_name,
 n_name,
 p_partkey,
 p_mfgr,
 s_address,
 s_phone,
 s_comment
from
 part,
 supplier,
 partsupp,
 nation
where
 p_partkey = ps_partkey
 and s_suppkey = ps_suppkey
 and p_size = 15
 and p_type like '%BRASS'
 and s_nationkey = n_nationkey
 and n_regionkey = 3
 and ps_supplycost = (
 select
 min(ps_supplycost)
 from
 partsupp,
 supplier,
 nation
 where
 p_partkey = ps_partkey
 and s_suppkey = ps_suppkey
 and s_nationkey = n_nationkey
 and n_regionkey = 3
 )
order by
 s_acctbal desc,
 n_name,
 s_name,
 p_partkey
limit 100;
