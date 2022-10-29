#!/bin/bash
# It should be called from can_node folder
cpplint sensors/*/* \
        common/* \
        devices/*/* \
        devices/* \
        platform_specific/* \
        platform_specific/*/* \
        tests/*/*
