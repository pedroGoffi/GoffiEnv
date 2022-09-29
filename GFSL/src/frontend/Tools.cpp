#ifndef LEXING_STAGE_SRC_CODE
#define LEXING_STAGE_SRC_CODE value
#include "./Tools.h"
#include <sstream>

void usage(FILE *stream, std::string program){
    /*  Since prinf has a beatiful sintax i'll use it instead of std::cout  */
    fprintf(stream, "Usage: %s [OPTIONS] <input.goffi>\n", program.c_str());
    fprintf(stream, "OPTIONS:\n");
    fprintf(stream, "    -h          print this help in stdout\n");
    fprintf(stream, "    -o <path>   provide output path\n");
}
char *shift(int *argc, char ***argv){
    assert(*argc > 0);
    char* result = **argv;
    *argv += 1;
    *argc -= 1;
    return result;
}
bool is_number(const std::string& str){
    for(char const &c : str){
        if (std::isdigit(c) == 0) return false;
    }
    return true;
}
template <typename I> std::string n2hexstr(I w, size_t hex_len = sizeof(I)<<1) {
    static const char* digits = "0123456789ABCDEF";
    std::string rc(hex_len,'0');
    for (size_t i=0, j=(hex_len-1)*4 ; i<hex_len; ++i,j-=4)
        rc[i] = digits[(w>>j) & 0x0f];
    return rc;
}
std::vector<std::byte> to_bytes(std::string const& s)
{
    std::vector<std::byte> bytes;
    bytes.reserve(std::size(s));
      
    std::transform(std::begin(s), std::end(s), std::back_inserter(bytes), [](char c){
        return std::byte(c);
    });

    return bytes;
}
std::string string_to_hex(const std::string& input)
{
    static const char hex_digits[] = "0123456789ABCDEF";

    std::string output;
    output.reserve(input.length() * 2);
    for (auto c = input.begin(); c != input.end(); ++c)
    {
      output.push_back('0');
      output.push_back('x');
      output.push_back(hex_digits[*c >> 4]);
      output.push_back(hex_digits[*c & 15]);
      output.push_back(',');
    }
    if (output[output.length() - 1] == ',')
      output = output.substr(0, output.length() - 1);
    return output;
}
std::uint64_t string_to_uint64(std::string val){
    uint64_t value;
    std::istringstream iss(val);
    iss >> value >> std::hex;

    return value;
}
#endif /* ifndef LEXING_STAGE_SRC_CODE */
