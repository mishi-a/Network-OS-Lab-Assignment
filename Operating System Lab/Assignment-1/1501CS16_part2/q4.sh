#! /bin/bash
####sentence : anything ending with full stop is considered sentence here
#this conditional if will check whether our empty file i.e result.txt is empty or not if not make empty

if [ -s ./OS/result.txt ]
then
	echo -n "" > ./OS/result.txt
fi
# os folder contai four nonempty file :-1)1.txt,2)2.txt,3)3.txt,4)4.txt and empty file namely result.txt
#file contain the file in ascending order of size 
file=`ls -r -S ./OS/ | tail -4`
# for each file in file var it opened it copy the first sentence which is marked by full stop
# and append it to empty file result.txt
for line in $file
do
	cat < ./OS/$line | tr "\n"  " " | tr "." "\n"  | head -1 |tr "\n" "." >> ./OS/result.txt
	#this helps to print every sentence in  new line
	echo " " >>./OS/result.txt    
done