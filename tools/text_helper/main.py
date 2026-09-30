import pandas as pd
import os
from enum import Enum
import json
import requests
import sys
from pathlib import Path
import hashlib
import math
import png
import debugpy
from dataclasses import dataclass

class Languages(Enum):
    Japanese = 0
    English = 1
    French = 2
    German = 3
    Italian = 4
    SpanishEU = 5
    SpanishLA = 6
    Korean = 7
    ChineseSI = 8
    ChineseTR = 9
    PortugueseBR = 10

LanguageStringMap = {
    "japanese": Languages.Japanese,
    "english": Languages.English,
    "french": Languages.French,
    "german": Languages.German,
    "italian": Languages.Italian,
    "spanishEU": Languages.SpanishEU,
    "spanishLA": Languages.SpanishLA,
    "korean": Languages.Korean,
    "chineseSI": Languages.ChineseSI,
    "chineseTR": Languages.ChineseTR,
    "portugueseBR": Languages.PortugueseBR,
}

class Font:
    def __init__(self, fileName, numColors, numChars, numCharsX, numCharsY, cellWidth, cellHeight, charWidth, charHeight):
        self.fileName = fileName
        self.numColors = numColors
        self.numChars = numChars
        self.numCharsX = numCharsX
        self.numCharsY = numCharsY
        self.cellWidth = cellWidth
        self.cellHeight = cellHeight
        self.charWidth = charWidth
        self.charHeight = charHeight

        self.bpp = int(math.log(numColors, 2)) + 1
        self.numWords = self.numChars * self.cellWidth * self.cellHeight * self.bpp // (8 * 4)
        self.numBytes = self.numWords * 4

        self.charWordTable = [0] * self.numWords
        self.charWidthTable = [0] * self.numBytes

class LanguageConfig:
    def __init__(self, language, column_aliases, char_array, token_indexes=None):
        self.language = language
        self.column_aliases = column_aliases
        self.char_array = char_array
        self.token_indexes = token_indexes

PURPOSEFUL_SPACE_CHAR = '|'
BG_PAL_INDEX = 0
TOKEN_NEWLINE = "\uE000"
TOKEN_BOX_BREAK = "\uE001"
TOKEN_SCROLL_BREAK = "\uE002"
TOKEN_CENTER_ON = "\uE003"
TOKEN_CENTER_OFF = "\uE004"
TOKEN_PRESERVED_BLANK_LINE = "\uE005"

FORMAT_TOKEN_TO_BYTE_CHAR = {
    TOKEN_NEWLINE: "Ň",
    TOKEN_BOX_BREAK: "ȼ",
    TOKEN_SCROLL_BREAK: "Ş",
    TOKEN_CENTER_ON: "ɑ",
    TOKEN_CENTER_OFF: "Ω",
}
BYTE_CHAR_TO_FORMAT_TOKEN = {value: key for key, value in FORMAT_TOKEN_TO_BYTE_CHAR.items()}

def format_internal_tokens(text):
    for token, byte_char in FORMAT_TOKEN_TO_BYTE_CHAR.items():
        text = text.replace(token, byte_char)
    return text

def normalize_control_glyphs_to_tokens(text):
    for byte_char, token in BYTE_CHAR_TO_FORMAT_TOKEN.items():
        text = text.replace(byte_char, token)
    return text

def coerce_to_bool(value):
    if pd.isna(value):
        return False
    if isinstance(value, str):
        return value.strip().lower() in {"1", "true", "yes", "y"}
    return bool(value)

BASE_DIR = Path(__file__).resolve().parent
BUILD_DIR = BASE_DIR / "build"
GEN_DIR = BASE_DIR.parent.parent / "build" / "generated"
BUILD_DIR.mkdir(parents=True, exist_ok=True)
GEN_DIR.mkdir(parents=True, exist_ok=True)
TRANSLATED_H_PATH = GEN_DIR / "translated_text.h"
TRANSLATED_CPP_PATH = GEN_DIR / "translated_text.cpp"
FONTS_H_PATH = GEN_DIR / "fonts.h"
OUTPUT_JSON_PATH = BUILD_DIR / "output.json"
THIS_SCRIPT_PATH = BASE_DIR / "main.py"

XLSX_URL = 'https://docs.google.com/spreadsheets/d/14LLs5lLqWasFcssBmJdGXjjYxARAJBa_QUOUhXZt4v8/export?format=xlsx'
NEW_TEXT_XLSX_PATH = BASE_DIR / 'new_text.xlsx'
TEXT_XLSX_PATH = BASE_DIR / 'text.xlsx'

def parse_build_args(argv):
    if len(argv) >= 4:
        return argv[1], argv[2], argv[3]
    return "", "debug", "local"  # BUILD_LANG not implemented yet

def normalize_column_name(name):
    return str(name).strip().lower()

def normalize_box_type_header(name):
    return "".join(ch for ch in str(name).lower() if ch.isalnum())

def find_required_box_type_column(columns_by_normalized, required_key):
    matches = []
    for normalized, col in columns_by_normalized.items():
        if required_key == "numLines":
            if "line" in normalized and ("num" in normalized or "number" in normalized) and "pixel" not in normalized:
                matches.append(col)
        elif required_key == "pixelsPerChar":
            if "pixel" in normalized and "char" in normalized:
                matches.append(col)
        elif required_key == "pixelsInLine":
            if "pixel" in normalized and "line" in normalized:
                matches.append(col)
        elif required_key == "includeBoxBreaks":
            if "box" in normalized and "break" in normalized:
                matches.append(col)
        elif required_key == "includeScrolling":
            if "scroll" in normalized:
                matches.append(col)
        elif required_key == "boxStyle":
            if "style" in normalized:
                matches.append(col)
        elif required_key == "verticallyCenterText":
            if "vertical" in normalized and "center" in normalized and "text" in normalized:
                matches.append(col)
    if len(matches) == 1:
        return matches[0]
    if len(matches) > 1:
        raise KeyError(f"Multiple Box Types columns match '{required_key}': {matches}")
    return None

def find_column_by_aliases(columns, aliases):
    normalized_columns = {normalize_column_name(col): col for col in columns}
    for alias in aliases:
        match = normalized_columns.get(normalize_column_name(alias))
        if match is not None:
            return match
    raise KeyError(f"Could not find column matching aliases: {aliases}")

def find_optional_column_by_aliases(columns, aliases):
    try:
        return find_column_by_aliases(columns, aliases)
    except KeyError:
        return None

def sanitize_macro_token(text):
    out = ""
    for char in str(text).upper():
        out += char if char.isalnum() else "_"
    while "__" in out:
        out = out.replace("__", "_")
    return out.strip("_")

def sanitize_c_identifier(text):
    out = ""
    for char in str(text).lower():
        out += char if char.isalnum() else "_"
    while "__" in out:
        out = out.replace("__", "_")
    out = out.strip("_")
    if not out:
        out = "unnamed"
    if out[0].isdigit():
        out = "n_" + out
    return out

@dataclass
class TextBuildContext:
    storage: dict

    def initialize_storage(self, sections):
        self.storage.clear()
        for lang in Languages:
            self.storage[lang.name] = {section: {} for section in sections}
            self.storage[lang.name]["Warnings"] = {}
            self.storage[lang.name]["Errors"] = {}

    def log(self, lang, level, text, entry_id=None):
        bucket = level + "s"
        prefix = f"[{entry_id}] " if entry_id is not None and str(entry_id).strip() != "" else ""
        message = prefix + level + ": " + format_internal_tokens(text)
        if message not in self.storage[lang.name][bucket].values():
            self.storage[lang.name][bucket][max(self.storage[lang.name][bucket].keys(), default=-1) + 1] = message

