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

   path_of_refs = os.path.join(my_curr_dir, ".git/refs/heads")
   list_of_hashes, dict_of_branch_hashes = put_branches_in_dict(path_of_refs)

   path_of_objects = os.path.join(my_curr_dir, ".git/objects")
   dict_of_parent_hashes = put_parents_in_dict(path_of_objects)

   commits = set()
   nodes = dict()

   while list_of_hashes is not []:
      has_visited = set()
      stack = list()
      stack.append(list_of_hashes.pop())
      while stack is not []:
         curr_hash = stack[-1]
         if len(dict_of_parent_hashes[curr_hash]) == 0:
            commits.add(curr_hash)
         appended = 0
         for parent in dict_of_parent_hashes[curr_hash]:
            if parent not in has_visited:
               if curr_hash not in nodes.keys():
                  nodes[curr_hash] = CommitNode(curr_hash)
               nodes[curr_hash].parents.add(parent)
               if parent not in nodes.keys():
                  nodes[parent] = CommitNode(parent)
               nodes[parent].children.add(curr_hash)
               stack.append(parent)
               appended = 1
               break
         if not appended:
            has_visited.add(curr_hash)
            stack.pop()
   commits = sorted(list(commits))
   ordered_commits = list()
   has_visited[:] = set()

   while commits is not []:
      curr_node = nodes[commits[-1]]
      appended = 0
      for index in sorted(list(curr_node.children)):
         if index not in has_visited:
            commits.append(index)
            appended = 1
            break

   num_commits = len(ordered_commits)
   for index, curr_commit in enumerate(ordered_commits):
      if index > 0 and ordered_commits[index - 1] not in nodes[curr_commit].children:
         print("=", end='')
         print(*list(nodes[curr_commit].children))
      if curr_commit in dict_of_branch_hashes.keys():
         print(curr_commit, end=' ')
         print(*sorted(dict_of_branch_hashes[curr_commit]), sep=' ')
      else:
         print(curr_commit)

      if index + 1 < num_commits and ordered_commits[index + 1] not in nodes[curr_commit].parents:
         print(*list(nodes[curr_commit].parents), sep=' ', end='')
         print("=\n")
         
def put_parents_in_dict(path):
   dict = dict()
   for directory in os.listdir(path):
      for curr_hash in os.listdir(os.path.join(path, directory)):
         dir_and_hash_path = directory + curr_hash
         compressed = open((os.path.join(path, directory, curr_hash)), 'rb')\
            .read()
         decompressed = zlib.decompress(compressed)
         if decompressed.startswith(b'commit'):
            parents_list = re.findall(r'parents\s\w{40}', decompressed.decode)
            for index, curr_parent in enumerate(parents_list):
               parents_list[index] = curr_parent[-40:]
            if dir_and_hash_path not in dict:
               dict[dir_and_hash_path] = parents_list
   return dict

def put_branches_in_dict(path):
   list = list()
   dict = dict()
   for curr_branch in os.listdir(path):
      curr_hash = open((os.path.join(path, curr_branch)), 'r').\
         readline().replace('\n', '')
      list.append(curr_hash)
      if curr_hash not in dict:
         dict[curr_hash] = list()
      else:
         dict[curr_hash].append(curr_branch)
   return list, dict

def find_git_repo():
   found_git_repo = 0
   while os.getcwd() is not "/" and not found_git_repo:
      for file in os.listdir():
         if file is ".git":
            found_git_repo = 1
            break
         if not found_git_repo:
            os.chdir("..")

   if not found_git_repo:
      sys.exit("ERROR: no git repository was found, exiting with error")

if __name__ == "__main__":
   topo_order_commits()
