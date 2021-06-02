import os

source_dir = "/home/g35jin/ldbc_dataset/sf30/"
input_path = os.path.join(source_dir, 'knows_names.csv')
output_path = os.path.join(source_dir, 'knows_shuffle.csv')

os.system("shuf " + input_path + " >> " + output_path)
