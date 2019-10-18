#! /bin/sh

path=$1

if [[ $1 =~ ^.*/$ ]]
then
    random=1 #Does nothing
else
    path="$1"/
    echo "I'm here!"
    echo $path
fi

if [[ $1 =~ ^- ]]
then
    echo "ERROR: FIRST PARAMETER CANNOT START WITH DASH" 1>&2
    exit 1
fi

if [ "$#" -gt 2 ];
then
    echo "ERROR: TWO OR MORE PARAMETERS HAVE BEEN PASSED" 1>&2
    exit 1
fi

if [ -r $path ]
then
    random=1 #Does nothing
else
    echo "ERROR: FILE DOES NOT HAVE READ PERMISSION, PERMISSIONS WILL BE CHANGED" 1>&2
    chmod u+r $path
fi


if [ -z "$path" ]
then
    path="."
    echo "Input was blank, default file path will be used"
fi

if [ -d "$path" ]
then
    cd $path
    touch ~/output.txt
    ls -a | grep -E "[^-\.a-zA-Z_0-9]" >> ~/output.txt
    ls -a | grep -E "^-+" >> ~/output.txt
    ls -a | grep -E "^\.[^\.]+" >> ~/output.txt
    ls -a | grep -E "^\.{2}[^\.]+" >> ~/output.txt 
    ls -a | grep -vE "^.{1,14}$" >> ~/output.txt
    ls -a | sort | uniq -iD >> ~/output.txt
    sed -i -e "s|^|$path|" ~/output.txt
    sort ~/output.txt | uniq -u
    rm ~/output.txt
else
    echo "ERROR: INPUT IS NOT A VALID PATH NAME" 1>&2
    exit 1
fi

if [ -L "$path" ]
then
    echo "ERROR: INPUT IS A SYMBOLIC LINK" 1>&2
    exit 1
fi
