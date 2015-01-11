#!/bin/sh

#TODO: whitespace in subject

declare -i	min=0
declare -i	max=0
declare -i	fileLen=0
declare -i	subjLen=0
declare -i	semicolon=0
declare -i	boundlength=0
declare -i	maxLength=0
declare -i	minlength=0
declare -i	count=1

# initialize: -c
if test $# -eq 2 && test $1 = "-c"
then
	if test -d $2
	then
		echo "error: directory $2 already exists"
	else
		mkdir $2
		echo "$min:$max" > $2/bounds
	fi
#

# add message: -a -s
elif test $# -ge 5 && test -d $1 && test -f $1/bounds && test $2 = "-a" && test -f $3 && test $4 = "-s"
then
	#get max and min from $1/bounds
	read -r tbounds < $1/bounds
	col=`expr index $tbounds ':'`
	min=${tbounds:0:$col-1}
	max=${tbounds:$col}
	#get and test length of file name and subject
	fileLen=`expr length $3`
	subjLen=`expr length $5`
	if test $subjLen -le 31 && test $fileLen -le 511
	# all is well
	then
		if test $min -eq 0
		then
			min=1
		fi
		max+=1
		# reset needed arguments
		one=$1
		three=$3
		subj=$5
		shift 5
		#concatenate subject whitespace
		until test $# -eq 0
		do
			subj="$subj $1"
			shift
		done
		echo "subject: $subj" > $one/$max
		strDate=$(date)
		echo "date: $strDate" >> $one/$max
		echo "=====" >> $one/$max
		cat $three >> $one/$max
		echo "$min:$max" > $one/bounds
	# something is wrong
	else
		echo "file name and/or subject is too long"
	fi
#	

# add message: -s -a
elif test $# -ge 5 && test -d $1 && test -f $1/bounds && test $2 = "-s"
then
	#get max and min from $1/bounds
	read -r tbounds < $1/bounds
	col=`expr index $tbounds ':'`
	min=${tbounds:0:$col-1}
	max=${tbounds:$col}
	#get and test length of file name and subject
	fileLen=`expr length $3`
	subjLen=`expr length $5`
	if test $subjLen -le 31 && test $fileLen -le 511
	# all is well
	then
		if test $min -eq 0
		then
			min=1
		fi
		max+=1
		# reset needed arguments
		one=$1
		subj=$3
		until test $# -eq 5
		do
			subj="$subj $4"
			shift
		done
		echo "subject: $subj" > $one/$max
		strDate=$(date)
		echo "date: $strDate" >> $one/$max
		echo "=====" >> $one/$max
		cat $5 >> $one/$max
		echo "$min:$max" > $one/bounds
	# something is wrong
	else
		echo "file name and/or subject is too long"
	fi
#

# remove message: -d
elif test $# -eq 3 && test -d $1 && test $2 = "-d"
then
	if test -f $1/$3
	then
		#get max and min from $1/bounds
		read -r tbounds < $1/bounds
		col=`expr index $tbounds ':'`
		min=${tbounds:0:$col-1}
		max=${tbounds:$col}
		# remove file
		rm $1/$3	
		# face consequences
		if test $min -eq $max
		then
			min=0
			max=0
		elif test $min -eq $3
		then
			min+=1
		else
			val=`expr $3 - 1`
			for i in $(eval echo {$val..$min})
			do
				x=`expr $i + 1`
				mv $1/$i $1/$x
			done
			min=`expr $min + 1`
		fi
		echo "$min:$max" > $1/bounds
	else
		echo "error: $3 is not in the archive"
	fi
#

# print subject list: -S
elif test $# -eq 2 && test -d $1 && test $2 = "-S"
then
	read -r tempBounds < $1/bounds
	col=`expr index $tempBounds ':'`
	min=${tempBounds:0:$col-1}
	max=${tempBounds:$col}
	if test $min -eq 0
	then
		exit
	fi
	for i in $( eval echo {$min..$max} )
	do
		read -r tFile < $1/$i
		subj=${tFile:9}
		date=$(head -n 2 $1/$i | tail -n 1)
		echo -e "$i\t$subj\t${date:5}"
	done
#

# search subjects: -ss
elif test $# -ge 3  && test -d $1 && test $2 = "-ss"
then

		read -r tempBounds < $1/bounds
		col=`expr index $tempBounds ':'`
		max=${tempBounds:$col}
		dir=$1
		shift 2
		search=$1
		shift
		until test $# -eq 0
		do
			search="$search $1"
			shift
		done

		while test $count -le $max
		do
			if test -f $dir/$count
			then
				read -r word < $dir/$count
				echo $word > $dir/temp2
				grep -c "$search" $dir/temp2 > $dir/temp
				read -r countStrings < $dir/temp

				if test $countStrings -gt 0
				then
					echo "$count"
				fi
				rm -f $dir/temp
				rm -f $dir/temp2
			fi
		countString=0
		count+=1
		done
#

# search bodies: -sb
elif test $# -ge 3  && test -d $1 && test $2 = "-sb"
then
		
		read -r tempBounds < $1/bounds
		col=`expr index $tempBounds ':'`
		max=${tempBounds:$col}
		dir=$1
		shift 2
		search=$1
		shift
		until test $# -eq 0
		do
			search="$search $1"
			shift
		done

		while test $count -le $max
		do
			if test -f $dir/$count
			then
				cat $dir/$count >> $dir/temp2
				sed -i -e "1d" $dir/temp2
				sed -i -e "1d" $dir/temp2
				sed -i -e "1d" $dir/temp2
				grep -c "$search" $dir/temp2 > $dir/temp
				read -r countString < $dir/temp
				if test $countString -gt 0
				then
					echo "$count"
				fi
				rm -f $dir/temp
				rm -f $dir/temp2
			fi
		countString=0
		count+=1
		done
#

# take care of all error cases
else
	echo "error: invalid argument"

fi