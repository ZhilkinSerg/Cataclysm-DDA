#pragma once
#ifndef CATACHARSET_H
#define CATACHARSET_H

#include <cstddef>
#include <cstdint>
#include <string>

#define ANY_LENGTH 5

static constexpr uint32_t NULL_UNICODE = 0x0000;
static constexpr uint32_t START_OF_HEADING_UNICODE = 0x0001;
static constexpr uint32_t START_OF_TEXT_UNICODE = 0x0002;
static constexpr uint32_t END_OF_TEXT_UNICODE = 0x0003;
static constexpr uint32_t END_OF_TRANSMISSION_UNICODE = 0x0004;
static constexpr uint32_t ENQUIRY_UNICODE = 0x0005;
static constexpr uint32_t ACKNOWLDEDGE_UNICODE = 0x0006;
static constexpr uint32_t BELL_UNICODE = 0x0007;
static constexpr uint32_t BACKSPACE_UNICODE = 0x0008;
static constexpr uint32_t HORIZONTAL_TABULATION_UNICODE = 0x0009;
static constexpr uint32_t NEW_LINE_UNICODE = 0x000A;
static constexpr uint32_t VERTICAL_TABULATION_UNICODE = 0x000B;
static constexpr uint32_t FORM_FEED_UNICODE = 0x000C;
static constexpr uint32_t CARRIAGE_RETURN_UNICODE = 0x000D;
static constexpr uint32_t SHIFT_OUT_UNICODE = 0x000E;
static constexpr uint32_t SHIFT_IN_UNICODE = 0x000F;
static constexpr uint32_t DATA_LINK_ESCAPE_UNICODE = 0x0010;
static constexpr uint32_t DEVICE_CONTROL_1_UNICODE = 0x0011;
static constexpr uint32_t DEVICE_CONTROL_2_UNICODE = 0x0012;
static constexpr uint32_t DEVICE_CONTROL_3_UNICODE = 0x0013;
static constexpr uint32_t DEVICE_CONTROL_4_UNICODE = 0x0014;
static constexpr uint32_t NEGATIVE_ACKNOWLDEDGE_UNICODE = 0x0015;
static constexpr uint32_t SYNCHRONOUS_IDLE_UNICODE = 0x0016;
static constexpr uint32_t END_OF_TRANSMISSION_BLOCK_UNICODE = 0x0017;
static constexpr uint32_t CANCEL_UNICODE = 0x0018;
static constexpr uint32_t END_OF_MEDIUM_UNICODE = 0x0019;
static constexpr uint32_t SUBSTITUTE_UNICODE = 0x001A;
static constexpr uint32_t ESCAPE_UNICODE = 0x001B;
static constexpr uint32_t FILE_SEPARATOR_UNICODE = 0x001C;
static constexpr uint32_t GROUP_SEPARATOR_UNICODE = 0x001D;
static constexpr uint32_t RECORD_SEPARATOR_UNICODE = 0x001E;
static constexpr uint32_t UNIT_SEPARATOR_UNICODE = 0x001F;
static constexpr uint32_t SPACE_UNICODE = 0x0020;
static constexpr uint32_t EXCLAMATION_MARK_UNICODE = 0x0021;
static constexpr uint32_t QUOTATION_MARK_UNICODE = 0x0022;
static constexpr uint32_t NUMBER_SIGN_UNICODE = 0x0023;
static constexpr uint32_t DOLLAR_SIGN_UNICODE = 0x0024;
static constexpr uint32_t PERCENT_SIGN_UNICODE = 0x0025;
static constexpr uint32_t AMPERSAND_UNICODE = 0x0026;
static constexpr uint32_t APOSTROPHE_UNICODE = 0x0027;
static constexpr uint32_t LEFT_PARENTHESIS_UNICODE = 0x0028;
static constexpr uint32_t RIGHT_PARENTHESIS_UNICODE = 0x0029;
static constexpr uint32_t ASTERISK_UNICODE = 0x002A;
static constexpr uint32_t PLUS_SIGN_UNICODE = 0x002B;
static constexpr uint32_t COMMA_UNICODE = 0x002C;
static constexpr uint32_t HYPHEN_MINUS_UNICODE = 0x002D;
static constexpr uint32_t FULL_STOP_UNICODE = 0x002E;
static constexpr uint32_t SOLIDUS_UNICODE = 0x002F;
static constexpr uint32_t DIGIT_ZERO_UNICODE = 0x0030;
static constexpr uint32_t DIGIT_ONE_UNICODE = 0x0031;
static constexpr uint32_t DIGIT_TWO_UNICODE = 0x0032;
static constexpr uint32_t DIGIT_THREE_UNICODE = 0x0033;
static constexpr uint32_t DIGIT_FOUR_UNICODE = 0x0034;
static constexpr uint32_t DIGIT_FIVE_UNICODE = 0x0035;
static constexpr uint32_t DIGIT_SIX_UNICODE = 0x0036;
static constexpr uint32_t DIGIT_SEVEN_UNICODE = 0x0037;
static constexpr uint32_t DIGIT_EIGHT_UNICODE = 0x0038;
static constexpr uint32_t DIGIT_NINE_UNICODE = 0x0039;
static constexpr uint32_t COLON_UNICODE = 0x003A;
static constexpr uint32_t SEMICOLON_UNICODE = 0x003B;
static constexpr uint32_t LESS_THAN_SIGN_UNICODE = 0x003C;
static constexpr uint32_t EQUALS_SIGN_UNICODE = 0x003D;
static constexpr uint32_t GREATER_THAN_SIGN_UNICODE = 0x003E;
static constexpr uint32_t QUESTION_MARK_UNICODE = 0x003F;
static constexpr uint32_t COMMERCIAL_AT_UNICODE = 0x0040;
static constexpr uint32_t LATIN_CAPITAL_LETTER_A_UNICODE = 0x0041;
static constexpr uint32_t LATIN_CAPITAL_LETTER_B_UNICODE = 0x0042;
static constexpr uint32_t LATIN_CAPITAL_LETTER_C_UNICODE = 0x0043;
static constexpr uint32_t LATIN_CAPITAL_LETTER_D_UNICODE = 0x0044;
static constexpr uint32_t LATIN_CAPITAL_LETTER_E_UNICODE = 0x0045;
static constexpr uint32_t LATIN_CAPITAL_LETTER_F_UNICODE = 0x0046;
static constexpr uint32_t LATIN_CAPITAL_LETTER_G_UNICODE = 0x0047;
static constexpr uint32_t LATIN_CAPITAL_LETTER_H_UNICODE = 0x0048;
static constexpr uint32_t LATIN_CAPITAL_LETTER_I_UNICODE = 0x0049;
static constexpr uint32_t LATIN_CAPITAL_LETTER_J_UNICODE = 0x004A;
static constexpr uint32_t LATIN_CAPITAL_LETTER_K_UNICODE = 0x004B;
static constexpr uint32_t LATIN_CAPITAL_LETTER_L_UNICODE = 0x004C;
static constexpr uint32_t LATIN_CAPITAL_LETTER_M_UNICODE = 0x004D;
static constexpr uint32_t LATIN_CAPITAL_LETTER_N_UNICODE = 0x004E;
static constexpr uint32_t LATIN_CAPITAL_LETTER_O_UNICODE = 0x004F;
static constexpr uint32_t LATIN_CAPITAL_LETTER_P_UNICODE = 0x0050;
static constexpr uint32_t LATIN_CAPITAL_LETTER_Q_UNICODE = 0x0051;
static constexpr uint32_t LATIN_CAPITAL_LETTER_R_UNICODE = 0x0052;
static constexpr uint32_t LATIN_CAPITAL_LETTER_S_UNICODE = 0x0053;
static constexpr uint32_t LATIN_CAPITAL_LETTER_T_UNICODE = 0x0054;
static constexpr uint32_t LATIN_CAPITAL_LETTER_U_UNICODE = 0x0055;
static constexpr uint32_t LATIN_CAPITAL_LETTER_V_UNICODE = 0x0056;
static constexpr uint32_t LATIN_CAPITAL_LETTER_W_UNICODE = 0x0057;
static constexpr uint32_t LATIN_CAPITAL_LETTER_X_UNICODE = 0x0058;
static constexpr uint32_t LATIN_CAPITAL_LETTER_Y_UNICODE = 0x0059;
static constexpr uint32_t LATIN_CAPITAL_LETTER_Z_UNICODE = 0x005A;
static constexpr uint32_t LEFT_SQUARE_BRACKET_UNICODE = 0x005B;
static constexpr uint32_t REVERSE_SOLIDUS_UNICODE = 0x005C;
static constexpr uint32_t RIGHT_SQUARE_BRACKET_UNICODE = 0x005D;
static constexpr uint32_t CIRCUMFLEX_ACCENT_UNICODE = 0x005E;
static constexpr uint32_t LOW_LINE_UNICODE = 0x005F;
static constexpr uint32_t GRAVE_ACCENT_UNICODE = 0x0060;
static constexpr uint32_t LATIN_SMALL_LETTER_A_UNICODE = 0x0061;
static constexpr uint32_t LATIN_SMALL_LETTER_B_UNICODE = 0x0062;
static constexpr uint32_t LATIN_SMALL_LETTER_C_UNICODE = 0x0063;
static constexpr uint32_t LATIN_SMALL_LETTER_D_UNICODE = 0x0064;
static constexpr uint32_t LATIN_SMALL_LETTER_E_UNICODE = 0x0065;
static constexpr uint32_t LATIN_SMALL_LETTER_F_UNICODE = 0x0066;
static constexpr uint32_t LATIN_SMALL_LETTER_G_UNICODE = 0x0067;
static constexpr uint32_t LATIN_SMALL_LETTER_H_UNICODE = 0x0068;
static constexpr uint32_t LATIN_SMALL_LETTER_I_UNICODE = 0x0069;
static constexpr uint32_t LATIN_SMALL_LETTER_J_UNICODE = 0x006A;
static constexpr uint32_t LATIN_SMALL_LETTER_K_UNICODE = 0x006B;
static constexpr uint32_t LATIN_SMALL_LETTER_L_UNICODE = 0x006C;
static constexpr uint32_t LATIN_SMALL_LETTER_M_UNICODE = 0x006D;
static constexpr uint32_t LATIN_SMALL_LETTER_N_UNICODE = 0x006E;
static constexpr uint32_t LATIN_SMALL_LETTER_O_UNICODE = 0x006F;
static constexpr uint32_t LATIN_SMALL_LETTER_P_UNICODE = 0x0070;
static constexpr uint32_t LATIN_SMALL_LETTER_Q_UNICODE = 0x0071;
static constexpr uint32_t LATIN_SMALL_LETTER_R_UNICODE = 0x0072;
static constexpr uint32_t LATIN_SMALL_LETTER_S_UNICODE = 0x0073;
static constexpr uint32_t LATIN_SMALL_LETTER_T_UNICODE = 0x0074;
static constexpr uint32_t LATIN_SMALL_LETTER_U_UNICODE = 0x0075;
static constexpr uint32_t LATIN_SMALL_LETTER_V_UNICODE = 0x0076;
static constexpr uint32_t LATIN_SMALL_LETTER_W_UNICODE = 0x0077;
static constexpr uint32_t LATIN_SMALL_LETTER_X_UNICODE = 0x0078;
static constexpr uint32_t LATIN_SMALL_LETTER_Y_UNICODE = 0x0079;
static constexpr uint32_t LATIN_SMALL_LETTER_Z_UNICODE = 0x007A;
static constexpr uint32_t LEFT_CURLY_BRACKET_UNICODE = 0x007B;
static constexpr uint32_t VERTICAL_LINE_UNICODE = 0x007C;
static constexpr uint32_t RIGHT_CURLY_BRACKET_UNICODE = 0x007D;
static constexpr uint32_t TILDE_UNICODE = 0x007E;
static constexpr uint32_t DELETE_UNICODE = 0x007F;
static constexpr uint32_t UNKNOWN_UNICODE = 0xFFFD;

