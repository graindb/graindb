import csv
import os

source_dir = "/home/g35jin/ldbc_dataset/sf30/"
# source_dir = "/Users/guodong/Developer/graindb/scripts/"
person_input_path = os.path.join(source_dir, 'person_temp.csv')
person_output_path = os.path.join(source_dir, 'person_names.csv')

with open(person_input_path) as csv_file:
    csv_reader = csv.reader(csv_file, delimiter='|')
    next(csv_reader)  # skip header
    person_output = open(person_output_path, "w+")
    csv_writer = csv.writer(person_output, delimiter='|')
    csv_writer.writerow(["name", "gender", "age", "creationDate", "locationIP", "browserUsed", "place"])
    for row in csv_reader:
        csv_writer.writerow(
            [row[1], row[2], row[3], row[4], row[5], row[6], row[7]])
    person_output.close()
