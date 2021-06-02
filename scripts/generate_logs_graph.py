import os
import sys

import duckdb_query_graph

if len(sys.argv) <= 1:
    print("Usage: python generate_logs_graph.py <input.log> <output_dir>")
    exit(1)

input_file = sys.argv[1]
output_dir = sys.argv[2]

def generate_query_graph(output_dir_, last_benchmark_):
    html_input = os.path.join(output_dir_, last_benchmark_ + ".json")
    html_output = os.path.join(output_dir_, last_benchmark_ + ".html")
    duckdb_query_graph.generate(html_input, html_output)
    with open(html_output, 'r') as html:
        text = html.read()
    # inline javascript files
    javascript_base = os.path.join('tools', 'pythonpkg', 'duckdb_query_graph')
    with open(os.path.join(javascript_base, 'raphael.js'), 'r') as f:
        raphael = f.read()
    with open(os.path.join(javascript_base, 'treant.js'), 'r') as f:
        treant = f.read()
    text = text.replace('<script src="../../raphael.js"></script>', '<script>' + raphael + '</script>')
    text = text.replace('<script src="../../treant.js"></script>', '<script>' + treant + '</script>')
    with open(html_output, 'w+') as html:
        html.write(text)

with open(input_file, 'r') as input:
    lines = input.readlines()
    current_benchmark = ""
    last_benchmark = ""
    output = open(os.path.join(output_dir, "empty.json"), 'w+')
    init = True
    for line in lines:
        if line.startswith('-'):
            continue
        elif line.startswith("||"):
            last_benchmark = current_benchmark
            current_benchmark = line.replace("||", "").strip().lower()  # get benchmark name
            if not init:  # generate and write the last html file
                generate_query_graph(output_dir, last_benchmark)
            output = open(os.path.join(output_dir, current_benchmark + ".json"), 'w+')  # open new file
            init = False
        else:
            output.write(line)  # write query log to file
            output.flush()
    generate_query_graph(output_dir, current_benchmark)
    output.close()
