import matplotlib.pyplot as plt

# if len(sys.argv) != 2:
#     print("USAGE: python3 scripts/extract_query_latency.py SOURCE_FILE")
#     sys.exit()

list1_1 = []
list1_2 = []
list1_3 = []
list1_4 = []
x_axis = []
data1 = []

# source_file = str(sys.argv[1])
source_file = "/Users/guodong/Developer/graindb-benchmark/micro-selectivity-q09.csv"
# source_file2 = "/Users/guodong/Developer/graindb-benchmark/micro-selectivity-q09.csv"

with open(source_file, 'r') as source:
    rows = source.readlines()
    titles = rows[0].split(',')
    row_id = 1
    while row_id < len(rows):
        line = rows[row_id].split('\n')[0]
        line_parts = line.split(',')
        x_axis.append(line_parts[0])
        list1_1.append(float(line_parts[1]))
        list1_2.append(float(line_parts[2]))
        list1_3.append(float(line_parts[3]))
        list1_4.append(float(line_parts[4]))
        row_id = row_id + 1
    data1.append(list1_1)
    data1.append(list1_2)
    data1.append(list1_3)
    data1.append(list1_4)
    # v1 = np.array(list1)
    # v2 = np.array(list2)
    # v3 = np.array(list3)
    # data = np.array(v1, v2 , v3)

figure, axes = plt.subplots()
plt.yscale('log')
# plt.xscale('log')
plt.ylabel('runtime in msec (log scale)', fontsize=16)
plt.xlabel('selectivity of Knows', fontsize=16)
axes.plot(x_axis, list1_1, 'D-', label='DuckDB', alpha=0.9, markersize=5)
axes.plot(x_axis, list1_2, 'o-', label='GRainDB', alpha=0.6, markersize=5)
axes.plot(x_axis, list1_3, '*-', label='GrahflowDB', alpha=0.8, markersize=5)
axes.plot(x_axis, list1_4, 's-', label='Neo4j', alpha=0.8, markersize=5)

axes.tick_params(axis='both', labelsize=14)

# axes.scatter(x_axis, list1, color='green', label='DuckDB')
# axes.scatter(x_axis, list2, color='orange', label='GRainDB')
# axes.scatter(x_axis, list3, color='yellow', label='GrahflowDB')
# axes.scatter(x_axis, list4, color='blue', label='Neo4j')
plt.xticks(rotation=45)
plt.tight_layout()
plt.legend(prop={'size': 14})
plt.show()
