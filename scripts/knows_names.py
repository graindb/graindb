# concatenate first_name and last_name in person.csv
# convert timestamp string to unix timestamp

import csv
import os
import datetime

source_dir = "/home/g35jin/ldbc_dataset/sf30/"
# source_dir = "/Users/guodong/Developer/graindb/scripts/"
person_output_path = os.path.join(source_dir, 'person_output.csv')
person_temp_path = os.path.join(source_dir, 'person_temp.csv')
person_input_path = os.path.join(source_dir, 'person.csv')
knows_input_path = os.path.join(source_dir, 'knows.csv')
knows_temp_path = os.path.join(source_dir, 'knows_temp.csv')
knows_output_path = os.path.join(source_dir, 'knows_output.csv')

# concatenate first_name and last_name in person
# replace birthday with age (compared with 2020)
# replace timestamp string with unix timestamp
# replace \s with '_'
with open(person_input_path) as csv_file:
    csv_reader = csv.reader(csv_file, delimiter='|')
    next(csv_reader)  # skip header
    person_temp = open(person_temp_path, "w+")
    csv_writer = csv.writer(person_temp, delimiter='|')
    csv_writer.writerow(["id", "name", "gender", "age", "creationDate", "locationIP", "browserUsed", "place"])
    for row in csv_reader:
        age = 2020 - int(datetime.datetime.strptime(row[4], "%Y-%m-%d").year)
        creation_date = int(datetime.datetime.strptime(row[5], "%Y-%m-%d %H:%M:%S").timestamp())
        browser = row[7].replace(" ", "_")
        csv_writer.writerow(
            [row[0], "" + row[1] + "_" + row[2], row[3], age, creation_date, row[6], browser, row[8]])
    person_temp.close()

# replace timestamp string with unix timestamp
with open(knows_input_path) as csv_file:
    csv_reader = csv.reader(csv_file, delimiter='|')
    next(csv_reader)
    knows_temp = open(knows_temp_path, "w+")
    csv_writer = csv.writer(knows_temp, delimiter='|')
    csv_writer.writerow(["Person.id", "Person.id", "creationDate"])
    for row in csv_reader:
        creation_date = int(datetime.datetime.strptime(row[2], "%Y-%m-%d %H:%M:%S").timestamp())
        csv_writer.writerow([row[0], row[1], creation_date])
    knows_temp.close()

# remove person id
# with open(person_temp_path) as csv_file:
#     csv_reader = csv_reader(csv_file, delimiter='|')
#     next(csv_reader)
#     person_output = open(person_output_path, "w+")
#     csv_writer = csv.writer(person_output, delimiter='|')
#     csv_writer.writerow(["name", "gender", "age", "creationDate", "locationIP", "browserUsed", "place"])
#     for row in csv_reader:
#         csv_writer.writerow([row[1], row[2], row[3], row[4], row[5], row[6], row[7]])
#     person_output.close()

