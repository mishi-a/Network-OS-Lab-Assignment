#! /bin/bash
# fibonacci sequence 0 1 1 2 3 5 8 .......
echo "Enter an odd number n for Fibonacci series "
read n
x=0
y=1
i=2
#array array stores the fibonacci sequencce
#code segment for fibonacci sequence generation
array[0]=0
array[1]=1
while [ $i -lt $n ]
do
    z=`expr $x + $y `
    array[$i]=$z
    # echo ${array[$i]}
    i=`expr $i + 1 `
    x=$y 
    y=$z
done

echo "Requred series"
# calculate n/3
# array arr stores the element wise sum of first and third segment
#code segment for arr generation
p=`expr $n / 3`
i=0
while [ $i -lt $p ]
do
	((val=i+p+p))
	((arr[$i]=array[$i]+array[$val]))
	# echo ${arr[$i]}
	((i=i+1))
done
i=$p
((x=p+p))
# storing the mid segement array array in array arr
while [ $i -lt $x ]
do
	((arr[$i]=array[$i]))
	# echo ${array[$i]}
	((i=i+1))
done
#Printing the required result
echo ${arr[*]}
