import os
import sys
sys.path.insert(0, 'benchmark')

import duckdb_query_graph

if len(sys.argv) <= 1:
    print("Usage: python generate_querygraph.py [input_dir] [output_dir]")
    exit(1)

input_logs_dir = sys.argv[1]
if len(sys.argv) <= 2:
    output_logs_dir = input_logs_dir
else:
    output_logs_dir = sys.argv[2]

for input_log in os.listdir(input_logs_dir):
    if ".json" in input_log:
        output_log = os.path.join(output_logs_dir, input_log.replace(".json", ".html"))
        input_log = os.path.join(input_logs_dir, input_log)

        duckdb_query_graph.generate(input_log, output_log)
        with open(output_log, "r") as f:
            text = f.read()
        #inline javascript files
        javascript_base = os.path.join('tools', 'pythonpkg', 'duckdb_query_graph')
        with open(os.path.join(javascript_base, 'raphael.js'), 'r') as f:
            raphael = f.read()

        with open(os.path.join(javascript_base, 'treant.js'), 'r') as f:
            treant = f.read()

        text = text.replace('<script src="../../raphael.js"></script>', '<script>' + raphael + '</script>')
        text = text.replace('<script src="../../treant.js"></script>', '<script>' + treant + '</script>')

        with open(output_log, 'w+') as f:
            f.write(text)
