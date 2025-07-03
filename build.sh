#!/bin/bash

echo "🚀 Building TransitFlow Terminal App..."

# Set source and output
SRC_DIR="src"
OUTPUT="$SRC_DIR/TransitFLOW.exe"

# Collect all .cpp files recursively inside src/
CPP_FILES=$(find $SRC_DIR -name "*.cpp")

# Compile
g++ $CPP_FILES -o $OUTPUT

# Check if build succeeded
if [ $? -eq 0 ]; then
    echo "✅ Build successful!"
    echo "🔁 Running the program..."
    echo "---------------------------"
    ./$OUTPUT
else
    echo "❌ Build failed!"
fi
