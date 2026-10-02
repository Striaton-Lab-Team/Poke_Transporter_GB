#!/bin/sh

searchDir=to_compress
fileName=build/compression_chunks.h

truncate -s 0 $fileName

currChunk=""
currArray=""
for entry in "$searchDir"/*
do
  entryName=$(basename ${entry})
  underscoreEntryName=$(printf '%s' "$entryName" | tr '.' '_')

  tmp=${entryName%.*}
  newChunk=${tmp%.*}
  
  if [ "$newChunk" != "$currChunk" ]; then
    if [ -n "$currChunk" ]; then
      printf "${currArray%, }};\n\n" >> $fileName
    fi
    currChunk=$newChunk
    currArray=""
  fi

  if [ "$currArray" = "" ]; then
    currArray="__attribute__((unused))\nstatic const unsigned char *${currChunk}_chunk_list[] = {"
  fi

  fullName=$(printf '%s' "$underscoreEntryName" | sed 's/_bin$/_lz10_bin/')
  printf "#include \"$fullName.h\"\n" >> $fileName
  currArray="$currArray$fullName, "
done
printf "${currArray%, }};\n\n" >> $fileName

