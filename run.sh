# Creates all the index files

echo 'Running...'

mkdir Index

i=0
#Creates Index files for the files in Wiki_Split_Files
for file in `ls Wiki_Split_Files/`
do
    ./a.out Wiki_Split_Files/$file Index/$i
		i=`expr $i + 1`
done

echo 'Created Indices'


#Merges all the index files
g++ merge.cpp
./a.out $i

echo 'Merged Indices'

#Creates Dense Index for Words
g++ dense_index.cpp
./a.out

echo 'Created Dense Index'

#Creates Dense Index for Titles
g++ dense_index_id_title.cpp
./a.out

echo 'Created Dense Title Index'

#Creates Secondary Index for Words
g++ secondary_index.cpp
./a.out

echo 'Created Secondary Index'

#Creates Secondary Index for Titles
g++ secondary_index_id_title.cpp
./a.out

echo 'Created Secondary Title Index'


