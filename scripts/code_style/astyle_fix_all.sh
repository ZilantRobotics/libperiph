#!/bin/bash
# Check http://astyle.sourceforge.net/astyle.html
 

ASTYLE_SCRIPT_PATH=$(dirname $(readlink -f $0))/astyle_fix_single_file.sh
WS_DIR=$(dirname $(readlink -f $0))/../..
cd $WS_DIR
