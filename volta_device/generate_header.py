import os
import glob
import json

header_comment_path = './header_template.txt'
config_file_path = './project_config.json'

def get_header_comment():
    with open(header_comment_path, 'r') as file:
        return file.read()

def update_file_header(file_path, header_comment):
    with open(file_path, 'r') as file:
        content = file.read()

    header_start = content.find('/*')
    header_end = content.find('*/', header_start + 2) if header_start != -1 else -1
    if header_start != -1 and header_end != -1:
        content = content[header_end + 2:].lstrip()

    filename = os.path.basename(file_path)
    updated_header = header_comment.replace('{filename}', filename)

    with open(file_path, 'w') as file:
        file.write(updated_header + content)


def process_c_project_folder(project_path, header_comment):
    files = glob.glob(os.path.join(project_path, '**/*.c'), recursive=True) + \
            glob.glob(os.path.join(project_path, '**/*.h'), recursive=True)

    for file_path in files:
        filename = os.path.basename(file_path)
        updated_header = header_comment.replace('{filename}', filename)
        update_file_header(file_path, updated_header)
        print(f"Updated header in file: {file_path}")

if __name__ == '__main__':
    project_path = './'

    with open(config_file_path, 'r') as config_file:
        config_data = json.load(config_file)

    header_comment = get_header_comment().format(**config_data)

    process_c_project_folder(project_path, header_comment)
