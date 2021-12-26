import matplotlib.pyplot as plt
import sys

if len(sys.argv) != 2:
    print("USAGE: python3 scripts/plot_selectivity.py SOURCE_FILE[Selectivity, DuckDB, GRainDB, GFDB, Neo4j]")
    sys.exit()

list1_1 = []
list1_2 = []
list1_3 = []
list1_4 = []
x_axis = []
data1 = []

source_file = str(sys.argv[1])
# source_file = "/Users/g35jin/Developer/graindb-benchmark/micro-selectivity-q09.csv"

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

figure, axes = plt.subplots()
plt.yscale('log')
plt.ylabel('runtime in msec (log scale)', fontsize=16)
plt.xlabel('selectivity of Knows', fontsize=16)
axes.plot(x_axis, list1_1, 'D-', label='DuckDB', alpha=0.9, markersize=5)
axes.plot(x_axis, list1_2, 'o-', label='GRainDB', alpha=0.6, markersize=5)
axes.plot(x_axis, list1_3, '*-', label='GrahflowDB', alpha=0.8, markersize=5)
axes.plot(x_axis, list1_4, 's-', label='Neo4j', alpha=0.8, markersize=5)

axes.tick_params(axis='both', labelsize=14)

plt.xticks(rotation=45)
plt.tight_layout()
plt.legend(prop={'size': 14})
plt.show()
