# Copyright (C) 2024 Ethan Uppal. All rights reserved.

import os, re

def change_extension(filename, new_extension):
    name, _ = os.path.splitext(filename)
    return f'{name}.{new_extension}'

class ICPDocumentationBuilder:
    def __init__(self, dir):
        self.dir = dir
        self.all_sources = set()
    
    def extract(self, file, contents):
        pattern = r'/\*\s*(#step|#name|#desc)(.*?)\*/'
        comments = re.findall(pattern, contents, re.DOTALL)
        if not comments:
            return
        md_filename = 'icp_' + change_extension(file, 'md')
        made_description = False
        with open(self.dir + '/' + md_filename, 'w') as md_file:
            step_cnt = 1
            for (kind, comment) in comments:
                comment_parts = comment.strip().split('Sources:', 1)
                comment_text = re.sub(r'\n *', '\n', comment_parts[0])
                sources = re.findall(r'https?://[^\s]+', comment_parts[1]) if len(comment_parts) > 1 else []
                self.all_sources.update(sources)
                if kind == '#name':
                    method_name = re.findall(r'register_method\("([^"]*)', contents)[0]
                    md_file.write(f"\page {comment_text.lower()}_icp {comment_text} ICP\n")
                    md_file.write(f'\par Usage\nYou can construct a new instance of {comment_text} ICP with `icp::ICP::from_method("{method_name}")`, with an additional optional parameter for configuration.')
                elif kind == '#step':
                    if not made_description:
                        md_file.write('\n\par Description\n')
                        made_description = True
                    lines = comment_text.splitlines()
                    first_line_parts = lines[0].split(':', 1)               
                    if len(first_line_parts) == 2:
                        lines[0] = f"**{first_line_parts[0]}**:{first_line_parts[1]}"
                    else:
                        lines[0] = f"**{first_line_parts[0]}**"
                    step_text = '\n'.join('    ' + line for line in lines)
                    md_file.write(f"\n{step_cnt}. {step_text}\n")
                    step_cnt += 1
                    if sources:
                        md_file.write('    Sources:  \n')
                        for source in sources:
                            md_file.write(f"    - {source}\n")
                        md_file.write('\n')
                elif kind == '#desc':
                    md_file.write(f"\n\par Description\n{comment_text}\n")
                    made_description = True
            md_file.write('\n\nRead \\ref icp_sources for a list of all resources used in this project.')
            md_file.write(f"\nThis page was automatically generated from {file} with {os.path.basename(__file__)}.")

    def build(self):
        for root, dirs, files in os.walk('src/icp'):
            for file in files:
                if file.endswith('.cpp'):
                    with open(os.path.join(root, file), 'r') as f:
                        self.extract(file, f.read())
        os.makedirs(self.dir + '/extra', exist_ok=True)
        with open(self.dir + '/extra/sources.md', 'w') as md_file:
            md_file.write('\page icp_sources ICP Sources\n\n')
            md_file.write('This list contains all resources used in implementing ICP for this project in alphabetical order.\n\n')
            for source in sorted(list(self.all_sources)):
                md_file.write(f" - {source}\n")
            

if __name__ == '__main__':
   doc_builder = ICPDocumentationBuilder('book/icp_descr')
   doc_builder.build()
    
