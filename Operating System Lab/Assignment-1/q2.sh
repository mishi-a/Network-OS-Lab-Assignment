#! /bin/sh
echo "Number of Unique words "
cat  < welcome.txt|tr -d ['.''('')'',']|tr ' ' '\n'| sort -u | wc -w

echo "Maximum frequency and Occurence word "

tr ['.''('')'','] '\0' < welcome.txt|tr ' ' '\n'| sort | uniq -c | sort -nr | head -1