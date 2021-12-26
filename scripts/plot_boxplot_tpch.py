import matplotlib.pyplot as plt
import sys

list1 = []
list2 = []

data = []

if len(sys.argv) != 2:
    print("USAGE: python3 scripts/plot_boxplot_tpch.py SOURCE_FILE[DuckDB, GRainDB]")
    sys.exit()

source_file = str(sys.argv[1])

px = 1 / plt.rcParams['figure.dpi']
with open(source_file, 'r') as source:
    rows = source.readlines()
    titles = rows[0].split(',')
    row_id = 1
    while row_id < len(rows):
        line = rows[row_id].split('\n')[0]
        line_parts = line.split(',')
        list1.append(float(line_parts[0]))
        list2.append(float(line_parts[1]))
        row_id = row_id + 1
    data.append(list1)
    data.append(list2)

figure, axes = plt.subplots()
axes.set_ylabel('runtime in msec (log scale)', fontsize=16)
axes.set_yscale('log')
axes.tick_params(axis='both', labelsize=14)
axes.boxplot(data, patch_artist=True, labels=['DUCKDB', 'GRAINDB'], showmeans=False)

plt.tight_layout()
plt.show()
