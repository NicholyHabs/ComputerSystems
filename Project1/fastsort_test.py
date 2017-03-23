from subprocess import call
import os


def isSorted(filename, size):

  f = open(filename)

  prev_value = -1;
  n_lines = 0

  for line in f:
    line = line.strip();

    if int(line) < prev_value:
      return False

    prev_value = int(line)
    n_lines += 1


  if n_lines != size:
    return False

  return True  


#--- Main
os.system('make clean')
os.system('make')

os.system('gcc -o generate generate.c')
os.system('gcc -o dump dump.c')

size_list = [100, 10000, 1, 333, 33333];
num_passed = 0

for size in size_list:
  infile = './input' + str(size)

  if size == 333 or size == 33333:
        seed = 3
  else:
        seed = 0

  call(['./generate', '-n', str(size), '-s', str(seed), '-o', infile])

  call(['./fastsort', '-i', infile, '-o', 'output'])
  os.system('./dump -i output > dump.txt')

  if isSorted('dump.txt', size):
    num_passed += 1
    print 'Passed for size ' + str(size)
  else:
    print 'Failed for size ' + str(size)

print 'Passed ' + str(num_passed) + '/5 sort tests'

os.system('./fastsort')
os.system('./fastsort -i /no/such/file -o output')
