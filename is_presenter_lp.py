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
    # (3,4),
    # (0,1),
    # (0,'A'),
    # (4,5),
    # (5,6),
    # ('A','B'),
    # ('B','C')
]

edges = [ (str(a),str(b)) for a,b in edges ]
vertices = sorted(list(set([a for a,b in edges] + [b for a,b in edges])))
rounds = len(edges)

print('max: score;')
for v in vertices: #start
    print(f'x_{v}_t_{0} <= 1;')
for r in range(rounds): #monotonicity
    for v in vertices:
        print(f'x_{v}_t_{r} >= x_{v}_t_{r+1};')
for v in vertices: #end
    print(f'x_{v}_t_{rounds} >= 0;')
print(f'x_{1}_t_{1} <= x_{2}_t_{1};') #break symmetry

for r,(a,b) in zip(range(1,rounds+1),edges): #add edge -> add requirement
    print(f'x_{a}_t_{r} + x_{b}_t_{r} <= 1;')
print()
print()
cur_vertices = set()
for r in range(1,rounds+1):
    cur_vertices.update(edges[r-1])
    # print(cur_vertices)
    for s in powerset(cur_vertices):
        # print(f"Subset: {s}")
        check = True
        for a,b in edges[0:r]: #assert independence
            if a in s and b in s:
                # print(f"Edge ({a},{b}) in s, set not independent, skipping")
                check = False
                break
        if check:
            print(' + '.join([f'x_{v}_t_{r}' for v in cur_vertices])+f' >= {len(s)}*score;')
            # print(' + '.join([str(len(s))] + [f'x_{v}_t_{r}' for v in vertices if v not in s])+f' <= {len(s)}*score;')
    print()
