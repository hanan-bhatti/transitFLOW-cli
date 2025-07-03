import os

def print_tree(start_path='.', prefix=''):
    files = sorted(os.listdir(start_path))
    total = len(files)
    
    for index, name in enumerate(files):
        path = os.path.join(start_path, name)
        connector = "└── " if index == total - 1 else "├── "
        print(prefix + connector + name)

        if os.path.isdir(path):
            extension = "    " if index == total - 1 else "│   "
            print_tree(path, prefix + extension)

if __name__ == "__main__":
    print("📁 Project Directory Tree:\n")
    print_tree('.')
