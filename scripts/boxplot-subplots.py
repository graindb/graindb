import matplotlib.pyplot as plt

list1_1 = []
list1_2 = []

list2_1 = []
list2_2 = []
list2_3 = []

list3_1 = []
list3_2 = []

data1 = []
data2 = []
data3 = []

# source_file = str(sys.argv[1])
source_file1 = "/Users/guodong/Developer/graindb-benchmark/job_113_runtime_duck_grain.csv"
source_file2 = "/Users/guodong/Developer/graindb-benchmark/ldbc_light_numbers.csv"
source_file3 = "/Users/guodong/Developer/graindb-benchmark/tpch_sf10.csv"

px = 1 / plt.rcParams['figure.dpi']
with open(source_file1, 'r') as source:
    rows = source.readlines()
    titles = rows[0].split(',')
    row_id = 1
    while row_id < len(rows):
        line = rows[row_id].split('\n')[0]
        line_parts = line.split(',')
        list1_1.append(float(line_parts[0]))
        list1_2.append(float(line_parts[1]))
        row_id = row_id + 1
    data1.append(list1_1)
    data1.append(list1_2)

with open(source_file2, 'r') as source:
    rows = source.readlines()
    titles = rows[0].split(',')
    row_id = 1
    while row_id < len(rows):
        line = rows[row_id].split('\n')[0]
        line_parts = line.split(',')
        list2_1.append(float(line_parts[0]))
        list2_2.append(float(line_parts[1]))
        list2_3.append(float(line_parts[2]))
        row_id = row_id + 1
    data2.append(list2_1)
    data2.append(list2_2)
    data2.append(list2_3)

with open(source_file3, 'r') as source:
    rows = source.readlines()
    titles = rows[0].split(',')
    row_id = 1
    while row_id < len(rows):
        line = rows[row_id].split('\n')[0]
        line_parts = line.split(',')
        list3_1.append(float(line_parts[0]))
        list3_2.append(float(line_parts[1]))
        row_id = row_id + 1
    data3.append(list3_1)
    data3.append(list3_2)

figure, (axes1, axes2, axes3) = plt.subplots(1, 3)
# plt.yscale('log')
# plt.ylabel('runtime in msec (log scale)')
axes1.set_ylabel('runtime in msec (log scale)', fontsize=16)
axes1.set_title('JOB', fontsize=16)
axes1.set_yscale('log')
axes1.tick_params(axis='both', labelsize=14)
axes1.boxplot(data1, patch_artist=True, labels=['DUCKDB', 'GRAINDB'], showmeans=False, widths=0.2)

axes2.set_title('SNB-M', fontsize=16)
axes2.set_yscale('log')
axes2.tick_params(axis='both', labelsize=14)
axes2.boxplot(data2, patch_artist=True, labels=['DUCKDB', 'GRAINDB', 'GFDB'], showmeans=False, widths=0.3)

axes3.set_title('TPC-H', fontsize=16)
axes3.set_yscale('log')
axes3.tick_params(axis='both', labelsize=14)
axes3.boxplot(data3, patch_artist=True, labels=['DUCKDB', 'GRAINDB'], showmeans=False, widths=0.2)
plt.tight_layout()
plt.show()
