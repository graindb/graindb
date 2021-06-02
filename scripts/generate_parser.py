# use bison to generate the parser files of postgres
# the following versions were used:
# bison (GNU Bison) 3.4.2
# flex 2.6.4
bison_location     = "/usr/bin/bison"
main_file_location = "third_party/libpg_query/grammar"
main_file          = "grammar.y"
compile_location = "third_party/libpg_query/src_backend_parser_gram.cpp"
compile_location_hpp = "third_party/libpg_query/src_backend_parser_gram.hpp"
target_location_header = "third_party/libpg_query/include/parser/gram.hpp"

import os, subprocess

yacc_file = os.path.join(main_file_location, main_file)
yacc_temp_file = yacc_file + ".tmp"

# generate the bison
cmd = [bison_location, "-o", compile_location, "-d", yacc_temp_file]
print(' '.join(cmd))
proc = subprocess.Popen(cmd)
res = proc.wait()

if res != 0:
    exit(1)


# replace namespace with base_yy to postgres
def replace_namespace(file_location):
    with open(file_location, 'r') as f:
        text = f.read()
    text = text.replace("namespace base_yy", "namespace postgres")
    # text = text.replace("location.hh", "location.hpp")
    with open(file_location, 'w+') as f:
        f.write(text)


# replace_namespace(compile_location_location)
replace_namespace(compile_location_hpp)
replace_namespace(compile_location)

# move headers to correct location
os.rename(compile_location_hpp, target_location_header)
# os.rename(compile_location_location, target_location_location)
