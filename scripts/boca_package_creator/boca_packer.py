#!/usr/bin/env python3

import zipfile
import sys
import os

languages = ['c', 'cc', 'cpp', 'java', 'py2', 'py3']


def write_limits(archive, language, time_limit):
    path = os.path.join("limits", language)
    archive.writestr(
        path, "#!/bin/bash\necho %d\necho 1\necho 1024\necho 4096\nexit 0\n" %
        time_limit)


def write_description(archive, base_name, full_name, descfile):
    if descfile:
        archive.write(descfile, "description/%s.pdf" % base_name)
        descfile = "%s.pdf" % base_name
    path = "description/problem.info"
    archive.writestr(path, "basename=%s\nfullname=%s\ndescfile=%s\n" %
                     (base_name, full_name, descfile))


def write_test(archive, language):
    path = os.path.join("tests", language)
    archive.writestr(path, "#!/bin/bash\nexit 0\n")


def copy_files_from_path(archive, origin_path, destination_path):
    for name in os.listdir(origin_path):
        archive.write(
            os.path.join(origin_path, name),
            os.path.join(destination_path, name))


def write_from_template(archive, path):
    archive.write(os.path.join("template", path), path)


def write_run(archive, language):
    write_from_template(archive, os.path.join("run", language))


def write_compile(archive, language):
    write_from_template(archive, os.path.join("compile", language))


def write_compare(archive, language, compare):
    if compare:
        archive.write(compare, os.path.join("compare", language))
    else:
        write_from_template(archive, os.path.join("compare", language))


def read_from_file(path):
    with open(path, 'r') as f:
        return "".join(f.readlines())


def main():
    package_path = sys.argv[1]
    problem_path = sys.argv[2].strip("/")

    base_name = os.path.basename(problem_path)

    if package_path == "@":
        package_path = os.path.join(problem_path, "build", base_name + ".zip")

    print("package_path=%s" % package_path)

    print("basename=%s" % base_name)

    full_name = read_from_file(
        os.path.join(problem_path, "attic/fullname")).strip()

    print("fullname=%s" % full_name)

    time_limit = int(
        read_from_file(os.path.join(problem_path, "attic/timelimit")).strip())

    print("time_limit=%d" % time_limit)

    archive = zipfile.ZipFile(package_path, 'w', zipfile.ZIP_DEFLATED)

    descfile = os.path.join(problem_path, "documents", "statement.pdf")
    if os.path.exists(descfile):
        print("has_statement=true")
    else:
        descfile = ""
        print("has_statement=false")
    write_description(archive, base_name, full_name, descfile)

    compare = os.path.join(problem_path, "attic", "compare")
    if os.path.exists(compare):
        print("comparator=true")
    else:
        print("comparator=false")
        compare = ""

    for language in languages:
        write_limits(archive, language, time_limit)
        write_test(archive, language)
        write_run(archive, language)
        write_compile(archive, language)
        write_compare(archive, language, compare)

    tests_path = os.path.join(problem_path, "build", "tests")
    base = 0
    for group in sorted(os.listdir(tests_path)):
        group_size = 0
        group_path = os.path.join(tests_path, group)
        for item in os.listdir(group_path):
            parts = item.split('.')
            index_inside_group = int(parts[0])
            global_index = base + index_inside_group
            name = ""
            if parts[1] == "in":
                name = "input/%03d" % global_index
            elif parts[1] == "sol":
                name = "output/%03d" % global_index
            else:
                assert False
            archive.write(os.path.join(group_path, item), name)
            group_size += 1

        base += group_size / 2

    print("number_of_tests=%d" % base)

    archive.close()


if __name__ == '__main__':
    main()
