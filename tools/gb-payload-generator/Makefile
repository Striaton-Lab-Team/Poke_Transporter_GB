SHELL := /bin/bash
.ONESHELL:

OUTPUT_DIR := build

.PHONY: all clean

all:
	@mkdir -p ./$(OUTPUT_DIR)
	shopt -s nullglob
	for filepath in ./src/gen1specific/*.asm
	do
		file="$${filepath##*/}"
		echo -e "\nStarting assembly of $$file\n" >&2
		outputfolder="./$(OUTPUT_DIR)/$${file%%.*}"
		if [ ! -d "$$outputfolder" ]
		then
			mkdir "$$outputfolder"
			echo -e "Creating new output folder at $$outputfolder\n"
		fi
		languages="en fr de it sp jp"
		for language in $${languages}
		do
			versions="r b y"
			if [ $$language == "jp" ]
			then
				versions="r r11 gr gr11 b y y11 y12 y13"
			fi
			for version in $${versions}
			do
				echo "building $${file%%.*}_$${language}_$${version}.bin"
				rgbasm -P ./include/dependencies/$${language}_$${version}.asm -o ./$(OUTPUT_DIR)/output.o $$filepath \
				&& rgblink -x -o $${outputfolder}/$${file%%.*}_$${language}_$${version}.bin ./$(OUTPUT_DIR)/output.o
				[ -f ./$(OUTPUT_DIR)/output.o ] && rm ./$(OUTPUT_DIR)/output.o
	#			if [ $$version = "b" -a -f "$${outputfolder}/$${file%%.*}_$${language}_r.bin" ]
	#			then
	#				cmp -s $${outputfolder}/$${file%%.*}_$${language}_$${version}.bin $${outputfolder}/$${file%%.*}_$${language}_r.bin \
	#				&& rm $${outputfolder}/$${file%%.*}_$${language}_$${version}.bin \
	#				&& echo "$${file%%.*}_$${language}_$${version}.bin is identical to $${file%%.*}_$${language}_r.bin." \
	#				&& echo "Removed $${file%%.*}_$${language}_$${version}.bin"
	#			fi
			done
		done
	done
	
	for filepath in ./src/gen1universal/*.asm ./src/gen2universal/*.asm
	do
		file="$${filepath##*/}"
		echo -e "\nStarting assembly of $$file\n" >&2
		outputfolder="./$(OUTPUT_DIR)/$${file%%.*}"
		if [ ! -d "$$outputfolder" ]
		then
			mkdir "$$outputfolder"
			echo -e "Creating new output folder at $$outputfolder\n"
		fi
		echo "building $${file%%.*}.bin"
		rgbasm -o ./$(OUTPUT_DIR)/output.o $$filepath \
		&& rgblink -x -o $${outputfolder}/$${file%%.*}.bin ./$(OUTPUT_DIR)/output.o
		[ -f ./$(OUTPUT_DIR)/output.o ] && rm ./$(OUTPUT_DIR)/output.o
	done
	
	for filepath in ./src/gen2specific/*.asm
	do
		file="$${filepath##*/}"
		echo -e "\nStarting assembly of $$file\n" >&2
		outputfolder="./$(OUTPUT_DIR)/$${file%%.*}"
		if [ ! -d "$$outputfolder" ]
		then
			mkdir "$$outputfolder"
			echo -e "Creating new output folder at $$outputfolder\n"
		fi
		languages="en fr de it sp jp kor"
		for language in $${languages}
		do
			versions="g s c"
			if [ $$language == "en" ]; then
				versions="g s c c11 cAU"
			elif [ $$language == "jp" ]; then
				versions="g g11 s s11 c"
			elif [ $$language == "kor" ]; then
				versions="g s"
			fi
			for version in $${versions}
			do
				echo "building $${file%%.*}_$${language}_$${version}.bin"
				rgbasm -P ./include/dependencies/$${language}_$${version}.asm -o ./$(OUTPUT_DIR)/output.o $$filepath \
				&& rgblink -x -o $${outputfolder}/$${file%%.*}_$${language}_$${version}.bin ./$(OUTPUT_DIR)/output.o
				[ -f ./$(OUTPUT_DIR)/output.o ] && rm ./$(OUTPUT_DIR)/output.o
	#			if [ $$version = "b" -a -f "$${outputfolder}/$${file%%.*}_$${language}_r.bin" ]
	#			then
	#				cmp -s $${outputfolder}/$${file%%.*}_$${language}_$${version}.bin $${outputfolder}/$${file%%.*}_$${language}_r.bin \
	#				&& rm $${outputfolder}/$${file%%.*}_$${language}_$${version}.bin \
	#				&& echo "$${file%%.*}_$${language}_$${version}.bin is identical to $${file%%.*}_$${language}_r.bin." \
	#				&& echo "Removed $${file%%.*}_$${language}_$${version}.bin"
	#			fi
			done
		done
	done

clean:
	@rm -rf ./$(OUTPUT_DIR)
