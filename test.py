a = []

for i in range(128):
   c = chr(i)
   if c.isprintable() and not c.isalnum():
      a.append(i)

a.sort()
a = [chr(x) for x in a]

print(a)