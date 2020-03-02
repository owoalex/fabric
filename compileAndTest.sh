#!/bin/bash
if make; then
    build/fabric -f "./data"
else
    echo "BUILD FAILED"
fi
