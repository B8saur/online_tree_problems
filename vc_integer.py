#!/usr/bin/env python3

import itertools

PRECISION = 10
#limit for vertex number 1:
LIMIT1 = PRECISION
#limit for all other vertices:
LIMIT = PRECISION

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

# print(edges)
# print(vertices)
# print(rounds)

degree = len(vertices)*[0]
print('min: score;')
#hard coded for PRECISION = 10
print(f'5 = x_0_1;')
print(f'  7 = x_0_2;')
print(f'5 = x_1_1;')
print(f'  8 = x_1_2;')
print(f'2 = x_2_1;')
print(f'  7 = x_2_2;')

for v in vertices[3:]:
    print(f'3 = x_{v}_1;')
    print(f'  7 = x_{v}_2;')

# for v in vertices:
#     print(f'0 <= x_{v}_1;')
#     print(f'  x_{v}_1 <= x_{v}_2;')
#     if v == 1:
#         print(f'    x_{v}_2 <= {LIMIT1};')
#     else:
#         print(f'    x_{v}_2 <= {LIMIT};')
    
for a, b in edges:
    degree[a] += 1
    degree[b] += 1
    print(f'x_{a}_{degree[a]} + x_{b}_{degree[b]} >= {PRECISION};')
print(f'x_{0}_1 >= x_{1}_1;')           #break symmetry
print()

degree = len(vertices)*[0]
for r in range(0, rounds):
    # print(f'    r={r}')
    a, b = edges[r]
    # print(f"  {a}, {b}")
    degree[a] += 1
    degree[b] += 1
    # print(degree)
    for s in powerset(vertices[0:r+2]):
        check = True
        for a,b in edges[0:r+1]:
            if a not in s and b not in s:
                check = False
                break
        if check:
            # print(s)
            print(' + '.join([str(len(s)*PRECISION)] + [f'x_{v}_{degree[v]}' for v in vertices if v not in s and v <= r+1])+f' <= {len(s)*PRECISION}*score;')

for v in vertices:
    print(f'int x_{v}_1;')
    print(f'int x_{v}_2;')