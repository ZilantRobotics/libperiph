#!/bin/bash

for file in $(find $dir -type f);
do
    if [[ $file == ./common* ]] || [[ $file == ./sensors* ]] || [[ $file == ./devices* ]] || [[ $file == ./platform_specific* ]]; then
        compare_result="$(dos2unix < $file | cmp - $file)"
        if [[ $compare_result == *"differ"* ]]; then
            echo $compare_result
        fi
    fi

done
