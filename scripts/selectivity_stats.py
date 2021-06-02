import csv

source_file = "/Users/guodong/Developer/GRBench/stats/ldbc/ldbc_pslikes.csv"
target_file = "/Users/guodong/Developer/GRBench/stats/ldbc/ldbc_pslikes_accum.csv"
target = open(target_file, 'w+')

with open(source_file, 'r') as source:
    rows = source.readlines()
    row_id = 0
    current_num = 0
    while row_id < len(rows):
        row = rows[row_id]
        rid = row.split(',')[0]
        num = row.split(',')[1]
        current_num += int(num)
        per = float(current_num)/44582924.0
        target.write(rid + "," + str(current_num) + ", " + str(per))
        target.write("\n")
        row_id = row_id + 1

    target.close()
