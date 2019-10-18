#! /bin/sh

if [ "$#" -gt 2 ];
then
    echo "ERROR: TOO MANY PARAMETERS HAVE BEEN PASSED" 1>&2
    exit 1
fi 

if [ $1 = '-r' ];
then
    path=$2
    echo "Running recursive search"
    cd $path
    touch ~/output.txt
    touch ~/output2.txt
 
    find . * | grep -E "\/([^-\.a-zA-Z_]|-+.*|\.[^\.]+|\.{2}[^\.]|\.{3}|[^\/]{15,})" >> ~/output.txt
    find . | sort | uniq -iD >> ~/output.txt

    sort ~/output.txt | uniq -u >> ~/output2.txt    
    cat ~/output2.txt
    rm ~/output.txt
    rm ~/output2.txt
    exit 0
fi

path=$1

if [[ $1 =~ ^.*/$ ]]
then
    random=1 #Does nothing
else
    path="$1"/
    echo $path
fi

if [[ $1 =~ ^- ]]
then
    echo "ERROR: FIRST PARAMETER CANNOT START WITH DASH" 1>&2
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
    ls -a | grep -E "[^-\.a-zA-Z_]|^-+.*|^\.[^\.]+|^\.{2}[^\.]+|^.{15,}" >> ~/output.txt
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
