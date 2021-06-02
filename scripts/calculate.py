import csv

source_file = "/Users/guodong/Downloads/grain_rj_evaluation.csv"
target_file = "/Users/guodong/Downloads/grain_rj_avg.csv"
target = open(target_file, 'w')

with open(source_file, 'r') as source:
    source_csv = csv.reader(source_file, delimiter=',')
    hj = []
    rid = []
    zone = []
    rows = []
    bid = []
    qid = 0
    rowid = 0
    for row in source_csv:
        if (rowid == 5):
            target.write(row[0] + ",")
            target.write(row[1] + ",")
        hj.append(float(row[2].strip()))
        rid.append(float(row[3].strip()))
        zone.append(float(row[4].strip()))
        rows.append(float(row[5].strip()))
        bid.append(float(row[6].strip()))
        rowid = rowid + 1
        target.write(row)

row_id = 0
sum_t = 0.0
for line in lines:
    if (line.strip() == 'TIMEOUT'):
        print("TIMEOUT")
        row_id = 0
        sum_t = 0.0
        continue
    sum_t += (float(line.strip()))
    row_id += 1
    if (row_id == 5):
        print(sum_t / 5)
        sum_t = 0.0
        row_id = 0