mainDict = {}
build_context = TextBuildContext(mainDict)
textSections = []
boxTypeDefinitions = {}
boxTypeNames = []
boxTypeIdByName = {}
boxTypeValueKeys = []
boxTypeValueMeta = []
fonts = {
    "International": Font("latin_normal", 1, 256, 16, 16, 16, 16, 16, 16),
    "Japanese": Font("japanese_normal", 1, 256, 16, 16, 16, 16, 16, 16),
}

charArrays = {
    "International": {
        "array": [0] * 0x100,
        "font": fonts["International"],
        "escape": [
            ["{SCL}", [0xFA]],
            ["{CLR}", [0xFB]],
            ["{DEF}", [0xFC, 0x01, 0x02]],
            ["{FEM}", [0xFC, 0x01, 0x04]],
            ["{FPC}", [0xFC, 0x01, 0x06]],
            ["{MLE}", [0xFC, 0x01, 0x08]],
            ["{SPA}", [0xFC]],
            ["{PLR}", [0xFD, 0x01]],
            ["{VLN}", [0xFD, 0x0A]], # Displays MAXIE in Ruby or ARCHIE in Sapphire and Emerald 
            ["{VLN2}", [0xFD, 0x0B]], # Displays ARCHIE in Ruby or MAXIE in Sapphire and Emerald. In our case, have purpose in Emerlad only.
            ["{NEW}", [0xFE]],
            ["{END}", [0xFF]],
        ]
    },
    "Japanese": {
        "array": [0] * 0x100,
        "font": fonts["Japanese"],
        "escape": [
            ["{SCL}", [0xFA]],
            ["{CLR}", [0xFB]],
            ["{DEF}", [0xFC, 0x06, 0x02]],
            ["{FEM}", [0xFC, 0x06, 0x03]], # ???
            ["{MLE}", [0xFC, 0x06, 0x04]],
            ["{SPA}", [0xFC]],
            ["{FPC}", [0xFC, 0x06, 0x05]],
            ["{PLR}", [0xFD, 0x01]],
            ["{VLN}", [0xFD, 0x0A]], # Displays MAXIE in Ruby or ARCHIE in Sapphire and Emerald 
            ["{VLN2}", [0xFD, 0x0B]], # Displays ARCHIE in Ruby or MAXIE in Sapphire and Emerald. Have purpose in Emerlad only.
            ["{NEW}", [0xFE]],
            ["{END}", [0xFF]],
        ]
    },
}

LANGUAGE_CONFIGS = {
    Languages.Japanese: LanguageConfig(Languages.Japanese, ("Japanese",), charArrays["Japanese"], (0x7A, 0x99, 0x79)),
    Languages.English: LanguageConfig(Languages.English, ("English",), charArrays["International"], (0x30, 0x60, 0x70)),
    Languages.French: LanguageConfig(Languages.French, ("French",), charArrays["International"], (0x31, 0x60, 0x71)),
    Languages.German: LanguageConfig(Languages.German, ("German",), charArrays["International"], (0x32, 0x61, 0x72)),
    Languages.Italian: LanguageConfig(Languages.Italian, ("Italian",), charArrays["International"], (0x33, 0x60, 0x71)),
    Languages.SpanishEU: LanguageConfig(Languages.SpanishEU, ("Spanish (EU)",), charArrays["International"], (0x34, 0x60, 0x72)),
    Languages.SpanishLA: LanguageConfig(Languages.SpanishLA, ("Spanish (LA)",), charArrays["International"], (0x34, 0x60, 0x72)),
    Languages.Korean: LanguageConfig(Languages.Korean, ("Korean",), charArrays["International"]),
    Languages.ChineseSI: LanguageConfig(Languages.ChineseSI, ("Chinese (Simplified)",), charArrays["International"]),
    Languages.ChineseTR: LanguageConfig(Languages.ChineseTR, ("Chinese (Traditional)",), charArrays["International"]),
    Languages.PortugueseBR: LanguageConfig(Languages.PortugueseBR, ("Brazilian Portuguese",), charArrays["International"]),
}

def get_language_config(lang):
    return LANGUAGE_CONFIGS[lang]

charConversionList = [
    # replaces the first char in the list with the latter
    ["'", "’"],
]

def split_into_sentences(text: str) -> list[str]:
    # -*- coding: utf-8 -*-
    import re
    alphabets= r"([A-Za-z])"
    prefixes = r"(Mr|St|Mrs|Ms|Dr)[.]"
    suffixes = r"(Inc|Ltd|Jr|Sr|Co)"
    starters = r"(Mr|Mrs|Ms|Dr|Prof|Capt|Cpt|Lt|He\s|She\s|It\s|They\s|Their\s|Our\s|We\s|But\s|However\s|That\s|This\s|Wherever)"
    acronyms = r"([A-Z][.][A-Z][.](?:[A-Z][.])?)"
    websites = r"[.](com|net|org|io|gov|edu|me)"
    digits = r"([0-9])"
    multiple_dots = r'\.{2,}'

    """
    Split the text into sentences.

    If the text contains substrings "<prd>" or "<stop>", they would lead 
    to incorrect splitting because they are used as markers for splitting.

    :param text: text to be split into sentences
    :type text: str

    :return: list of sentences
    :rtype: list[str]
    """
    text = " " + text + "  "
    text = text.replace("\n"," ")
    text = re.sub(prefixes,"\\1<prd>",text)
    text = re.sub(websites,"<prd>\\1",text)
    text = re.sub(digits + "[.]" + digits,"\\1<prd>\\2",text)
    text = re.sub(multiple_dots, lambda match: "<prd>" * len(match.group(0)) + "<stop>", text)
    if "Ph.D" in text: text = text.replace("Ph.D.","Ph<prd>D<prd>")
    text = re.sub(r"\s" + alphabets + "[.] "," \\1<prd> ",text)
    text = re.sub(acronyms+" "+starters,"\\1<stop> \\2",text)
    text = re.sub(alphabets + "[.]" + alphabets + "[.]" + alphabets + "[.]","\\1<prd>\\2<prd>\\3<prd>",text)
    text = re.sub(alphabets + "[.]" + alphabets + "[.]","\\1<prd>\\2<prd>",text)
    text = re.sub(" "+suffixes+"[.] "+starters," \\1<stop> \\2",text)
    text = re.sub(" "+suffixes+"[.]"," \\1<prd>",text)
    text = re.sub(" " + alphabets + "[.]"," \\1<prd>",text)
    if "”" in text: text = text.replace(".”","”.")
    if "\"" in text: text = text.replace(".\"","\".")
    if "!" in text: text = text.replace("!\"","\"!")
    if "?" in text: text = text.replace("?\"","\"?")
    if "。" in text: text = text.replace("。\"","\"。") # Added for Japanese support
    if "？" in text: text = text.replace("？\"","\"？") # Added for Japanese support
    if "！" in text: text = text.replace("！\"","\"！") # Added for Japanese support
    text = text.replace(".",".<stop>")
    text = text.replace("?","?<stop>")
    text = text.replace("!","!<stop>")
    text = text.replace("。","。<stop>") # Added for Japanese support
    text = text.replace("？","？<stop>") # Added for Japanese support
    text = text.replace("！","！<stop>") # Added for Japanese support
    text = text.replace("<prd>",".")
    for token in (
        TOKEN_NEWLINE,
        TOKEN_BOX_BREAK,
        TOKEN_SCROLL_BREAK,
        TOKEN_CENTER_OFF,
        TOKEN_CENTER_ON,
    ):
        text = text.replace(token, f"<stop>{token}<stop>")

    sentences = text.split("<stop>")
    sentences = [s.strip() for s in sentences]
    if sentences and not sentences[-1]: sentences = sentences[:-1]
    return remove_redundant_terminal_breaks(sentences)

