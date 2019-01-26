#! /bin/bash
echo "Enter an odd number n for Fibonacci series "
read n
x=0
y=1
i=2
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
while [ $i -lt $x ]
do
	((arr[$i]=array[$i]))
	# echo ${array[$i]}
	((i=i+1))
done
echo ${arr[*]}
