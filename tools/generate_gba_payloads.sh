#!/bin/bash
# This script generates the payloads using gba-payload-generator
# and pack the content.
# The english payload will be stored just compressed.
# but the other payloads will be turned into BPS patches based on the english payload, which will then be packed into
# a compressed filecontainer
# This is all done to keep the file sizes as small as possible
# The script requires 2 arguments: 
# 1.) the path to the build dir.
# 2.) the path to the uncompressed RSEFRLG text table bin file
BUILD_DIR=$1
RSEFRLG_BIN_PATH=$2
RAW_PAYLOAD_DIR=$1/gba-payloads
BPS_PATCH_DIR=$1/bps-patches

ABSOLUTE_BASE_VARIANT=ruby_english_1_0.bin

games=("ruby" "sapphire" "emerald" "firered" "leafgreen")
langs=("english" "french" "german" "italian" "japanese" "spanish")

# This function uses the flips application to generate a BPS patch
create_patch(){
    flips --create --bps $1 $2 $3
}

determine_base_section30(){
	case "$1" in
		*ruby*|*sapphire*)
			printf '%s\n' "section30_ruby_english_1_0.bin"
			;;
		*emerald*)
			printf '%s\n' "section30_emerald_english_1_0.bin"
			;;
		*firered*|*leafgreen*)
			printf '%s\n' "section30_firered_english_1_0.bin"
			;;
		*)
			return 1
			;;
	esac
}

determine_base_script(){
	case "$1" in
		*ruby*|*sapphire*)
			printf '%s\n' "script_ruby_english_1_0.bin"
			;;
		*emerald*)
			printf '%s\n' "script_emerald_english_1_0.bin"
			;;
		*firered*|*leafgreen*)
			printf '%s\n' "script_firered_english_1_0.bin"
			;;
		*)
			return 1
			;;
	esac
}

# fail the script if any command fails
set -e

mkdir -p $RAW_PAYLOAD_DIR
mkdir -p $BPS_PATCH_DIR
tools/gba-payload-generator/gba-payload-generator $RSEFRLG_BIN_PATH $RAW_PAYLOAD_DIR

# first copy our absolute base payload to to_compress
cp $RAW_PAYLOAD_DIR/script_$ABSOLUTE_BASE_VARIANT to_compress/
cp $RAW_PAYLOAD_DIR/section30_$ABSOLUTE_BASE_VARIANT to_compress/


# First generate base payloads patches for the other games. We only need 2:
# Convert from Ruby to Emerald
# Convert from Ruby to FireRed
#
# All the language specific ones can be based on those 3 variants (english included)
create_patch "$RAW_PAYLOAD_DIR/script_ruby_english_1_0.bin" "$RAW_PAYLOAD_DIR/script_emerald_english_1_0.bin" "$BPS_PATCH_DIR/script_emerald_english_1_0.bps"
create_patch "$RAW_PAYLOAD_DIR/script_ruby_english_1_0.bin" "$RAW_PAYLOAD_DIR/script_firered_english_1_0.bin" "$BPS_PATCH_DIR/script_firered_english_1_0.bps"
create_patch "$RAW_PAYLOAD_DIR/section30_ruby_english_1_0.bin" "$RAW_PAYLOAD_DIR/section30_emerald_english_1_0.bin" "$BPS_PATCH_DIR/section30_emerald_english_1_0.bps"
create_patch "$RAW_PAYLOAD_DIR/section30_ruby_english_1_0.bin" "$RAW_PAYLOAD_DIR/section30_firered_english_1_0.bin" "$BPS_PATCH_DIR/section30_firered_english_1_0.bps"

# Generate patches for every game and language variant. Each variant is based on
# the smallest appropriate English game-family payload.
for i in "${games[@]}"
do
	for j in "${langs[@]}"
	do
		specific_script_identifier="script_${i}_${j}*"
		specific_section30_identifier="section30_${i}_${j}*"

		find $RAW_PAYLOAD_DIR -name $specific_script_identifier | while read filename; do
			file_basename=$(basename $filename)
			patch_file=${file_basename%.*}.bps
			base_filename=$(determine_base_script "$filename") || exit 1
			base_path="$RAW_PAYLOAD_DIR/$base_filename"

			# avoid replacing base payload patch files created above.
			if [[ -f "$BPS_PATCH_DIR/$patch_file" ]]; then
				continue
			fi
			
			echo "Creating patch for $filename based on $base_path"
			create_patch "$base_path" "$filename" "$BPS_PATCH_DIR/$patch_file"
		done
		
		find $RAW_PAYLOAD_DIR -name $specific_section30_identifier | while read filename; do
			file_basename=$(basename $filename)
			patch_file=${file_basename%.*}.bps
			base_filename=$(determine_base_section30 "$filename") || exit 1
			base_path="$RAW_PAYLOAD_DIR/$base_filename"

			# avoid replacing base payload patch files created above.
			if [[ -f "$BPS_PATCH_DIR/$patch_file" ]]; then
				continue
			fi
			
			echo "Creating patch for $filename based on $base_path"
			create_patch "$base_path" "$filename" "$BPS_PATCH_DIR/$patch_file"
		done
	done
done

# we want to fill up the containerdef files separately from the BPS file generation, because we want the containerdef file ordered by language.
# For optimal compression, we want to keep the same language game variants stored contiguously, because
# the translation texts from the RSEFRLG text table will be mostly the same. Therefore compression could leverage that redundancy.
for j in "${langs[@]}"
do
	for i in "${games[@]}"
	do
		script_identifier="script_${i}_${j}*"
		section30_identifier="section30_${i}_${j}*"
		find $BPS_PATCH_DIR -name $script_identifier | LC_ALL=C sort | while read filename; do
			echo $filename >> $BPS_PATCH_DIR/script_patches.containerdef
		done
		find $BPS_PATCH_DIR -name $section30_identifier | LC_ALL=C sort | while read filename; do
			echo $filename >> $BPS_PATCH_DIR/section30_patches.containerdef
		done
	done
done
