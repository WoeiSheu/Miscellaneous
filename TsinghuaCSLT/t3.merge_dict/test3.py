f1 = open("./test3.1.txt","r");
f2 = open("./test3.2.txt","r");

i = 0;
dict1 = {};
while True:
    line1 = f1.readline();
    if line1 == "\n":
        continue;
    elif line1 != "" :
        pass
        i += 1;
        temp = line1.split(' ');
        dict1[temp[0]] = float(temp[1]);
    else:
        break;

i = 0;
dict2 = {};
while True:
    line2 = f2.readline();
    if line2 == "\n":
        continue;
    elif line2 != "" :
        pass
        i += 1;
        temp = line2.split(' ');
        dict2[temp[0]] = float(temp[1]);
    else:
        break;

print "dict1:  ",dict1,'\n','\n',"dict2:  ",dict2;
f1.close();
f2.close();

dict3 = {};
for key in dict1:
    if dict3.has_key(key):
        dict3[key] += dict1[key];
    else:
        dict3[key] = dict1[key];

for key in dict2:
    if dict3.has_key(key):
        dict3[key] += dict2[key];
    else:
        dict3[key] = dict2[key];
        
print "dict3:  ",dict3;

f3 = open("./test3.txt","w");
for key in dict3:
    f3.write(key);
    f3.write(" ");
    f3.write(str(dict3[key]));
    f3.write("\n");
    
stop = raw_input("Enter to stop.");