// This should never appear as a real symbol!
static constexpr uint32_t PSEUDO_AUTO_WALL_PLACEHOLDER = START_OF_TEXT_UNICODE;

static constexpr uint32_t PSEUDO_WEST_UNICODE = LESS_THAN_SIGN_UNICODE;
static constexpr uint32_t PSEUDO_EAST_UNICODE = GREATER_THAN_SIGN_UNICODE;
static constexpr uint32_t PSEUDO_NORTH_UNICODE = CIRCUMFLEX_ACCENT_UNICODE;
static constexpr uint32_t PSEUDO_SOUTH_UNICODE = LATIN_SMALL_LETTER_V_UNICODE;

static constexpr uint32_t PSEUDO_VEHICLE_BROKEN_PART_UNICODE = NUMBER_SIGN_UNICODE;
static constexpr uint32_t PSEUDO_VEHICLE_OPEN_DOOR_UNICODE = APOSTROPHE_UNICODE;

class utf8_wrapper;

// get a Unicode character from a utf8 string
uint32_t UTF8_getch( const char **src, int *srclen );
inline uint32_t UTF8_getch( const std::string &str )
{
    const char *utf8str = str.c_str();
    int len = str.length();
    return UTF8_getch( &utf8str, &len );
}
// convert cursorx value to byte position
int cursorx_to_position( const char *line, int cursorx, int *prevpos = nullptr, int maxlen = -1 );
int utf8_width( const char *s, bool ignore_tags = false );
int utf8_width( const std::string &str, bool ignore_tags = false );
int utf8_width( const utf8_wrapper &str, bool ignore_tags = false );

