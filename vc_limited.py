#!/usr/bin/env python3

import itertools

def powerset(iterable):
    "Subsequences of the iterable from shortest to longest."
    # powerset([1,2,3]) → () (1,) (2,) (3,) (1,2) (1,3) (2,3) (1,2,3)
    s = list(iterable)
    return itertools.chain.from_iterable(itertools.combinations(s, r) for r in range(len(s)+1))

n = int(input())
edges = []
for i in range(n):
    while True:
        edge = input().split()
        if len(edge) != 0:
            edges.append(edge)
            break

edges = [ (int(a),int(b)) for a,b in edges ]
vertices = sorted(list(set([a for a,b in edges] + [b for a,b in edges])))
rounds = len(edges)

print('min: score;')
print(f'x_{0}_t_{1} >= x_{1}_t_{1};') 
for v in vertices:
    print(f'x_{v}_t_{0} >= 0;') 
    for r in range(rounds+1):
        print(f'x_{v}_t_{r} <= x_{v}_t_{r+1};')
    print(f'x_{v}_t_{rounds+1} <= 1;') 
for r,(a,b) in zip(range(1,rounds+1),edges):
    print(f'x_{a}_t_{r} + x_{b}_t_{r} >= 1;')

for r in range(1,rounds+1):
    for s in powerset(vertices[:r+1]):
        check = True
        for a,b in edges[0:r]:
            if int(a not in s) + int(b not in s) != 1:
                check = False
                break
        if check:
            # print(s)
            print(' + '.join([str(len(s))] + [f'x_{v}_t_{r}' for v in vertices if v not in s and int(v) <= r])+f' <= {len(s)}*score;')
