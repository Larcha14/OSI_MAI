#! /usr/bin/env bash

#This script don't require any arguments
#Is needed for deleting all .txt-files in current directory


for file in $( find ./ -type f -name "*"); do
	f="$(basename -- "$file")"
	ext="${f##*.}"
	if [[ "$ext" = "txt" ]]; then
		rm "$f"
		echo "File "$f" was delete successfully"
	fi
done