std::string left_justify( const std::string &str, int width, bool ignore_tags = false );
std::string right_justify( const std::string &str, int width, bool ignore_tags = false );
std::string utf8_justify( const std::string &str, int width, bool ignore_tags = false );

/**
 * Center text inside whole line.
 * @param text to be centered.
 * @param start_pos printable position on line.
 * @param end_pos printable position on line.
 * @return First char position of centered text or start_pos if text is too big.
*/
int center_text_pos( const char *text, int start_pos, int end_pos );
int center_text_pos( const std::string &text, int start_pos, int end_pos );
int center_text_pos( const utf8_wrapper &text, int start_pos, int end_pos );
std::string utf32_to_utf8( uint32_t ch );
std::string utf8_truncate( const std::string &s, size_t length );

std::string base64_encode( const std::string &str );
std::string base64_decode( const std::string &str );

std::wstring utf8_to_wstr( const std::string &str );
std::string wstr_to_utf8( const std::wstring &wstr );

std::string native_to_utf8( const std::string &str );
std::string utf8_to_native( const std::string &str );

std::string utf32_to_utf8( const std::u32string &str );
std::u32string utf8_to_utf32( const std::string &str );

/**
 * UTF8-Wrapper over std::string.
 * It looks and feels like a std::string, but uses code points counts
 * as index, not bytes.
 * A multi-byte Unicode character might by represented
 * as 3 bytes in UTF8, this class will see these 3 bytes as 1 character.
 * It will never separate them. It will however split between code points
 * which might be problematic when containing combination characters.
 * In this case use the *_display functions. They operate on the display width.
 * Code points with a zero width are considered to belong to the previous code
 * point and are not split from that.
 * Having a string with like [letter0][letter1][combination-mark][letter2]
 * (assuming each letter has a width of 1) will return a display_width of 3.
 * substr_display(1, 2) returns [letter1][combination-mark][letter2],
 * substr_display(1, 1) returns [letter1][combination-mark]
 * substr_display(2, 1) returns [letter2]
 *
 * Note: functions use code points, not bytes, for counting/indexing!
 * Functions with the _display suffix use display width for counting/indexing!
 * Protected functions might behave different
 *
 * For function documentation see std::string, the functions here
 * mimic the behavior of the equally named std::string function.
 */
