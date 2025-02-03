


with open("large.input", "r") as f:
    l1 = []
    l2 = []
    for line in f:
        i1, i2 = line.split()
        l1.append(int(i1))
        l2.append(int(i2))
    
    l1.sort()
    l2.sort()

    total = 0
    for num1, num2 in zip(l1, l2):
        total += abs(num1 - num2)
    
    print(total)

    similarity = 0
    for num in l1:
        similarity += num * l2.count(num)
    
    print(similarity)