def remove_redundant_terminal_breaks(sentences: list[str]) -> list[str]:
    cleaned_sentences = []
    for index, sentence in enumerate(sentences):
        look_ahead_index = index + 1
        while look_ahead_index < len(sentences) and sentences[look_ahead_index] == TOKEN_CENTER_OFF:
            look_ahead_index += 1

        if sentence == TOKEN_CENTER_OFF and (
            look_ahead_index >= len(sentences) or sentences[look_ahead_index] in (TOKEN_BOX_BREAK, TOKEN_SCROLL_BREAK)
        ):
            continue

        if sentence not in (TOKEN_NEWLINE, TOKEN_SCROLL_BREAK):
            cleaned_sentences.append(sentence)
            continue

        if look_ahead_index >= len(sentences):
            continue

        if sentences[look_ahead_index] in (TOKEN_BOX_BREAK, TOKEN_SCROLL_BREAK):
            continue

        cleaned_sentences.append(sentence)

    return cleaned_sentences
 
def split_sentence_into_lines(sentence, offset, pixelsPerChar, pixelsInLine, centered, lang, currLineCount, numLines, entry_id=None, context=None):
    outStr = ""
    currLine = ""
    lineCount = 0
    currWordIndex = 0
    lineLength = 0
    spaceLength = 0

    language_config = get_language_config(lang)
    language_char_array = language_config.char_array

    def format_output_line(line, line_pixel_length, trim_trailing_space=False):
        if trim_trailing_space and line.endswith(" "):
            line = line[:-1]
            line_pixel_length -= spaceLength

        if centered and line:
            count = ((pixelsInLine - line_pixel_length) // 2)
            line = f'_[{count}]{line}'
            line_pixel_length += count
        return line, line_pixel_length

    # A centered block may get split into multiple sentences for wrapping, but each
    # centered sentence still needs to begin at a real line start.
    if centered and offset != 0 and sentence not in [
        TOKEN_CENTER_ON,
        TOKEN_CENTER_OFF,
        TOKEN_BOX_BREAK,
        TOKEN_NEWLINE,
        TOKEN_SCROLL_BREAK,
        '',
    ]:
        outStr += TOKEN_NEWLINE
        lineCount += 1
        offset = 0
        if sentence.startswith(PURPOSEFUL_SPACE_CHAR):
            sentence = sentence[1:]

    words = sentence.split()


    while(currWordIndex < len(words)):
        word = words[currWordIndex]

        # See if the whole sentence is a newline or scroll
        if (sentence == TOKEN_NEWLINE or sentence == TOKEN_SCROLL_BREAK):
            if (sentence == TOKEN_NEWLINE):
                outStr += TOKEN_NEWLINE
            elif (sentence == TOKEN_SCROLL_BREAK):
                outStr += TOKEN_SCROLL_BREAK
            currLine = ""
            lineCount += 1
            offset = 0
            lineLength = 0
            currWordIndex += 1

        # See if the whole sentence is a center character
        elif (sentence == TOKEN_CENTER_ON or sentence == TOKEN_CENTER_OFF):
            if (sentence == TOKEN_CENTER_ON):
                centered = True
                # Only advance when centering starts in the middle of an occupied line.
                if offset != 0:
                    outStr += TOKEN_NEWLINE
            else:
                centered = False
                # Only advance when centered text actually occupied the current line.
                if offset != 0:
                    outStr += TOKEN_NEWLINE
            currLine = ""
            offset = 0
            lineLength = 0
            currWordIndex += 1

        # See if the sentence is a new box
        elif(sentence == TOKEN_BOX_BREAK):
            outStr += sentence
            currLine = ""
            offset = 0
            lineLength = 0
            currWordIndex += 1

        else:
            wordLength = 0

            # Use the same command-aware measurement for wrapping and diagnostics.
            wordLength = get_text_pixel_length(
                word, pixelsPerChar, language_char_array, lang, entry_id, context
            )
            spaceLength = (language_char_array["font"].charWidthTable[0]
                           if pixelsPerChar == -1 else pixelsPerChar)

            # Test if the word is too long in general
            if (wordLength > pixelsInLine):
                log_warning_error(lang, "Error", f"Word {word} exceeds alloted length ({pixelsInLine} pixels)", entry_id, context)
                currWordIndex += 1

            # Test if adding the word will go over our alloted space
            elif ((wordLength + lineLength + offset) <= pixelsInLine):
                # If not, add the word and increase the index
                if (currWordIndex == (len(words) - 1)):
                    # Don't add a space to the end of the sentence.
                    currLine += word
                    lineLength += wordLength
                else:
                    currLine += (word + " ")
                    lineLength += (wordLength + spaceLength)
                currWordIndex += 1

            # We need to move to the next line
            else:
                # Every wrapped line in a centered block needs its own horizontal offset.
                formatted_line, _ = format_output_line(currLine, lineLength, trim_trailing_space=True)
                outStr += (formatted_line + TOKEN_NEWLINE)
                currLine = ""
                lineCount += 1
                lineLength = 0
                offset = 0
    if (centered and (len(words) > 0) and words[0] not in [
        TOKEN_CENTER_ON,
        TOKEN_BOX_BREAK,
        TOKEN_NEWLINE,
        TOKEN_SCROLL_BREAK,
    ]):
        currLine, lineLength = format_output_line(currLine, lineLength)
    outStr += currLine
    return lineLength + offset, lineCount, outStr, centered

def get_text_pixel_length(text, pixelsPerChar, language_char_array, lang, entry_id=None, context=None):
    if not text:
        return 0

    total = 0
    index = 0
    spacing_char = language_char_array["array"][0xFC]
    widths = language_char_array["font"].charWidthTable
    while index < len(text):
        char = text[index]
        if char == spacing_char and text[index + 1:index + 2] == "[":
            end = text.find("]", index + 2)
            count = text[index + 2:end] if end != -1 else ""
            if count.isascii() and count.isdecimal():
                total += widths[0xFC] * int(count)
                index = end + 1
                continue
        if char == PURPOSEFUL_SPACE_CHAR:
            char = " "
        if pixelsPerChar == -1:
            total += widths[convert_char_to_byte(ord(char), language_char_array["array"], lang, entry_id, context)]
        else:
            total += pixelsPerChar
        index += 1
    return total

def convert_char_to_byte(incoming, array, lang, entry_id=None, context=None):
    for pair in charConversionList:
        if incoming == ord(pair[0]):
            incoming = ord(pair[1])
            log_warning_error(lang, "Warning", f"Character {pair[0]} was used but is not in character table. Replaced with {pair[1]} .", entry_id, context)
    
    index = 0
    #print(array)
    for val in array:
        if str(val) == chr(incoming):
            return index
        index += 1    
    if chr(incoming) != '_':
        log_warning_error(lang, "Error", f"No match found for char [ {chr(incoming)} ]!", entry_id, context)
    return 0
    
def log_warning_error(lang, type, text, entry_id=None, context=None):
    (context or build_context).log(lang, type, text, entry_id)

def hash_excel(path):
    sheets = pd.read_excel(path, sheet_name=None)
    h = hashlib.sha256()
    for name in sorted(sheets):
        h.update(name.encode())
        h.update(pd.util.hash_pandas_object(
            sheets[name], index=True
        ).values)
    return h.digest()

def hash_file_bytes(path):
    h = hashlib.sha256()
    with open(path, "rb") as f:
        for chunk in iter(lambda: f.read(1024 * 1024), b""):
            h.update(chunk)
    return h.digest()

def apply_escape_sequences(line, arr, escape_list):
    # Convert structural text controls to the formatter's internal sentinels
    # before generic escape replacement so layout logic can see them reliably.
    line = line.replace("{NEW}", TOKEN_NEWLINE)

    for token, char_indexes in escape_list:
        if token in line:
            escape_string = "".join(arr[idx] for idx in char_indexes)
            line = line.replace(token, escape_string)

    # Special case for centering escape characters
    line = line.replace("{CTR}", TOKEN_CENTER_ON)
    line = line.replace("{nCTR}", TOKEN_CENTER_OFF)
    # Some control markers also arrive via the language escape table as visible
    # glyphs. Normalize those too so layout treats them consistently.
    return normalize_control_glyphs_to_tokens(line)

def apply_language_tokens(line, arr, lang):
    indexes = get_language_config(lang).token_indexes
    if indexes is None:
        return line

    lvl_index, pp_index, no_index = indexes
    return (
        line
        .replace("{LVL}", arr[lvl_index])
        .replace("{PP}", arr[pp_index])
        .replace("{NO}", arr[no_index])
    )

@dataclass
class FormatState:
    out_text: str = ""
    current_line_count: int = 0
    current_offset: int = 0
    escape_count: int = 0
    centered: bool = False

def preserve_punctuation_spacing(line):
    spaces = [' ', '　']
    puncts = ['.', '?', '!', '。', '！', '？']
    for space in spaces:
        for punct in puncts:
            line = line.replace(punct + space, punct + PURPOSEFUL_SPACE_CHAR)
    return line

def force_newline_before_center_transition(state, sentence):
    if sentence not in (TOKEN_CENTER_ON, TOKEN_CENTER_OFF):
        return
    if not state.out_text or state.out_text[-1] in (TOKEN_NEWLINE, TOKEN_SCROLL_BREAK, TOKEN_BOX_BREAK):
        return
    state.out_text += TOKEN_NEWLINE
    state.current_line_count += 1
    state.current_offset = 0

def append_formatted_sentence(state, out, include_scrolling, numLines):
    if out == TOKEN_BOX_BREAK:
        state.current_offset = 0
        state.current_line_count = 0
        if state.out_text and (state.out_text[-1] in (" ", TOKEN_NEWLINE, TOKEN_SCROLL_BREAK)):
            state.out_text = state.out_text[:-1]
        state.out_text += TOKEN_BOX_BREAK
        return True

    if state.current_line_count < (numLines + int(include_scrolling)):
        if state.out_text and out and state.out_text[-1] == TOKEN_BOX_BREAK and out[0] in (" ", TOKEN_NEWLINE, TOKEN_SCROLL_BREAK):
            out = out[1:]
        state.out_text += out
        return True

    return False

def handle_box_overflow(state, out, numLines, include_scrolling, include_box_breaks, prev_offset, prev_curr_line,
                        pixelsInLine, pixelsPerChar, language_char_array, lang, entry_id, context=None):
    if not include_box_breaks:
        remaining_lines = max(0, (numLines + int(include_scrolling)) - prev_curr_line)
        if remaining_lines > 0:
            remaining_pixels = max(0, pixelsInLine - prev_offset) + ((remaining_lines - 1) * pixelsInLine)
        else:
            remaining_pixels = 0
        required_pixels = get_text_pixel_length(out.replace(TOKEN_NEWLINE, '').replace(TOKEN_SCROLL_BREAK, ''), pixelsPerChar, language_char_array, lang, entry_id, context)
        overflow_pixels = max(0, required_pixels - remaining_pixels)
        if overflow_pixels > 0:
            log_warning_error(lang, "Error", f"Attempted to make a new text box when disabled, sentence \"{format_internal_tokens(state.out_text)}\" is too long by at least {overflow_pixels} pixels!", entry_id, context)
        else:
            extra_lines = max(1, state.current_line_count - (numLines + int(include_scrolling)) + 1)
            log_warning_error(lang, "Error", f"Attempted to make a new text box when disabled, sentence \"{format_internal_tokens(state.out_text)}\" requires at least {extra_lines} additional line(s)!", entry_id, context)
    elif state.out_text and (state.out_text[-1] in (" ", TOKEN_NEWLINE, TOKEN_SCROLL_BREAK)):
        state.out_text = state.out_text[:-1]

    state.out_text += TOKEN_BOX_BREAK
    state.current_offset = 0
    state.current_line_count = 0
    state.escape_count += 1

def apply_vertical_centering_to_page(page_text, capacity, line_count_override=None):
    trailing_newlines = len(page_text) - len(page_text.rstrip(TOKEN_NEWLINE))
    content_page = page_text.rstrip(TOKEN_NEWLINE)
    if not content_page:
        return page_text

    line_count = line_count_override if line_count_override is not None else (content_page.count(TOKEN_NEWLINE) + 1)
    if line_count >= capacity:
        return page_text

    top_padding = (capacity - line_count) // 2
    return (TOKEN_NEWLINE * top_padding) + content_page + (TOKEN_NEWLINE * trailing_newlines)

def count_centering_lines(page_text):
    content_page = page_text.rstrip(TOKEN_NEWLINE)
    if not content_page:
        return 0
    return content_page.count(TOKEN_NEWLINE) + 1

def apply_vertical_centering(out_text, numLines, reference_text=None):
    centered_boxes = []
    reference_boxes = reference_text.split(TOKEN_BOX_BREAK) if reference_text is not None else None
    for box_index, box in enumerate(out_text.split(TOKEN_BOX_BREAK)):
        reference_box = None if reference_boxes is None or box_index >= len(reference_boxes) else reference_boxes[box_index]
        reference_pages = reference_box.split(TOKEN_SCROLL_BREAK) if reference_box is not None else None
        centered_pages = []
        for page_index, page_text in enumerate(box.split(TOKEN_SCROLL_BREAK)):
            reference_page = None if reference_pages is None or page_index >= len(reference_pages) else reference_pages[page_index]
            line_count_override = None
            if reference_page is not None:
                reference_line_count = count_centering_lines(reference_page)
                if reference_line_count > 0:
                    line_count_override = reference_line_count
            centered_pages.append(apply_vertical_centering_to_page(page_text, numLines, line_count_override))
        centered_boxes.append(TOKEN_SCROLL_BREAK.join(centered_pages))
    return TOKEN_BOX_BREAK.join(centered_boxes)

def protect_explicit_blank_lines(text):
    while TOKEN_NEWLINE + TOKEN_NEWLINE in text:
        text = text.replace(
            TOKEN_NEWLINE + TOKEN_NEWLINE,
            TOKEN_NEWLINE + TOKEN_PRESERVED_BLANK_LINE + TOKEN_NEWLINE,
        )
    return text

def restore_explicit_blank_lines(text):
    return text.replace(TOKEN_PRESERVED_BLANK_LINE, "")

def normalize_formatted_text(out_text, numLines, include_scrolling, lang, entry_id, context=None, vertically_center_text=False):
    original_out_text = out_text
    out_text = protect_explicit_blank_lines(out_text)
    out_text = out_text.replace(f"{TOKEN_NEWLINE}{PURPOSEFUL_SPACE_CHAR}", TOKEN_NEWLINE)
    out_text = out_text.replace(f"{TOKEN_SCROLL_BREAK}{PURPOSEFUL_SPACE_CHAR}", TOKEN_SCROLL_BREAK)
    out_text = out_text.replace(PURPOSEFUL_SPACE_CHAR, " ")

    exitLoop = False
    while(not exitLoop):
        newStr = ""

        splitBoxes = out_text.split(TOKEN_BOX_BREAK)
        outIndex = 0
        for box in splitBoxes:
            if box and ((box[0] == " ")):
                box = box[1:]
                outIndex += 1
            box = box.replace(TOKEN_SCROLL_BREAK, TOKEN_NEWLINE)
            leading_newlines = len(box) - len(box.lstrip(TOKEN_NEWLINE))
            splitLines = box.split(TOKEN_NEWLINE)
            outBox = ""
            i = 1
            for split in splitLines:
                outIndex += len(split)
                if split == splitLines[-1]:
                    breakChar = ""
                elif ((i >= numLines) and include_scrolling):
                    breakChar = TOKEN_SCROLL_BREAK
                else:
                    breakChar = out_text[outIndex]
                outBox += split + breakChar
                outIndex += 1
                i += 1
            if leading_newlines:
                existing_leading_newlines = len(outBox) - len(outBox.lstrip(TOKEN_NEWLINE))
                if existing_leading_newlines < leading_newlines:
                    outBox = (TOKEN_NEWLINE * (leading_newlines - existing_leading_newlines)) + outBox
            if (outBox and (outBox[:-1] == TOKEN_BOX_BREAK) or (outBox[:-1] == TOKEN_NEWLINE)):
                newStr += f'{outBox[:-1]}{TOKEN_BOX_BREAK}'
            elif (outBox):
                newStr += f'{outBox}{TOKEN_BOX_BREAK}'
        newStr = newStr[:-1]

        if len(newStr) > 1023:
            newStr = newStr[:1023]
            log_warning_error(lang, "Warning", f"String {newStr} exceeds character limit of 1023 and has been truncated.", entry_id, context)

        exitLoop = (newStr == out_text)
        out_text = newStr
    if vertically_center_text:
        out_text = apply_vertical_centering(out_text, numLines, original_out_text)
    return restore_explicit_blank_lines(out_text)

def encode_formatted_text(out_text, arr, lang, entry_id, context=None):
    byteStr = ""
    i = 0
    while i < len(out_text[:-1]):
        char = FORMAT_TOKEN_TO_BYTE_CHAR.get(out_text[i], out_text[i])
        if (char == '['):
            val = ''
            i += 1
            while out_text[i] != ']':
                val = val + out_text[i]
                i += 1
            num = int(val)
            byteStr += f"{num:02x} "
        else:
            byteStr += f"{convert_char_to_byte(ord(char), arr, lang, entry_id, context):02x} "
        i += 1
    if (len(out_text) > 0 and out_text[-1] != ' '):
        byteStr += f"{convert_char_to_byte(ord(FORMAT_TOKEN_TO_BYTE_CHAR.get(out_text[-1], out_text[-1])), arr, lang, entry_id, context):02x} "

    return byteStr + "ff"

def render_debug_text(byte_string, arr):
    byte_values = byte_string.split(" ")
    outText = ""
    index = 0
    while index < len(byte_values):
        byte_value = int(byte_values[index], 16)
        if byte_value == 0xFC and index + 1 < len(byte_values):
            outText += f"_[{int(byte_values[index + 1], 16)}]"
            index += 2
            continue
        outText += str(arr[byte_value])
        index += 1
    return outText

def format_text_entry(ogDict, lang, context=None):
    line = ogDict["bytes"]
    entry_id = ogDict.get("entryId")
    numLines = ogDict["numLines"]
    pixelsPerChar = ogDict["pixelsPerChar"]
    pixelsInLine = ogDict["pixelsInLine"]
    include_box_breaks = ogDict["includeBoxBreaks"]
    include_scrolling = ogDict["includeScrolling"]
    vertically_center_text = coerce_to_bool(ogDict["verticallyCenterText"])

    language_char_array = get_language_config(lang).char_array
    arr = language_char_array["array"]
    escape_list = language_char_array["escape"]

    line = apply_escape_sequences(line, arr, escape_list)
    line = apply_language_tokens(line, arr, lang)
    line = preserve_punctuation_spacing(line)

    split_sents = split_into_sentences(line)
    state = FormatState()
    index = 0
    while index < len(split_sents) and state.escape_count < 100:
        force_newline_before_center_transition(state, split_sents[index])
        prev_offset = state.current_offset
        prev_curr_line = state.current_line_count
        state.current_offset, recievedLine, out, state.centered = split_sentence_into_lines(
            split_sents[index], state.current_offset, pixelsPerChar, pixelsInLine,
            state.centered, lang, state.current_line_count, numLines, entry_id, context
        )
        state.current_line_count += recievedLine

        if append_formatted_sentence(state, out, include_scrolling, numLines):
            index += 1
        else:
            handle_box_overflow(
                state, out, numLines, include_scrolling, include_box_breaks, prev_offset,
                prev_curr_line, pixelsInLine, pixelsPerChar, language_char_array, lang, entry_id, context
            )

    if state.escape_count == 100:
        total_capacity = (numLines + int(include_scrolling)) * pixelsInLine
        required_pixels = get_text_pixel_length(
            out.replace(TOKEN_NEWLINE, '').replace(TOKEN_SCROLL_BREAK, ''),
            pixelsPerChar,
            language_char_array,
            lang,
            entry_id,
            context,
        )
        overflow_pixels = max(0, required_pixels - total_capacity)
        if overflow_pixels > 0:
            log_warning_error(lang, "Error", f"Sentence \"{out}\" is too long by at least {overflow_pixels} pixels!", entry_id, context)
        else:
            log_warning_error(lang, "Error", f"Sentence \"{out}\" requires additional line(s) beyond the available box height!", entry_id, context)

    return normalize_formatted_text(
        state.out_text,
        numLines,
        include_scrolling,
        lang,
        entry_id,
        context,
        vertically_center_text,
    )

def convert_item(ogDict, lang, context=None):
    # convert_item replaces ogDict["bytes"] (the source text) with the encoded hex string,
    # so re-running it on an already converted entry would encode that hex string as text.
    # This is a destructive operation, so we need to make sure it's not run more than once.
    # This is relevant because generate_text_tables() runs through all tables (including RSEFRLG)
    # and generate_rsefrlg_tables_for_all_langs() does it again for the RSEFRLG section multiple times.
    # Note however that ogDict being passed here is a specific entry for a specific language.
    # however, because both generate_text_tables() and generate_rsefrlg_tables_for_all_langs() 
    # are getting called, BUILD_LANG is being converted twice for the RSEFRLG section!
    if ogDict.get("converted"):
        return ogDict
    normalized_text = format_text_entry(ogDict, lang, context)
    arr = get_language_config(lang).char_array["array"]
    entry_id = ogDict.get("entryId")
    ogDict["bytes"] = encode_formatted_text(normalized_text, arr, lang, entry_id, context)
    ogDict["converted"] = True
    return ogDict

# in order to safely use getPointerToFileInDecompressionBuffer() in FileContainerReader
# we must ensure that a table that is used this way fits inside a single chunk
def get_file_container_chunk_size(section):
    if section in ("RSEFRLG"):
        return 4096
    else:
        return 2048

def write_text_file_container(filename, dictionary, lang, section, context=None):
    buildpath = str(BUILD_DIR) + "/"
    Path(buildpath).mkdir(parents=True, exist_ok=True)
    defPath = buildpath + filename + ".containerdef"
    with open(defPath, 'wb') as defFile:
        defLine = "@chunkSize=" + str(get_file_container_chunk_size(section)) + "\n"
        defFile.write(defLine.encode("utf-8"))
        
        for key, line in dictionary.items():
            entryFilename = str(key) + "_" + lang.name
            with open(buildpath + entryFilename, 'wb') as lineFile:
                dictionary[key] = convert_item(line, lang, context)
                linedata = bytes.fromhex(dictionary[key]['bytes'])
                lineFile.write(linedata)
                lineFile.close()
            
            defLine = buildpath + entryFilename + ":" + str(key) + "\n"
            defFile.write(defLine.encode("utf-8"))
        defFile.close()


def write_text_bin_file(filename, dictionary, lang, section, context=None):
    MAX_BIN_SIZES = {
        "PTGB": 6144,
        "RSEFRLG": 3444,
        "GB": 9999,
        "GENERAL": 2048,
        "CREDITS": 2048,
        "TUTORIAL": 2048,
        "PKMN_NAMES": 3072,
    }

    Path(filename).parent.mkdir(parents=True, exist_ok=True)
    with open(filename, 'wb') as binFile:
        # Let the first byte indicate the number of entries
        dict_size = len(dictionary)
        # We need to store 2 bytes instead of one, because not aligning the data to 16 bits will cause corruption on the gba.
        binFile.write(bytes([dict_size & 0xFF, (dict_size >> 8) & 0xFF]))
        # After this initial byte, we will read the offset (16 bit) of each line (relative to the last index byte)
        index = bytearray(len(dictionary) * 2)
        # bindata will contain the binary data of each entry
        bindata = bytearray()
        current_offset = 0

        num = 0
        # Append every line's binary data to bindata
        # keep an index of the binary offset within bindata at which each line starts
        for key, line in dictionary.items():
            dictionary[key] = convert_item(line, lang, context)
            # store the offset of the line in the index as a 16 bit little endian value
            index[num * 2] = (current_offset & 0xFF)
            index[num * 2 + 1] = (current_offset >> 8) & 0xFF
            linedata = bytes.fromhex(dictionary[key]['bytes'])

            bindata.extend(linedata)
            current_offset += len(linedata)

            num += 1

        # Write the index and bindata to the file
        binFile.write(index)
        binFile.write(bindata)
        binFile.seek(0, os.SEEK_END)
        if binFile.tell() > MAX_BIN_SIZES[section]:
            log_warning_error(lang, "Error", f'Section {section} exceeds the max binary file size by {binFile.tell() - MAX_BIN_SIZES[section]} bytes!', context=context)
        binFile.close()

def write_enum_to_header_file(hFile, prefix, dictionary):
    num = 0
    for key, line in dictionary.items():
        hFile.write(f"#define {prefix}{key} {num}\n")
        num += 1
    hFile.write(f"\n#define {prefix}LENGTH {num}\n")
    hFile.write("\n")
    return num

def update_xlsx_file(build_xlsx_mode):

    if build_xlsx_mode == "local":
        print("\tUsing local XLSX file.")

        if not TEXT_XLSX_PATH.exists():
            print("ERROR: Local XLSX file not found.")
            sys.exit(1)
        return False

    elif build_xlsx_mode == "remote":
        print("\tDownloading XLSX.")

        try:
            response = requests.get(XLSX_URL, timeout=5)
            response.raise_for_status()

            with open(NEW_TEXT_XLSX_PATH, 'wb') as f:
                f.write(response.content)

            print("\tDownload successful.")

        except requests.exceptions.RequestException as e:
            print("ERROR: Failed to download XLSX.")
            print(f"Reason: {e}")
            sys.exit(1)

        # If cached file exists, compare hashes
        if TEXT_XLSX_PATH.exists():
            if hash_file_bytes(NEW_TEXT_XLSX_PATH) == hash_file_bytes(TEXT_XLSX_PATH):
                print("\tDownloaded file is identical. Skipping parse.")
                NEW_TEXT_XLSX_PATH.unlink()
                return False
            else:
                TEXT_XLSX_PATH.unlink()
                NEW_TEXT_XLSX_PATH.rename(TEXT_XLSX_PATH)
                return True
        else:
            print("\tNo cached XLSX - forcing rebuild.")
            NEW_TEXT_XLSX_PATH.rename(TEXT_XLSX_PATH)
            return True

    else:
        print(f"ERROR: Invalid BUILD_XLSX value '{build_xlsx_mode}'")
        sys.exit(1)

def are_text_build_artifacts_newer():
    if debugpy.is_client_connected():
        print("\tDebugger connected, forcing text rebuild.")
        return False

    artifacts = [TRANSLATED_H_PATH, TRANSLATED_CPP_PATH, OUTPUT_JSON_PATH]
    if any(not path.exists() for path in artifacts):
        print("\tText artifacts missing - forcing rebuild.")
        return False

    artifacts_are_stale = are_generated_files_stale(get_text_source_files(), artifacts)
    if artifacts_are_stale:
        print("\tText artifacts are older than sources - forcing rebuild.")
        return False

    print("\tText artifacts are newer than sources. Skipping text rebuild.")
    return True

def initialize_translation_storage():
    build_context.initialize_storage(textSections)

def transfer_xlsx_to_dict():
    global boxTypeDefinitions
    global boxTypeNames
    global boxTypeIdByName
    global boxTypeValueKeys
    global boxTypeValueMeta

    print("\tGetting character arrays")
    currSheet = pd.read_excel(TEXT_XLSX_PATH, sheet_name="Character Arrays", header=None)
    offset = 0
    for key, value in charArrays.items():
        for r in range(16):
            for c in range(16):
                val = currSheet.iloc[r + 1, c + offset]
                if pd.isna(val):
                    val = " "
                value["array"][r * 0x10 + c] = val
        # print(charArrays[key])
        offset += 16



    print("\tGetting string data")
    currSheet = pd.read_excel(TEXT_XLSX_PATH, sheet_name="Translations")
    sheet_columns = list(currSheet.columns)

    print("\tGetting box types")
    boxTypeSheet = pd.read_excel(TEXT_XLSX_PATH, sheet_name="Box Types")
    box_type_columns = list(boxTypeSheet.columns)
    box_type_name_col = None
    for col in box_type_columns:
        if normalize_box_type_header(col) == "boxtype":
            box_type_name_col = col
            break
    if box_type_name_col is None:
        raise KeyError("Could not find 'Box Type' column in Box Types sheet.")

    boxTypeValueKeys = []
    boxTypeValueMeta = []
    box_type_columns_by_normalized = {}
    for col in box_type_columns:
        if col == box_type_name_col:
            continue
        normalized = normalize_box_type_header(col)
        if normalized in box_type_columns_by_normalized:
            raise KeyError(f"Duplicate normalized Box Types column '{normalized}' found.")
        box_type_columns_by_normalized[normalized] = col
        boxTypeValueKeys.append(col)
        boxTypeValueMeta.append({"key": col, "macro_name": str(col)})

    boxTypeDefinitions = {}
    boxTypeNames = []
    boxTypeIdByName = {}
    for _, box_type_row in boxTypeSheet.iterrows():
        box_type_name = box_type_row[box_type_name_col]
        if pd.isna(box_type_name):
            continue
        box_type_name = str(box_type_name).strip()
        if box_type_name in boxTypeDefinitions:
            raise KeyError(f"Duplicate Box Type '{box_type_name}' found in Box Types sheet.")
        boxTypeDefinitions[box_type_name] = {}
        for col in boxTypeValueKeys:
            value = box_type_row[col]
            if normalize_box_type_header(col) == "pixelsperchar" and pd.isna(value):
                value = "Default"
            boxTypeDefinitions[box_type_name][col] = value

        required_keys = (
            "numLines",
            "pixelsPerChar",
            "pixelsInLine",
            "includeBoxBreaks",
            "includeScrolling",
            "boxStyle",
        )
        for internal_key in required_keys:
            col = find_required_box_type_column(box_type_columns_by_normalized, internal_key)
            if col is None:
                raise KeyError(f"Missing required Box Types column matching '{internal_key}'.")
            boxTypeDefinitions[box_type_name][internal_key] = boxTypeDefinitions[box_type_name][col]
        vertical_center_col = find_required_box_type_column(box_type_columns_by_normalized, "verticallyCenterText")
        if vertical_center_col is None:
            raise KeyError("Missing required Box Types column matching 'verticallyCenterText'.")
        boxTypeDefinitions[box_type_name]["verticallyCenterText"] = int(
            coerce_to_bool(boxTypeDefinitions[box_type_name][vertical_center_col])
        )
        boxTypeIdByName[box_type_name] = len(boxTypeNames)
        boxTypeNames.append(box_type_name)

    text_section_col = find_column_by_aliases(sheet_columns, ("Text Section",))
    text_key_col = find_column_by_aliases(sheet_columns, ("Text Key", "Text ID", "Key"))
    box_type_col = find_column_by_aliases(sheet_columns, ("Box Type",))
    entry_id_col = sheet_columns[0]

    language_columns = {
        lang: find_column_by_aliases(sheet_columns, get_language_config(lang).column_aliases)
        for lang in Languages
    }
    english_col = language_columns[Languages.English]

    textSections.clear()
    for row in currSheet.iterrows():
        currRow = row[1][text_section_col]
        if (currRow not in textSections):
            textSections.append(currRow)

    initialize_translation_storage()

    for row in currSheet.iterrows():
        #print(row)
        for lang in Languages:
            currRow = row[1]
            lang_col = language_columns[lang]
            text_value = currRow[lang_col]
            if pd.isna(text_value):
                text_value = currRow[english_col]
            box_type_name = currRow[box_type_col]
            if pd.isna(box_type_name):
                raise KeyError(f"Missing Box Type for row key '{currRow[text_key_col]}' in section '{currRow[text_section_col]}'.")
            box_type_name = str(box_type_name).strip()
            box_type_data = boxTypeDefinitions.get(box_type_name)
            if box_type_data is None:
                raise KeyError(
                    f"Unknown Box Type '{box_type_name}' for row key '{currRow[text_key_col]}' "
                    f"in section '{currRow[text_section_col]}'."
                )
            entry = {"bytes": text_value, "boxType": box_type_name, "entryId": currRow[entry_id_col]}
            entry.update(box_type_data)
            mainDict[lang.name][currRow[text_section_col]][currRow[text_key_col]] = entry

def generate_header_file():
    print("\tGenerating header file")
    with open(TRANSLATED_H_PATH, 'w') as hFile:
        hFile.write("// THIS FILE HAS BEEN GENERATED BY text_helper/main.py !\n\n#ifndef TRANSLATED_TEXT_H\n#define TRANSLATED_TEXT_H\n\n#include <tonc.h>\n\n")

        sectionEnds = []
        index = 0
        # The lang doesn't matter. They're all the same in this context.
        lang = Languages.English
        for section in textSections:
            num = write_enum_to_header_file(hFile, section + "_", mainDict[lang.name][section])
            hFile.write("#define " + section + "_INDEX " + str(index))
            if(section == "PTGB"):
                hFile.write(f"\n#define DIA_END {num}\n")

            hFile.write("/** Returns the LZ10 compressed " + section + " text table.*/\n")
            sectionEnds.append(num)
            index += 1

        hFile.write("#define NUM_TEXT_SECTIONS " + str(index) + "\n")
        hFile.write("const int text_section_lengths[] = {\n")
        for end in sectionEnds:
            hFile.write("\t" + str(end) + ",\n")
        hFile.write("};\n\n")

        hFile.write("#define BOX_TYPE_INVALID 0xFF\n")
        for box_type_name in boxTypeNames:
            box_type_id = boxTypeIdByName[box_type_name]
            hFile.write(f"#define BOX_TYPE_{sanitize_macro_token(box_type_name)} {box_type_id}\n")
        hFile.write(f"#define NUM_BOX_TYPES {len(boxTypeNames)}\n\n")
        for index, meta in enumerate(boxTypeValueMeta):
            hFile.write(f"#define BOX_TYPE_VAL_{sanitize_macro_token(meta['macro_name'])} {index}\n")
        hFile.write(f"#define NUM_BOX_TYPE_VALS {len(boxTypeValueMeta)}\n\n")
        hFile.write("const int box_type_info[NUM_BOX_TYPES][NUM_BOX_TYPE_VALS] = {\n")
        for box_type_name in boxTypeNames:
            boxType = boxTypeDefinitions[box_type_name]
            values = []
            for meta in boxTypeValueMeta:
                key = meta["key"]
                value = boxType[key]
                if key in ("includeBoxBreaks", "includeScrolling", "boxStyle"):
                    value = int(value)
                values.append(str(value))
            hFile.write(f"\t{{{', '.join(values)}}},\n")
        hFile.write("};\n\n")

        hFile.write("u8 get_text_box_type(int table_index, int text_index);\n")
        hFile.write("extern const u8* const text_box_type_tables[NUM_TEXT_SECTIONS];\n")


        hFile.write("\n#endif")
        hFile.close()

def generate_text_tables(lang):
    print("\tGenerating text tables")
    for section in textSections:
        table_name = section
        write_text_file_container(table_name, mainDict[lang.name][section], lang, section, build_context)
    generate_rsefrlg_tables_for_all_langs()

# To make the output filenames consistent with the BUILD_LANG values in our options.json file
# we need to turn the language name into camelcase
def first_lower(s):
   if len(s) == 0:
      return s
   else:
      return s[0].lower() + s[1:]

# the RSEFRLG table is special. We want to be able to generate the mystery gift payload
# in the inserted cartridge language regardless of the PTGB build lang.
# To accomplish that, we must generate all variants, regardless of BUILD_LANG
def generate_rsefrlg_tables_for_all_langs():
    for lang in Languages:
        section = "RSEFRLG"
        table_name = section + "_" + first_lower(lang.name)
        write_text_file_container(table_name, mainDict[lang.name][section], lang, section, build_context)

def generate_cpp_file():
    print("\tGenerating cpp file")
    with open(TRANSLATED_CPP_PATH, 'w') as cppFile:
        cppFile.write("// THIS FILE HAS BEEN GENERATED BY text_helper/main.py !\n#include \"translated_text.h\"\n#include \"dbg/debug_mode.h\"\n")

        cppFile.write("\n")
        for section in textSections:
            section_var = sanitize_c_identifier(section)
            box_type_macros = []
            for _, entry in mainDict[Languages.English.name][section].items():
                box_type_name = entry["boxType"]
                box_type_macros.append(f"\n\tBOX_TYPE_{sanitize_macro_token(box_type_name)}")
            cppFile.write(f"\nstatic const u8 {section_var}_box_types[] = {{")
            cppFile.write(",".join(box_type_macros))
            cppFile.write("\n};\n")

        cppFile.write("\nextern const u8* const text_box_type_tables[NUM_TEXT_SECTIONS] = {")
        for section in textSections:
            section_var = sanitize_c_identifier(section)
            cppFile.write(f"\n\t{section_var}_box_types,")
        cppFile.write("\n};\n")

        cppFile.write("\n\nu8 get_text_box_type(int table_index, int text_index)\n")
        cppFile.write("{\n")
        cppFile.write("\tif (text_index < 0)\n")
        cppFile.write("\t\treturn BOX_TYPE_INVALID;\n")
        cppFile.write("\tswitch (table_index)\n")
        cppFile.write("\t{\n")
        for section in textSections:
            section_var = sanitize_c_identifier(section)
            cppFile.write(f"\tcase({section}_INDEX):\n")
            cppFile.write(f"\t\tif (text_index >= {section}_LENGTH)\n")
            cppFile.write("\t\t\treturn BOX_TYPE_INVALID;\n")
            cppFile.write(f"\t\treturn {section_var}_box_types[text_index];\n")
        cppFile.write("\tdefault:\n")
        cppFile.write("\t\treturn BOX_TYPE_INVALID;\n")
        cppFile.write("\t}\n")
        cppFile.write("}\n")

def output_json_file(lang):
    print("\tOutputting json file")
    for section in textSections:
        for item in mainDict[lang.name][section]:
            arr = get_language_config(lang).char_array["array"]
            mainDict[lang.name][section][item]["text"] = render_debug_text(
                mainDict[lang.name][section][item]["bytes"], arr
            )

    with open(OUTPUT_JSON_PATH, 'w', encoding='utf-8') as jsonFile:
        jsonFile.write(json.dumps(mainDict[lang.name], ensure_ascii=False, indent=2))

def are_generated_files_stale(source_files, generated_files):
    source_paths = [Path(path) for path in source_files]
    generated_paths = [Path(path) for path in generated_files]

    missing_sources = [path for path in source_paths if not path.exists()]
    if missing_sources:
        raise FileNotFoundError(f"Missing source files: {', '.join(str(path) for path in missing_sources)}")

    if any(not path.exists() for path in generated_paths):
        return True

    newest_source_mtime = max(path.stat().st_mtime for path in source_paths)
    oldest_generated_mtime = min(path.stat().st_mtime for path in generated_paths)
    return newest_source_mtime > oldest_generated_mtime

def update_generated_files(target_name, source_files, generated_files, generate_function, lang, force_generate):
    if debugpy.is_client_connected():
        print(f"\tDebugger connected, rebuilding {target_name}!")
        generate_function(lang)
        return

    if force_generate or are_generated_files_stale(source_files, generated_files):
        print(f"\t{target_name} outputs are outdated or missing. Rebuilding...")
        generate_function(lang)
        return

    print(f"\t{target_name} outputs are up to date. Skipping rebuild.")

def build_h():
    print("\tBuilding font.h")
    with open(FONTS_H_PATH, 'w') as f:
        f.write(f'''#ifndef PTGB_BUILD_LANGUAGE
#error "PTGB_BUILD_LANGUAGE not defined"
#endif

#include "dbg/debug_mode.h"
#include "pokemon_data.h"

#ifndef __FONTS_H__
#define __FONTS_H__''')


        for myFont in fonts.values():
            f.write(f'''\n
#define {myFont.fileName}GlyphsLen {myFont.numBytes}
#define {myFont.fileName}WidthsLen {myFont.numChars}''')
            
            f.write(f'\n\nconst unsigned int {myFont.fileName}Glyphs[{myFont.numWords}] __attribute__((aligned(4)))=\n{{\n\t')

            for i in range(myFont.numWords):
                f.write(f'{myFont.charWordTable[i]:#010x},')
                if (i == myFont.numWords - 1):
                    f.write("\n};\n\n")
                elif (i % 64 == 63):
                    f.write("\n\n\t")
                elif (i % 8 == 7):
                    f.write("\n\t")

            f.write(f'const unsigned char {myFont.fileName}Widths[{myFont.numChars}] __attribute__((aligned(4)))=\n{{\n\t')

            for i in range(myFont.numChars):
                f.write(f'{myFont.charWidthTable[i]:#04x}, ')
                if (i == myFont.numChars - 1):
                    f.write("\n};\n\n")
                elif (i % 128 == 127):
                    f.write("\n\n\t")
                elif (i % 16 == 15):
                    f.write("\n\t")

            f.write(f'''const TFont {myFont.fileName}Font= 
{{
    {myFont.fileName}Glyphs, 
    {myFont.fileName}Widths, 
    0, // All heights are the same 
    0, // Character offset, is set to zero
    {myFont.numChars},
    {myFont.charWidth}, {myFont.charHeight},
    {myFont.cellWidth}, {myFont.cellHeight}, 
    {myFont.numBytes // myFont.numChars}, 
    {myFont.bpp}, 
    0, // Padding, left blank
}};''')

        f.write('\n\n#endif')
    f.close()

def generate_tables():
    for myFont in fonts.values():
        print(f'\t\t{myFont.fileName}')
        reader = png.Reader(f'{BASE_DIR}/fonts/{myFont.fileName}.png')
        png_info = reader.read()[3]
        palette = png_info.get('palette')
        if (palette is None):
            print("Error: Image file does not contain a palette")
            exit()

        width, height, rows, info = reader.read()
        pixels = list(rows)

        bitsPerWord = 32
        pixelsPerTileX = 8
        pixelsPerTileY = 8
        tilesPerCharX = myFont.cellWidth // pixelsPerTileX
        tilesPerCharY = myFont.cellHeight // pixelsPerTileY
        charsPerChartX = myFont.numCharsX
        charsPerChartY = myFont.numCharsY

        globalX = 0
        globalY = 0
        bitTotal = 0

        for charY in range(charsPerChartY):
            for charX in range(charsPerChartX):
                for tileX in range(tilesPerCharX): # Tiles go from top to bottom, then left to right
                    for tileY in range(tilesPerCharY):
                        for pixelY in range(pixelsPerTileY):
                            for pixelX in range(pixelsPerTileX):
                                
                                arrayIndex = bitTotal // bitsPerWord
                                bitIndex = bitTotal % bitsPerWord

                                globalX = pixelX + (tileX * pixelsPerTileX) + (charX * tilesPerCharX * pixelsPerTileX)
                                globalY = pixelY + (tileY * pixelsPerTileY) + (charY * tilesPerCharY * pixelsPerTileY)

                                val = (pixels[globalY][globalX] - 1)
                                if val < 0:
                                    val = 0
                                val &= myFont.bpp
                                myFont.charWordTable[arrayIndex] |= val << bitIndex

                                #print(f'globalX: {globalX}, globalY: {globalY}, arrayIndex:{arrayIndex}, bitIndex:{bitIndex}, val:{val}')
                                bitTotal += myFont.bpp
                
                #print(f'{charX, charY}')

                myFont.charWidthTable[(charY * charsPerChartX) + charX] = (tilesPerCharX * pixelsPerTileX)
                for x in range(tilesPerCharX * pixelsPerTileX):
                    globalX = x + (charX * tilesPerCharX * pixelsPerTileX)
                    globalY = 0 + (charY * tilesPerCharY * pixelsPerTileY)
                    #print(f'x: {globalX}, y: {globalY}')
                    if (pixels[globalY][globalX] == BG_PAL_INDEX):
                        myFont.charWidthTable[(charY * charsPerChartX) + charX] = x
                        break

def get_font_source_files():
    return [THIS_SCRIPT_PATH] + [BASE_DIR / "fonts" / f"{font.fileName}.png" for font in fonts.values()]

def get_font_generated_files():
    return [FONTS_H_PATH]

def generate_font_files(lang):
    print("\tGenerating font tables:")
    generate_tables()
    build_h()

def update_font_files():
    update_generated_files(
        target_name="Fonts.h",
        source_files=get_font_source_files(),
        generated_files=get_font_generated_files(),
        generate_function=generate_font_files,
        lang = Languages.English,
        force_generate=False
    )

def get_text_source_files():
    return [THIS_SCRIPT_PATH, TEXT_XLSX_PATH]

def get_text_generated_files():
    generated_files = [TRANSLATED_H_PATH, TRANSLATED_CPP_PATH, OUTPUT_JSON_PATH]
    for lang in Languages:
        for section in textSections:
            generated_files.append(Path(os.curdir) / "to_compress" / f"{section}_{lang.name.lower()}.bin")
    return generated_files

def generate_text_files(lang):
    generate_header_file()
    generate_text_tables(lang)
    generate_cpp_file()
    output_json_file(lang)

def update_text_files(lang):
    # force_generate is set to true because we're only generating the BUILD_LANG.
    # therefore if we generate another lang, we need to update it again too.
    # So we might as well always update it.
    update_generated_files(
        target_name="Text",
        source_files=get_text_source_files(),
        generated_files=get_text_generated_files(),
        generate_function=generate_text_files,
        lang=lang,
        force_generate=True
    )

def main():
    lang_name, _, build_xlsx_mode = parse_build_args(sys.argv)
    print("Running text_helper:")
    update_font_files()
    update_xlsx_file(build_xlsx_mode)
    if are_text_build_artifacts_newer():
        print("text_helper finished!\n")
        return
    transfer_xlsx_to_dict()
    update_text_files(LanguageStringMap[lang_name])
    print("text_helper finished!\n")

if __name__ == "__main__":
    main()
