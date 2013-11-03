#Takes the full path of xml file and splits the file and compiles parse.cpp

mkdir Wiki_Split_Files
cd Wiki_Split_Files
split -b 100M --suffix-length=4 $1
cd ..
python quantizing.py "Wiki_Split_Files/"
g++ parse.cpp -w -O3


