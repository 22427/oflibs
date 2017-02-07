#!/usr/bin/env python3
import os
import os.path
import re
import sys

# the two regex to check if a line is a pragma or an local include
po_test = re.compile('#pragma\s*once')
inc_test = re.compile('#include\s*"([^.]*).([^"]*)"')

sys_inc_test = re.compile('#include\s*<([^.]*.[^"]*)>')
sys_inc_block_start = re.compile('//\s*OFL_IB\s*([^\s]*)')
sys_inc_block_end = re.compile('//\s*OFL_IB')



lib_list = []
collection = "collections"
split = False
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
            split))
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


class File:
    def __init__(self,name,ending,code):
        self.name = name
        self.ending = ending
        self.code = code



def process_file(name,ending, dic,dep,sysheaders):
    file = include_dir+'/'+name+'.'+ending
    if os.path.exists(file):
        if name not in dic:
            dep[name]= []
            dic[name] = File(name,ending,parse_code(name,ending,open(file).read(), dic,dep,sysheaders))
    #else:
        #print('Ignoring file {}'.format(file))

def parse_code(name,ending,code,dic,dep,sysheaders):
    guard = 'OFL_'+name.upper()+'_'+ending.upper()
    result = ''
    result += '#ifndef ' + guard + '\n'
    result += '#define ' + guard + '\n'
    lines = code.split('\n')

    in_ib = False
    ib = ''
    for l in lines:
        if po_test.match(l) is not None:
            continue

        m = sys_inc_block_end.match(l)
        if in_ib and m is not None:
            in_ib = False
            sysheaders.append(ib)
            ib = ''
            continue

        m = sys_inc_block_start.match(l)
        if not in_ib and m is not None:
            in_ib = True
            continue

        if in_ib:
            ib+=l+'\n'
            continue

        m = inc_test.match(l)
        if m is not None:
            if ending is not 'cpp':
                process_file(m.group(1),m.group(2),dic,dep,sysheaders)
            dep[name].append(m.group(1))
            continue

        m = sys_inc_test.match(l)
        if m is not None:
            sysheaders.append(l)
            continue
        result += l+'\n'
    result += '#endif // ' + guard + '\n'
    return result


def remove_doubles(list):
    res =  []
    rset = set()
    for f in list:
        if f not in rset:
            res.append(f)
            rset |= {f}
    return res


def iterate_throug_files(files,deps,file):
    ne = deps.copy()
    curr = ne.copy()
    found_one = True
    while found_one:
        curr = ne.copy()
        found_one = False
        for n,l in curr.items():
            if len(l) is 0:
                found_one = True
                file.write(files[n].code)
                for nt, lt in ne.items():
                    if n in ne[nt]:
                        ne[nt].remove(n)
                ne.pop(n)
                break

def write_sysheaders(sysheaders, file):
    for sh in sysheaders:
        file.write(sh+'\n')


def read_module_set(mod_set):
    modules = mod_set
    new_modules = mod_set
    sysheaders = []
    while True:
        modules = new_modules.copy()
        deps_cpp = {}
        files_cpp = {}
        files_h = {}
        deps_h = {}
        for m in modules :
            process_file(m, 'cpp', files_cpp, deps_cpp, sysheaders)
            process_file(m, 'h', files_h, deps_h, sysheaders)
        for k,v in deps_h.items():
            new_modules |= set(v)
        for k, v in deps_cpp.items():
            new_modules |= set(v)
        if len(new_modules) == len(modules):
            break

    return  modules


def process_module_set(mod_set,lib_name):
    print('Generating library {} ...'.format(lib_name))
    for m in mod_set:
        print(m,end=', ')
    print('Extended module list:')
    modules = read_module_set(mod_set)
    for m in modules:
        print(m,end=', ')
    print('')
    sysheaders = []

    headers = {}.copy()
    headers_dep = {}.copy()

    cpps = {}.copy()
    cpp_dep = {}.copy()

    for m in modules:
        process_file(m, 'h', headers, headers_dep, sysheaders)
    for m in modules:
        process_file(m, 'cpp', cpps, cpp_dep, sysheaders)

    for k,v in cpp_dep.items():
        v.clear()



    sysheaders = remove_doubles(sysheaders)


    header_output_file = open(output_dir + output_prefix + lib_name + '.h', 'w')
    header_output_file.write(preambel)
    source_output_file = None

    if  split:
        source_output_file = open(output_dir + output_prefix + lib_name + '.cpp', 'w')
        source_output_file.write(preambel)
    else:
        source_output_file = header_output_file

    # Write the header part.
    guard = output_prefix.upper() + lib_name.upper();
    if not split:
        header_output_file.write('#ifndef ' + guard + '\n\n')
        header_output_file.write('#define ' + guard + '\n\n')
    else:
        header_output_file.write('#pragma once \n\n')
    header_output_file.write(headers_header)
    header_output_file.write('\n\n')


    write_sysheaders(sysheaders,header_output_file)
    iterate_throug_files(headers,headers_dep,header_output_file)

    if not split:
        header_output_file.write('#endif //' + guard + '\n\n')

    # Write the implementation part
    if not split:
        source_output_file.write('#ifdef ' + output_prefix.upper() + 'IMPLEMENTATION\n')

    iterate_throug_files(cpps,cpp_dep,source_output_file)

    if not split:
        source_output_file.write('#endif //' + output_prefix.upper() + 'IMPLEMENTATION\n')

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

    process_module_set(set(libs),lib_name)




"""


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

    if  split:
        source_output_file = open(output_dir + output_prefix + lib_name + '.cpp', 'w')
        source_output_file.write(preambel)
    else:
        source_output_file = header_output_file

    done_header = []
    done_cpp = []

# Write the header part.
    guard = output_prefix.upper() + lib_name.upper();
    if not split:
        header_output_file.write('#ifndef ' + guard + '\n\n')
        header_output_file.write('#define ' + guard + '\n\n')
    else:
        header_output_file.write('#pragma once \n\n')
    header_output_file.write(headers_header)
    header_output_file.write('\n\n')

    for lib in libs:
        process_header(lib, done_header, header_output_file)

    if not split:
        header_output_file.write('#endif //' + guard + '\n\n')

# Write the implementation part
    if not split:
        source_output_file.write('#ifdef ' + output_prefix.upper() + 'IMPLEMENTATION\n')
    for cpp in done_header:
        process_cpp(cpp, done_cpp, done_header, source_output_file)
    if not split:
        source_output_file.write('#endif //' + output_prefix.upper() + 'IMPLEMENTATION\n')

    source_output_file.close()
    header_output_file.close()


"""