input = [
    "805A",
    "170A",
    "129A",
    "283A",
    "540A"
]

# Python is goddamn easier... since I can just spam @cache everywhere

from functools import cache
from itertools import pairwise

NUMERIC = '789', '456', '123', ' 0A'
DIRECTIONAL = ' ^A', '<v>'


def walk(keypad, x, y, path):
    for direction in path:
        neighbors = (x - 1, y), (x + 1, y), (x, y - 1), (x, y + 1)
        x, y = neighbors['<>^v'.index(direction)]
        yield keypad[y][x]


def paths_between(keypad, start, end):
    x1, y1 = next((x, y) for y, row in enumerate(keypad)
                  for x, key in enumerate(row) if key == start)
    x2, y2 = next((x, y) for y, row in enumerate(keypad)
                  for x, key in enumerate(row) if key == end)
    hor = '<>'[x2 > x1] * abs(x2 - x1)
    ver = '^v'[y2 > y1] * abs(y2 - y1)
    t = tuple(path + 'A' for path in {hor + ver, ver + hor}
                 if ' ' not in walk(keypad, x1, y1, path))
    return t


@cache
def cost_between(keypad, start, end, links):
    return min(cost(DIRECTIONAL, path, links - 1)
               for path in paths_between(keypad, start, end)) if links else 1


@cache
def cost(keypad, keys, links):
    return sum(cost_between(keypad, a, b, links)
               for a, b in pairwise('A' + keys))


def complexity(code, robots):
    return cost(NUMERIC, code, robots + 1) * int(code[:-1])


ans = 0
for c in input:
    # part 1
    # ans += complexity(c, 2)
    # part 2
    ans += complexity(c, 25)

print(ans)
