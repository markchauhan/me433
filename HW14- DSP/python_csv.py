import csv

def load_data(file):
    t = [] # column 0
    data1 = [] # column 1
    data2 = [] # column 2

    with open(file, 'r') as f:
        # open the csv file
        reader = csv.reader(f)
        for row in reader:
            # read the rows 1 one by one
            t.append(float(row[0])) # leftmost column
            data1.append(float(row[1])) # second column
         #   data2.append(float(row[2])) # third column

    return t, data1


  #  for i in range(t):
  #      # print the data to verify it was read
  #      print(str(t[i]) + ", " + str(data1[i]) + ", " + str(data2[i]))


timeA, valueA = load_data('/Users/markchauhan/Desktop/Pico/HW14/sigA.csv')
timeB, valueB = load_data('/Users/markchauhan/Desktop/Pico/HW14/sigB.csv')
timeC, valueC = load_data('/Users/markchauhan/Desktop/Pico/HW14/sigC.csv')
timeD, valueD = load_data('/Users/markchauhan/Desktop/Pico/HW14/sigD.csv')


print(f'Signal A: {list(zip(timeA[:5], valueA[:5]))}')
print(f'Signal B: {list(zip(timeB[:5], valueB[:5]))}')
print(f'Signal C: {list(zip(timeC[:5], valueC[:5]))}')
print(f'Signal D: {list(zip(timeD[:5], valueD[:5]))}')
