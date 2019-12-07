import os
import zlib
import sys
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
    found = False;
    while not(found) and os.getcwd() != '/':
        for fileName in os.listdir():
            if fileName == '.git':
                found = True;
                break
        if not(found):
                os.chdir('..')

    if not(found):
            sys.exit('Not inside a Git repository')

    objectsPath = os.path.join(os.getcwd(), '.git/objects')
    refsPath = os.path.join(os.getcwd(), '.git/refs/heads')

    hashToNode = dict()
    root_commits = set()
    branchesHash = []
    hashToBranch = dict()
    hashToParents = dict()

    for branch in os.listdir(refsPath):
        hash = open((os.path.join(refsPath, branch)), 'r').readline().replace('\n', '')
        branchesHash.append(hash)
        if hash not in hashToBranch:
                hashToBranch[hash] = []
        hashToBranch[hash].append(branch)

    '''
    for hash in hashToBranch.keys():
        print(hash)
        for branch in hashToBranch[hash]:
            print(branch)
        print("\n")
    '''    

    for dir in os.listdir(objectsPath):
        for hash in os.listdir(os.path.join(objectsPath, dir)):
            compressed_content = open((os.path.join(objectsPath, dir, hash)), 'rb').read()
            decompressed_content = zlib.decompress(compressed_content)
            if decompressed_content.startswith(b'commit'):
                parents = re.findall(r'parent\s\w{40}', decompressed_content.decode())
                for i, parent in enumerate(parents):
                    parents[i]  = parent[-40:]
                if (dir + hash) not in hashToParents:
                    hashToParents[dir+hash] = parents

    '''
    for hash in hashToParents:
        print(hash)
        for parent in hashToParents[hash]:
            print(parent)
        print('\n')
    '''

    while branchesHash:
        visited1 = set()
        stack = []
        stack.append(branchesHash.pop())
        while stack:
            hash = stack[-1]
            if len(hashToParents[hash]) == 0:
                root_commits.add(hash)
            hasAppended = False
            for parent in hashToParents[hash]:
                if parent not in visited1:
                    if hash not in hashToNode.keys():
                        hashToNode[hash] = CommitNode(hash)
                    hashToNode[hash].parents.add(parent)
                    if parent not in hashToNode.keys():
                        hashToNode[parent] = CommitNode(parent)
                    hashToNode[parent].children.add(hash)
                    stack.append(parent)
                    hasAppended = True
                    break
            if not hasAppended:
                visited1.add(hash)
                stack.pop()

    root_commits = sorted(list(root_commits))
    orderedCommits = []
    visited2 = set()

    while root_commits:
        node = hashToNode[root_commits[-1]]
        hasAppended = False
        for v in sorted(list(node.children)):
            if v not in visited2:
                root_commits.append(v)
                hasAppended = True
                break
        if not hasAppended:
            orderedCommits.append(node.commit_hash)
            root_commits.pop()
            visited2.add(node.commit_hash)

    size = len(orderedCommits)
    for i, commit in enumerate(orderedCommits):
        if i>0 and orderedCommits[i-1] not in hashToNode[commit].children:
            print('=', end='')
            print(*list(hashToNode[commit].children))
        if commit in hashToBranch.keys():
            print(commit, end=' ')
            print(*sorted(hashToBranch[commit]), sep=' ')
        else:
            print(commit)

        if i+1<size and orderedCommits[i+1] not in hashToNode[commit].parents:
            print(*list(hashToNode[commit].parents), sep=' ', end='')
            print('=\n')






if __name__ == '__main__':
    topo_order_commits()
