#!/usr/bin/python3

from jinja2 import Environment, FileSystemLoader
import sys
import os
import re
import TexSoup
import shutil
from PIL import Image

def load_file(path):
   with open(path, 'r') as f:
      return f.read()

def write_file(path, data):
   with open(path, 'w') as f:
      f.write(data)

def read_fullname(path):
   return load_file(os.path.join(path, "attic", "fullname")).strip()

def read_timelimit(path):
   return int(load_file(os.path.join(path, "attic", "timelimit")).strip())

def read_statement(path):
   return load_file(os.path.join(path, "documents", "statement.tex"))

def read_author(path):
   return load_file(os.path.join(path, "attic", "author")).strip()

def render(data):
   file_loader = FileSystemLoader("templates")
   environment = Environment(loader = file_loader)
   template = environment.get_template("statement.html")
   result = template.render(data = data)
   write_file("statement.html", result)

def tex_math(node):
    ans = "<span class=\"math-class\">\\("
    for child in node.contents:
        ans += str(child)
    ans += "\\)</span>"
    return ans

def tex_image(node, path):
   source=""
   scale = 1.0
   for arg in node.args:
      if isinstance(arg, TexSoup.data.RArg):
         source=arg.value
      elif re.match("scale=.*", arg.value):
         scale = float(arg.value.split('=')[1])
   cwd = os.path.join(path, "documents")
   source = source.replace("\\CWD", cwd)
   name = os.path.basename(source)
   shutil.copyfile(source, name)
   image = Image.open(name)
   width, height = image.size
   width = int(width * scale)
   height = int(height * scale)
   return "<img src=%s style=\"width: %dpx; height: %dpx; \"/>" % (name, width, height)

def tex_enumerate(node, path):
   ans = ""
   for item in node.children:
      item_ans = "<li> "
      for part in item.extra:
         if isinstance(part, TexSoup.data.TexCmd):
            for subpart in part.contents:
               item_ans += str(subpart)
         else:
            item_ans += str(part)
      item_ans += " </li>"
      ans += item_ans + "\n"
   return ans

def tex_empth(node, path):
   ans = "<em>"
   for child in node.contents:
      ans += tex2html_inner(child, path)
   ans += "</em>"
   return ans

def tex2html_inner(node, path):
    ans = ""
    if isinstance(node, TexSoup.data.TexNode):
       
        if node.name == "label":
           return ""

        if node.name == "texttt" or node.name == "textit" or node.name == "empth":
           return tex_empth(node, path)

        if node.name == "includegraphics":
            ans = "<p class=\"center\">\n"
            ans += tex_image(node, path)
            ans += "</p>\n"
            return ans
        
        if node.name == "$":
            return tex_math(node)
        
        if node.name == "$$":
            return "<p>" + tex_math(node) + "</p>\n"
        
        if node.name == "enumerate":
            return "<ol>\n" + tex_enumerate(node, path) + "</ol>\n"
        
        if node.name == "itemize":
            return "<ul>\n" + tex_enumerate(node, path) + "</ul>\n"

        for child in node.contents:
            ans += tex2html_inner(child, path)
            
    else:
        ans += re.sub("%.*", "", str(node))

    return ans

def tex2html(tex, path):
    ans = ""
    for part in re.split("\n\n+", tex):
        ans += "<p>"
        tex_part = re.sub("\n\n+", "\n\n", 
                          "\\begin{document}\n" + part + "\n\\end{document}")
        soup = TexSoup.TexSoup(tex_part)
        ans += tex2html_inner(soup, path);
        ans += "</p>\n"
    ans = re.sub("<p></p>", "", ans)
    ans = re.sub("\n+", "\n", ans)
    ans = ans.replace("\\{", "{")
    ans = ans.replace("\\}", "}")
    ans = re.sub("\\\\label{.*}", "", ans)
    return ans

def process_statement(path, statement, data):
   parts = re.split("\\\\section\\*\\{.*\\}|\\\\subsection\\*\\{.*\\}", statement)
   statement_tex = parts[0]
   input_tex = parts[1] + parts[3]
   output_tex = parts[2]

   data['description'] = tex2html(statement_tex, path)
   data['input_description'] = tex2html(input_tex, path)
   data['output_description'] = tex2html(output_tex, path)

   if len(parts) > 4 and len(parts[0]) > 30:
      data['notes'] = tex2html(parts[4], path)

def prepare_io_data(data):
   return data.replace("\n", "<br />\n")

def process_samples(path, data):
   samples = []
   for name in os.listdir(os.path.join(path, "documents")):
      if re.match("sample.*\.in", name):
         samples.append((name, name.replace(".in", ".sol")))
   for in_file, sol_file in sorted(samples):
      sample = {}
      input_data = load_file(os.path.join(path, "documents", in_file))
      sample['input'] = prepare_io_data(input_data)
      output_data = load_file(os.path.join(path, "documents", sol_file))
      sample['output'] = prepare_io_data(output_data)
      data['samples'].append(sample)

if __name__ == "__main__":
   problem_path = sys.argv[1]

   data = {}
   data['title'] = read_fullname(problem_path)
   data['author'] = read_author(problem_path)
   data['timelimit'] = read_timelimit(problem_path)
   
   data['notes'] = ""
   statement = read_statement(problem_path)
   process_statement(problem_path, statement, data)

   data['samples'] = []
   process_samples(problem_path, data)

   render(data)
