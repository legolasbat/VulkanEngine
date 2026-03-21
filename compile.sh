#!/bin/bash

INPUT="$1"

if [ -z "$INPUT" ]; then
	echo "Usage: $0 <shader file>"
fi

OUTPUT="${INPUT}".spv

/usr/bin/glslc "$INPUT" -o "$OUTPUT"

