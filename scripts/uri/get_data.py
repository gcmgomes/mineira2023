#!/usr/bin/python3

from jinja2 import Environment, FileSystemLoader
import sys
import os
import re
import shutil

def load_file(path):
   with open(path, 'r') as f:
      return f.read()

def write_file(path, data):
   with open(path, 'w') as f:
      f.write(data)

def read_fullname(path):
   return load_file(os.path.join(path, "attic", "fullname")).strip()

def read_basename(path):
   return os.path.basename(path)

def read_timelimit(path):
   return int(load_file(os.path.join(path, "attic", "timelimit")).strip())

def render_metadata(data):
   file_loader = FileSystemLoader("templates")
   environment = Environment(loader = file_loader)
   template = environment.get_template("metadata.yaml")
   result = template.render(data = data)
   write_file("metadata.yaml", result)

def rep(n):
   if n < 26:
      return str(chr(ord('a') + n))
   return rep(n // 26 - 1) + rep(n % 26)

def copy_testcases(problem_path):
   problem_path = problem_path.strip("/")
   basename = read_basename(problem_path)
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
            name = "input/%s-%s.in" % (basename, rep(global_index - 1))
         elif parts[1] == "sol":
            name = "output/%s-%s.sol" % (basename, rep(global_index - 1))
         else:
            assert False
         shutil.copyfile(os.path.join(group_path, item), name)
         group_size += 1
      base += group_size // 2



if __name__ == "__main__":
   problem_path = sys.argv[1]

   data = {}
   data['title'] = read_fullname(problem_path)
   data['timelimit'] = read_timelimit(problem_path)
   data['level'] = 1
   data['category'] = 1
   data['subject'] = ""
   render_metadata(data)

   copy_testcases(problem_path)
