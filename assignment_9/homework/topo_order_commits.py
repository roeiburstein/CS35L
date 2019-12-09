import os
import sys
import zlib
import re

class CommitNode:
        def __init__(self, commit_hash):
            """
            :type commit_hash: str
            """
            self.commit_hash = commit_hash
            self.parents = set()
            self.children = set()

def topo_order_commits():
    find_git_repo()
    my_curr_dir = os.getcwd()

    list_of_hashes = []
    dict_of_branch_hashes = dict()
    path_of_refs = os.path.join(my_curr_dir, '.git/refs/heads')

    for curr_branch in os.listdir(path_of_refs):
        curr_hash = open((os.path.join(path_of_refs, curr_branch)),
                         'r').readline().replace('\n', '')
        list_of_hashes.append(curr_hash)

        if curr_hash not in dict_of_branch_hashes:
            dict_of_branch_hashes[curr_hash] = []
        dict_of_branch_hashes[curr_hash].append(curr_branch)

    path_of_objects = os.path.join(my_curr_dir, '.git/objects')
    dict_of_parent_hashes = dict()

    for directory in os.listdir(path_of_objects):

        for curr_hash in os.listdir(os.path.join(path_of_objects, directory)):
            compressed = open((os.path.join(path_of_objects, directory,
                                            curr_hash)), 'rb').read()
            decompressed = zlib.decompress(compressed)

            if decompressed.startswith(b'commit'):
                parents_list = re.findall(r'parent\s\w{40}',
                                          decompressed.decode())
                for index, parent in enumerate(parents_list):
                    parents_list[index] = parent[-40:]

                if (directory + curr_hash) not in dict_of_parent_hashes:
                    dict_of_parent_hashes[directory+curr_hash] = parents_list

    nodes = dict()
    commits = set()

    while list_of_hashes != []:
        stack = list()
        stack.append(list_of_hashes.pop())
        has_visited = set()

        while stack != []:
            curr_hash = stack[-1]

            if len(dict_of_parent_hashes[curr_hash]) == 0:
                commits.add(curr_hash)

            appended = False

            for parent in dict_of_parent_hashes[curr_hash]:

                if parent not in has_visited:

                    if curr_hash not in nodes.keys():
                        nodes[curr_hash] = CommitNode(curr_hash)
                    nodes[curr_hash].parents.add(parent)

                    if parent not in nodes.keys():
                        nodes[parent] = CommitNode(parent)

                    nodes[parent].children.add(curr_hash)
                    stack.append(parent)
                    appended = True
                    break

            if not appended:
                has_visited.add(curr_hash)
                stack.pop()

    commits = sorted(list(commits))
    ordered_commits = []
    has_visited.clear()

    while commits != []:
        appended = False
        curr_node = nodes[commits[-1]]

        for index in sorted(list(curr_node.children)):

            if index not in has_visited:
                commits.append(index)
                appended = True
                break

        if not appended:
            ordered_commits.append(curr_node.commit_hash)
            commits.pop()
            has_visited.add(curr_node.commit_hash)

    num_commits = len(ordered_commits)

    for index, commit in enumerate(ordered_commits):

        if index > 0 and ordered_commits[index - 1] not in \
           nodes[commit].children:
            print('=', end='')
            print(*list(nodes[commit].children))

        if commit in dict_of_branch_hashes.keys():
            print(commit, end=' ')
            print(*sorted(dict_of_branch_hashes[commit]), sep=' ')

        else:
            print(commit)

        if index + 1 < num_commits and ordered_commits[index + 1] not in \
           nodes[commit].parents:
            print(*list(nodes[commit].parents), sep=' ', end='')
            print('=\n')

def find_git_repo():
   found_git_repo = False

   while os.getcwd() != '/' and not (found_git_repo):

      for file in os.listdir():

         if file == '.git':
            found_git_repo = True
            break

      if not (found_git_repo):
         os.chdir('..')

   if not (found_git_repo):
      sys.exit("ERROR: no git repository was found, exiting with error")

if __name__ == '__main__':
    topo_order_commits()
