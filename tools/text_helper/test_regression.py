import importlib.util
import sys
import types
import unittest
from pathlib import Path


sys.modules.setdefault("debugpy", types.SimpleNamespace())
sys.modules.setdefault(
    "pandas",
    types.SimpleNamespace(
        isna=lambda value: value is None,
        util=types.SimpleNamespace(hash_pandas_object=lambda *args, **kwargs: types.SimpleNamespace(values=b""))
    ),
)


MODULE_PATH = Path(__file__).resolve().parent / "main.py"
SPEC = importlib.util.spec_from_file_location("text_helper_main", MODULE_PATH)
text_helper = importlib.util.module_from_spec(SPEC)
TEXT_HELPER_IMPORT_ERROR = None
try:
    SPEC.loader.exec_module(text_helper)
except ModuleNotFoundError as exc:
    TEXT_HELPER_IMPORT_ERROR = exc


def install_test_charset():
    arr = [" "] * 0x100
    for codepoint in range(32, 127):
        arr[codepoint] = chr(codepoint)
    arr[0x5F] = " "
    arr[0x01] = "ｌ"
    arr[0x02] = "ρ"
    arr[0x03] = "№"
    arr[0xFC] = "_"
    arr[0xFB] = "Ş"
    arr[0xFD] = "ȼ"
    arr[0xFE] = "Ň"
    arr[0xFF] = "ƞ"

    widths = [8] * 0x100
    language_config = text_helper.get_language_config(text_helper.Languages.English)
    language_config.token_indexes = (0x01, 0x02, 0x03)
    language_config.char_array["array"] = arr
    language_config.char_array["font"].charWidthTable = widths
    language_config.char_array["escape"] = [
        ["{SCL}", [0xFA]],
        ["{CLR}", [0xFB]],
        ["{DEF}", [0xFC, 0x01, 0x02]],
        ["{FEM}", [0xFC, 0x01, 0x04]],
        ["{FPC}", [0xFC, 0x01, 0x06]],
        ["{MLE}", [0xFC, 0x01, 0x08]],
        ["{SPA}", [0xFC]],
        ["{PLR}", [0xFD, 0x01]],
        ["{NEW}", [0xFE]],
        ["{END}", [0xFF]],
    ]


def decode_text(byte_string, lang):
    arr = text_helper.get_language_config(lang).char_array["array"]
    text = text_helper.render_debug_text(byte_string, arr)
    return text.removesuffix(str(arr[0xFF]))


@unittest.skipIf(TEXT_HELPER_IMPORT_ERROR is not None, f"text_helper dependencies missing: {TEXT_HELPER_IMPORT_ERROR}")
class TextHelperRegressionTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        install_test_charset()
        text_helper.build_context.initialize_storage([])

    def make_entry(self, text, **overrides):
        entry = {
            "bytes": text,
            "entryId": overrides.pop("entryId", "test::entry"),
            "numLines": 2,
            "pixelsPerChar": 8,
            "pixelsInLine": 208,
            "includeBoxBreaks": 1,
            "includeScrolling": 0,
            "verticallyCenterText": 0,
        }
        entry.update(overrides)
        return entry

    def convert_text(self, text, **overrides):
        entry = self.make_entry(text, **overrides)
        return text_helper.convert_item(entry, text_helper.Languages.English, text_helper.build_context)

    def test_centered_text_renders_offsets_readably(self):
        entry = self.convert_text("{CTR}Connecting to Game Boy.")
        text = decode_text(entry["bytes"], text_helper.Languages.English)
        self.assertIn("_[", text)
        self.assertIn("Connecting to Game Boy.", text)

    def test_centered_multi_sentence_text_keeps_second_sentence_at_line_start(self):
        entry = self.convert_text("{CTR}Alpha. Beta.")
        text = decode_text(entry["bytes"], text_helper.Languages.English)
        self.assertNotIn("._[", text)
        self.assertRegex(text, r"Alpha\.Ň_\[\d+\]Beta\.")

    def test_center_tag_mid_text_forces_newline_before_centered_block(self):
        entry = self.convert_text("Alpha {CTR}Beta.")
        text = decode_text(entry["bytes"], text_helper.Languages.English)
        self.assertRegex(text, r"AlphaŇ_\[\d+\]Beta\.")

    def test_terminal_center_close_does_not_append_newline(self):
        entry = self.convert_text("{CTR}PUSH THE START BUTTON!{nCTR}")
        text = decode_text(entry["bytes"], text_helper.Languages.English)
        self.assertRegex(text, r"^_\[\d+\]PUSH THE START BUTTON!$")

    def test_terminal_newline_before_center_close_is_ignored(self):
        entry = self.convert_text("{CTR}PUSH THE START BUTTON!{NEW}{nCTR}", numLines=1, includeBoxBreaks=0)
        text = decode_text(entry["bytes"], text_helper.Languages.English)
        self.assertRegex(text, r"^_\[\d+\]PUSH THE START BUTTON!$")

    def test_newline_removes_leading_space_on_following_line(self):
        entry = self.convert_text("Thanks.{NEW} Again.")
        text = decode_text(entry["bytes"], text_helper.Languages.English)
        self.assertNotIn("Ň ", text)

    def test_level_token_resolves_before_encoding(self):
        entry = self.convert_text("Reach {LVL} 5.")
        text = decode_text(entry["bytes"], text_helper.Languages.English)
        lvl_index, _, _ = text_helper.get_language_config(text_helper.Languages.English).token_indexes
        expected_token = text_helper.get_language_config(text_helper.Languages.English).char_array["array"][lvl_index]
        self.assertEqual(text, f"Reach {expected_token} 5.")
        self.assertNotIn("{LVL}", text)

    def test_vertical_centering_uses_box_type_flag(self):
        entry = self.convert_text(
            "Line one.{NEW}Line two.",
            numLines=5,
            includeScrolling=0,
            includeBoxBreaks=1,
            verticallyCenterText=1,
        )
        text = decode_text(entry["bytes"], text_helper.Languages.English)
        self.assertEqual(text, "ŇLine one.ŇLine two.")

    def test_vertical_centering_in_eight_line_box_uses_two_top_lines_for_four_line_block(self):
        entry = self.convert_text(
            "{CTR}Line one{NEW}{NEW}Line three{NEW}Line four{nCTR}",
            numLines=8,
            includeScrolling=0,
            includeBoxBreaks=0,
            verticallyCenterText=1,
        )
        text = decode_text(entry["bytes"], text_helper.Languages.English)
        self.assertTrue(text.startswith("ŇŇ_["))

    def test_explicit_blank_line_counts_toward_vertical_centering(self):
        entry = self.convert_text(
            "{CTR}Top{NEW}{NEW}Bottom{nCTR}",
            numLines=8,
            includeScrolling=0,
            includeBoxBreaks=0,
            verticallyCenterText=1,
        )
        text = decode_text(entry["bytes"], text_helper.Languages.English)
        self.assertTrue(text.startswith("ŇŇ_["))

    def test_explicit_blank_line_is_preserved_after_centering(self):
        entry = self.convert_text(
            "{CTR}Top{NEW}{NEW}Bottom{nCTR}",
            numLines=8,
            includeScrolling=0,
            includeBoxBreaks=0,
            verticallyCenterText=1,
        )
        text = decode_text(entry["bytes"], text_helper.Languages.English)
        self.assertRegex(text, r"_\[\d+\]TopŇŇ_\[\d+\]Bottom")


if __name__ == "__main__":
    unittest.main()
