import sys
R = open(sys.argv[1],'r')
f1 = open('edges.txt','w')
f2 = open('vertex.txt','w')
for line in R:
  line.strip()
  if len(line) < 5:
    continue
  if line[1:5] == 'node':
    parts = line.split('"')
    f2.write(parts[1]+" "+parts[3] + " " + parts[5] + "\n")
  elif line[1:5] == 'edge':
    parts = line.split('"')
    f1.write(parts[3] + " " + parts[5] + "\n")
