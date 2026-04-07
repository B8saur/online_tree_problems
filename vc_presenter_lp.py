#!/usr/bin/env python3

import itertools

def powerset(iterable):
    "Subsequences of the iterable from shortest to longest."
    # powerset([1,2,3]) → () (1,) (2,) (3,) (1,2) (1,3) (2,3) (1,2,3)
    s = list(iterable)
    return itertools.chain.from_iterable(itertools.combinations(s, r) for r in range(len(s)+1))

edges = [
    (1,2),
    (2,3),
    (3,4),
    (0,1),
    (0,'A'),
    (4,5),
    (5,6),
    ('A','B'),
    ('B','C')
]

edges = [ (str(a),str(b)) for a,b in edges ]
vertices = sorted(list(set([a for a,b in edges] + [b for a,b in edges])))
rounds = len(edges)

print('min: score;')
print(f'x_{1}_t_{1} >= x_{2}_t_{1};') 
for v in vertices:
    print(f'x_{v}_t_{0} >= 0;') 
    for r in range(rounds+1):
        print(f'x_{v}_t_{r} <= x_{v}_t_{r+1};')
    print(f'x_{v}_t_{rounds+1} <= 1;') 
for r,(a,b) in zip(range(1,rounds+1),edges):
    print(f'x_{a}_t_{r} + x_{b}_t_{r} >= 1;')

for r in range(1,rounds+1):
    for s in powerset(vertices):
        check = True
        for a,b in edges[0:r]:
            if a not in s and b not in s:
                check = False
                break
        if check:
            print(' + '.join([str(len(s))] + [f'x_{v}_t_{r}' for v in vertices if v not in s])+f' <= {len(s)}*score;')
