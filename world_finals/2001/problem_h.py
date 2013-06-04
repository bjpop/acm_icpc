import sys

class State(object):
    def __init__(self, bits):
        self.bits = bits

    def comparator(self, wire1, wire2):
        bit1_mask = 1 << (wire1 - 1)
        bit2_mask = 1 << (wire2 - 1)

        bit1 = self.bits & bit1_mask
        bit2 = self.bits & bit2_mask

        # if the first bit is 1 and the second is 0 swap them over
        if bit1 and not bit2:
            self.bits ^= (bit1_mask | bit2_mask)

    def __str__(self):
        result = ''
        for pos in range(32):
            result += str (int (bool ((self.bits & (1 << pos)))))
        return result

def network(bits, links):
    state = State(bits)
    for wire1, wire2 in links:
        state.comparator(wire1, wire2)
    return state

def monotone_increasing(n, bits):
    prev = 0
    for pos in range(n):
        bit = bits & (1 << pos)
        if bit < prev:
            return False
        prev = bit
    return True

def depth(n, links):
    max_depth = 0
    ds = [0] * n
    for wire1, wire2 in links:
        new_depth = max(ds[wire1-1], ds[wire2-1]) + 1
        ds[wire1 - 1] = new_depth
        ds[wire2 - 1] = new_depth
        if new_depth > max_depth:
            max_depth = new_depth
    return max_depth

case_count = 1
while True:
    fields = next(sys.stdin).split()
    n, k = map(int, fields)
    if n == 0 and k == 0:
        break
    links = []
    num_comparators = 0
    while num_comparators < k:
        fields = next(sys.stdin).split()
        while fields:
            links.append(tuple(map(int, fields[0:2])))
            fields = fields[2:]
            num_comparators += 1
    sorting = True
    #for i in range(1 << (n - 1)):
    for i in range(1 << n):
        output = network(i, links)
        if not monotone_increasing(n, output.bits):
            sorting = False
            break
    decision = 'is' if sorting else 'is not'
    time = depth(n, links)
    print('Case {} {} a sorting network and operates in {} time units.'.format(case_count, decision, time))
    case_count += 1