class utf8_wrapper
{
    public:
        utf8_wrapper() : _length( 0 ), _display_width( 0 ) { }
        utf8_wrapper( const std::string &d );
        utf8_wrapper( const char *d );

        void insert( size_t start, const utf8_wrapper &other );
        utf8_wrapper substr( size_t start, size_t length ) const;
        utf8_wrapper substr( size_t start ) const {
            return substr( start, _length - start );
        }
        void erase( size_t start, size_t length );
        void erase( size_t start ) {
            erase( start, _length - start );
        }
        void append( const utf8_wrapper &other );
        utf8_wrapper &replace_all( const utf8_wrapper &search, const utf8_wrapper &replace );
        /**
         * Returns a substring based on the display width, not the number of
         * code points (as the other substr function does).
         * @param start Start the returned substring with the character that is
         * at that position when this string would be have been printed (rounded down).
         * E.g. a string "a´a´" (where a is a normal character, and ` is a combination
         * code point) would be displayed as two cells: "áá".
         * substr_display(0,2) would return the whole string, substr_display(0,1)
         * would return the first two code points, substr_display(1,1) would return the
         * last two code points.
         * @param length Display length of the returned string, the returned string can
         * have a shorter display length (especially if the last character is a multi-cell
         * character and including it would exceed the length parameter).
         */
        utf8_wrapper substr_display( size_t start, size_t length ) const;
        utf8_wrapper substr_display( size_t start ) const {
            return substr_display( start, _length - start );
        }

