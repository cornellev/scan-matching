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
        pattern = r'/\*\s*(#step|#name)(.*?)\*/'
        comments = re.findall(pattern, contents, re.DOTALL)
        if not comments:
            return
        md_filename = 'icp_' + change_extension(file, 'md')
        with open(self.dir + '/' + md_filename, 'w') as md_file:
            for (kind, comment) in comments:
                comment_parts = comment.strip().split('Sources:', 1)
                comment_text = re.sub(r'\n *', '\n', comment_parts[0])
                sources = re.findall(r'https?://[^\s]+', comment_parts[1]) if len(comment_parts) > 1 else []
                self.all_sources.update(sources)
                if kind == '#name':
                    md_file.write(f"# {comment_text} ICP\n\n> Automatically generated from {file}\n")
                elif kind == '#step':
                    md_file.write(f"\n## {comment_text}\n")
                if sources:
                    md_file.write('**Sources**  \n')
                    for source in sources:
                        md_file.write(f"- {source}\n")

    def build(self):
        for root, dirs, files in os.walk('src/icp'):
            for file in files:
                if file.endswith('.cpp'):
                    with open(os.path.join(root, file), 'r') as f:
                        self.extract(file, f.read())
        os.makedirs(self.dir + '/extra', exist_ok=True)
        with open(self.dir + '/extra/sources.md', 'w') as md_file:
            md_file.write('# ICP Sources\n\n')
            for source in sorted(list(self.all_sources)):
                md_file.write(f" - {source}\n")
            

if __name__ == '__main__':
   doc_builder = ICPDocumentationBuilder('book/icp_descr')
   doc_builder.build()
    
