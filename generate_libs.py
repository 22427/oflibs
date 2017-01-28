#!/usr/bin/env python3
import os
import re
import sys

# the two regex to check if a line is a pragma or an local include
po_test = re.compile('#pragma\s*once')
inc_test = re.compile('#include\s*"([^.]*).([^"]*)"')

lib_list = []
collection = "collections"
implementation_in_header = True
include_dir = "./dev/"
source_dir = "./dev/"
doc_dir = "./dev/doc/"

output_dir = "./libs/"
output_prefix = "ofl_"

headers_header = "/** Here your advert could appeare soon! **/"

preambel = "// This is auto generated code. Do not edit it directly.\n\n"


def print_help():
    print(
        "-split\n\tIf set the implementation will be stored in a seperat .cpp file.\n\tDefault: {0}".format(
            implementation_in_header))
    print(
        "-include_dir\n\tSet the source directory for the header files.\n\tDefault: {0}".format(
            include_dir))
    print(
        "-source_dir\n\tSet the source directory for the source files.\n\tDefault: {0}".format(
            source_dir))
    print(
        "-doc_dir\n\tSet the source directory for the documentation files.\n\tDefault: {0}".format(
            doc_dir))
    print(
        "-out_dir\n\tSet the directory for the results.\n\tDefault: {0}".format(
            output_dir))
    print(
        "-prefix\n\tSet the prefix for the output file names, as well as the in file defines.\n\tDefault: {0}".format(
            output_prefix))
    print(
        "-collection\n\tSet the collection directory where the library collection files are found.\n\tDefault: {0}".format(
            collection))
    print(
        "-headers_header\n\tSet the headers header. This is included directly into the libs header, after the include guard.\n\tDefault: {0}".format(
            headers_header))
    print(
        "-h\n\tShow this help text.\n")

for i in range(1,len(sys.argv)):
    if '-split' in sys.argv[i]:
        implementation_in_header = False
    elif '-h' in sys.argv[i]:
        print_help()
        exit(0)
    elif '-include_dir' in sys.argv[i]:
        include_dir = sys.argv[i+1]
        i += 1
    elif '-source_dir' in sys.argv[i]:
        source_dir = sys.argv[i+1]
        i += 1
    elif '-doc_dir' in sys.argv[i]:
        doc_dir = sys.argv[i+1]
        i += 1
    elif '-out_dir' in sys.argv[i]:
        output_dir = sys.argv[i+1]
        i += 1
    elif '-prefix' in sys.argv[i]:
        output_prefix = sys.argv[i+1]
        i += 1
    elif '-collection' in sys.argv[i]:
        collection = sys.argv[i + 1]
        i += 1
    elif '-headers_header' in sys.argv[i]:
        headers_header = sys.argv[i+1]
        i += 1
    else:
        lib_list.append(sys.argv[i])


def process_header(lib_name, done_list, output_file):
    if lib_name in done_list:
        return
    done_list.append(lib_name)
    guard = output_prefix.upper() + lib_name.upper()
    output_file.write('#ifndef ' + guard + '\n')
    output_file.write('#define ' + guard + '\n')
    with open(include_dir + lib_name + ".h") as f:
        lines = f.readlines()
    for l in lines:
        if po_test.match(l) is not None:
            continue
        m = inc_test.match(l)
        if m is not None:
            name = m.group(1)
            if name not in done_list:
                process_header(name, done_list, output_file)
            continue
        output_file.write(l)
    output_file.write('#endif //' + guard + '\n')


def process_cpp(lib_name,done_list, done_headers, output_file):
    if lib_name in done_list:
        return
    done_list.append(lib_name)
    try:
        with open(source_dir + lib_name + '.cpp') as f:
            lines = f.readlines()
    except FileNotFoundError:
        return

    guard = output_prefix.upper() + lib_name.upper()+"_IMPLEMENTATION"
    output_file.write('#ifndef ' + guard + '\n')
    output_file.write('#define ' + guard + '\n')

    for l in lines:
        if po_test.match(l) is not None:
            continue
        m = inc_test.match(l)
        if m is not None:
            name = m.group(1)
            if name not in done_headers:
                process_header(name, done_headers, output_file)
            continue
        output_file.write(l)
    output_file.write('#endif //' + guard + '\n')


if not os.path.exists(output_dir):
    os.makedirs(output_dir)

if len(lib_list) is 0:
    lib_list = os.listdir(collection)


lib_list = ['{0}/{1}'.format(collection,llist) for llist in lib_list]


for llist in lib_list:
    lib_name = os.path.splitext(os.path.basename(llist))[0]
    with open(llist) as f:
        libs = f.readlines()
    libs = [x.strip() for x in libs]

    header_output_file = open(output_dir + output_prefix + lib_name + '.h', 'w')
    header_output_file.write(preambel)
    source_output_file = None

    if not implementation_in_header:
        source_output_file = open(output_dir + output_prefix + lib_name + '.cpp', 'w')
        source_output_file.write(preambel)
    else:
        source_output_file = header_output_file

    done_header = []
    done_cpp = []

# Write the header part.
    guard = output_prefix.upper() + lib_name.upper();
    if implementation_in_header:
        header_output_file.write('#ifndef ' + guard + '\n\n')
        header_output_file.write('#define ' + guard + '\n\n')
    else:
        header_output_file.write('#pragma once \n\n')
    header_output_file.write(headers_header)
    header_output_file.write('\n\n')

    for lib in libs:
        process_header(lib, done_header, header_output_file)

    if implementation_in_header:
        header_output_file.write('#endif //' + guard + '\n\n')

# Write the implementation part
    if implementation_in_header:
        source_output_file.write('#ifdef ' + output_prefix.upper() + 'IMPLEMENTATION\n')
    for cpp in done_header:
        process_cpp(cpp, done_cpp, done_header, source_output_file)
    if implementation_in_header:
        source_output_file.write('#endif //' + output_prefix.upper() + 'IMPLEMENTATION\n')

    source_output_file.close()
    header_output_file.close()