        utf8_wrapper &operator=( const std::string &d ) {
            *this = utf8_wrapper( d );
            return *this;
        }
        const std::string &str() const {
            return _data;
        }

        // Returns Unicode character at position start
        uint32_t at( size_t start ) const;

        // Returns number of Unicode characters
        size_t size() const {
            return _length;
        }
        size_t length() const {
            return size();
        }
        bool empty() const {
            return size() == 0;
        }
        // Display size might be different from length, as some characters
        // are displayed as 2 chars in a terminal
        size_t display_width() const {
            return _display_width;
        }
        const char *c_str() const {
            return _data.c_str();
        }
        /**
         * Return a substring at most maxlength width (display width).
         * If the string had to shortened, an ellipsis (...) is added. The
         * string with the ellipsis will be exactly maxlength displayed
         * characters.
         */
        std::string shorten( size_t maxlength ) const;
    protected:
        std::string _data;
        size_t _length;
        size_t _display_width;
        // Byte offset into @ref _data for codepoint at index start.
        // bstart is a initial offset (in bytes!). The function operates on
        // _data.substr(bstart), it ignores everything before bstart.
        size_t byte_start( size_t bstart, size_t start ) const;
        // Byte offset into @ref _date for the codepoint starting at displayed cell start,
        // if the first character occupies two cells, than byte_start_display(2)
        // would return the byte offset of the second codepoint
        // byte_start_display(1) and byte_start_display(0) would return 0
        size_t byte_start_display( size_t bstart, size_t start ) const;
        // Same as @ref substr, but with a byte index as start
        utf8_wrapper substr_byte( size_t bytestart, size_t length, bool use_display_width ) const;
        void init_utf8_wrapper();
};

#endif
