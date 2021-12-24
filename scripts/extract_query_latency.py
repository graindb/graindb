if len(sys.argv) != 3:
    print("USAGE: python3 scripts/extract_query_latency.py SOURCE_FILE AVERAGE_FILE")
    sys.exit()

source_file = str(sys.argv[1])
average_file = str(sys.argv[2])
average = open(average_file, 'w+')

with open(source_file, 'r') as source:
    rows = source.readlines()
    row_id = 0
    times = [0.0, 0.0, 0.0, 0.0, 0.0]
    while row_id < len(rows):
        row = rows[row_id]
        if row.startswith("||"):
            row_id += 1
            continue
        elif row.startswith("-"):
            row_id += 1
            continue
        elif row.startswith("\n"):
            row_id += 1
            if rows[row_id].startswith("TIMEOUT"):
                average.write("600000")
                average.write("\n")
                continue
            times[0] = float(rows[row_id + 0])
            times[1] = float(rows[row_id + 1])
            times[2] = float(rows[row_id + 2])
            times[3] = float(rows[row_id + 3])
            times[4] = float(rows[row_id + 4])
            avgv = (times[0] + times[1] + times[2] + times[3] + times[4]) / 5.0
            average.write(str(avgv))
            average.write("\n")
            row_id += 5
            continue
        else:
            row_id += 1
            continue

average.close()
