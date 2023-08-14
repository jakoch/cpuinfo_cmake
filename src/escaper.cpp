#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <chrono>
#include <ctime>
#include <iomanip>

/**
 * @brief Function to perform string replacement.
 *
 * @param input
 * @param search
 * @param replacement
 * @return replaced string
 */
std::string replaceString(const std::string& input, const std::string& search, const std::string& replace)
{
    std::string result = input;
    size_t pos = 0;

    while ((pos = result.find(search, pos)) != std::string::npos) {
        result.replace(pos, search.length(), replace);
        pos += replace.length();
    }

    return result;
}

/**
 * @brief Function to perform multiple string replacements using replaceString().
 *
 * @param input
 * @param replacements
 * @return string
 */
std::string multiReplaceString(const std::string& input, const std::vector<std::pair<std::string, std::string>>& replacements)
{
    std::string result = input;

    for (const auto& replacement : replacements) {
        result = replaceString(result, replacement.first, replacement.second);
    }

    return result;
}

/**
 * @brief Function to read file contents
 *
 * @param filename
 * @return std::string
 */
std::string readFile(const std::string& filename)
{
    std::ifstream inputFile(filename);

    if (!inputFile.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return "";
    }

    std::string fileContents((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());

    inputFile.close();

    return fileContents;
}

/**
 * @brief Function to write file contents
 *
 * @param filename
 * @param contents
 * @return bool
 */
bool writeFile(const std::string& filename, const std::string& contents)
{
    std::ofstream outputFile(filename);
    if (!outputFile.is_open()) {
        std::cerr << "Error creating output file: " << filename << std::endl;
        return false;
    }

    outputFile << contents;
    outputFile.close();

    std::cout << "File successfully written: " << filename << std::endl;

    return true;
}

/**
 * @brief Function to align lines containing "+NL+"
 *
 * @param input
 * @return std::string aligned +NL+
 */
std::string alignNL(const std::string& input)
{
    std::string result;
    size_t maxLineLength = 0;

    // Split input into lines
    std::istringstream iss(input);
    std::vector<std::string> lines;
    std::string line;

    while (std::getline(iss, line))
    {
        // if the line contains a "+NL+"
        // find position after the last double-quote
        // set it as new maxLineLength, if greater then old
        if (line.find("+NL+") != std::string::npos) {
            size_t pos_after_last_double_quote = line.rfind(R"(")") + 1;
            maxLineLength = std::max(maxLineLength, pos_after_last_double_quote);
        }
        lines.push_back(line);
    }

    // calculate the offsets and create modified lines with padding
    std::stringstream modifiedSS;
    const std::string seperator = " ";
    for (const std::string &currentLine : lines)
    {
        size_t pos_nl = currentLine.find("+NL+");
        if (pos_nl != std::string::npos) {
            if(pos_nl <= maxLineLength) {
                // +NL+ token is inside the string up to maxLineLength
                // use pos_nl and fill up the diff with more spaces
                size_t diff = maxLineLength - pos_nl + seperator.length() ;
                modifiedSS << currentLine.substr(0, pos_nl) << std::string(diff, ' ') << "+NL+\n";
            } else {
                modifiedSS << currentLine.substr(0, maxLineLength) << std::string(seperator.length(), ' ') << "+NL+\n";
            }
        } else {
            modifiedSS << currentLine << "\n";
        }
    }

    // Get the final modified result
    result = modifiedSS.str();

    return result;
}

/**
 * @brief Get the Current Time object (C++11 compat)
 *
 * @return std::string
 */
std::string getCurrentTime()
{
    // get current time
    auto currentTime = std::chrono::system_clock::now();
    std::time_t currentTime_t = std::chrono::system_clock::to_time_t(currentTime);

    // convert time to tm struct
    std::tm* localTime = std::localtime(&currentTime_t);

    // format as a string
    std::ostringstream timeStream;
    timeStream << std::put_time(localTime, "%Y-%m-%d %H:%M:%S");

    return timeStream.str();
}

int main(int argc, char* argv[])
{
    /**
     * Step 1
     *
     * Escape and double-escape "cpuinfo.cpp" source for embedding into CMake script.
     */

    std::cout << "Escaping cpuinfo.cpp source for embedding into CMake script..\n";

    auto cpp_source = readFile("cpuinfo.cpp");
    if (cpp_source.empty()) { return 1; }

    // define search-replace pairs
    std::vector<std::pair<std::string, std::string>> replacements_for_cpp_source;
    replacements_for_cpp_source.emplace_back( R"(\n)"        , R"(\\\\n)"       ); // WARNING
    replacements_for_cpp_source.emplace_back( R"(")"         , R"(\")"          ); // REPLACEMENTS
    replacements_for_cpp_source.emplace_back( R"( \\")"      , R"( \\\\\")"     ); // ORDER IS IMPORTANT
    replacements_for_cpp_source.emplace_back( R"(\\": )"     , R"(\\\\\": )"    );
    replacements_for_cpp_source.emplace_back( R"( \"\\"\")"  , R"( \"\\\\\"\")" );
    replacements_for_cpp_source.emplace_back( R"( \"\\",\")" , R"( \"\\\\\",\")");

    replacements_for_cpp_source.emplace_back(
        R"(console_output = true)",
        R"(console_output = false)"
    );

    // add CMAKE_BINARY_DIR is important to find the file, else it gets lost in a cmake working dir
    replacements_for_cpp_source.emplace_back(
        R"(ofstream file(\"cpuinfo.json\", std)",
        R"(ofstream file(\"${CMAKE_BINARY_DIR}/cpuinfo.json\", std)"
    );

    std::string escaped_source = multiReplaceString(cpp_source, replacements_for_cpp_source);

    escaped_source = alignNL(escaped_source);

    /**
     * Step 2
     *
     * Insert the escaped "cpuinfo.cpp" source into the "cpuinfo.cmake.tpl" template file.
     * Finally, generate "cpuinfo.cmake".
     */

    std::cout << "Inserting escaped 'cpuinfo.cpp' source into 'cpuinfo.cmake.tpl' template file..\n";

    auto cmake_template_source = readFile("cpuinfo.cmake.tpl");
    if (cmake_template_source.empty()) { return 1; }

    std::string header = "Auto-generated from cpuinfo.cpp. Date / Time: " + getCurrentTime();

    std::vector<std::pair<std::string, std::string>> replacements_for_cmake_tpl;
    replacements_for_cmake_tpl.emplace_back("{{generated_header}}",  header);
    replacements_for_cmake_tpl.emplace_back("{{insert_cpp_source}}", escaped_source);

    std::string cmake_source = multiReplaceString(cmake_template_source, replacements_for_cmake_tpl);

    if(!writeFile("CPUINFO.cmake", cmake_source)) {
        std::cout << "Error.";
        return 1;
    }

    std::cout << "CPUINFO.cmake was generated successfully.\nDone.";
    return 0;
}