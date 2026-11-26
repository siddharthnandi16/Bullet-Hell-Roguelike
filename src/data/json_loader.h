#pragma once
#include <cstdint>
#include <initializer_list>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

struct cJSON;

class DataError : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

// Non-owning view of one cJSON node. Valid only while its JsonDocument is alive.
class JsonNode {
public:
    JsonNode(const cJSON* node, std::string file, std::string path);

    std::string where() const;
    [[noreturn]] void fail(const std::string& message) const;

    void require_only(std::initializer_list<const char*> allowed) const;
    bool has(const char* key) const;
    JsonNode child(const char* key) const;
    std::vector<JsonNode> items() const;
    std::vector<std::pair<std::string, JsonNode>> members() const;

    int as_int() const;
    double as_number() const;
    std::string as_string() const;
    bool as_bool() const;
    std::string as_raw() const;  // compact JSON text, used for scripts

    int get_int(const char* key) const { return child(key).as_int(); }
    double get_number(const char* key) const { return child(key).as_number(); }
    std::string get_string(const char* key) const { return child(key).as_string(); }
    bool get_bool(const char* key) const { return child(key).as_bool(); }
    std::string get_raw(const char* key) const { return child(key).as_raw(); }
    std::vector<JsonNode> get_array(const char* key) const { return child(key).items(); }
    std::vector<std::string> get_strings(const char* key) const;

private:
    const cJSON* node_;
    std::string file_;
    std::string path_;
};

// Owns a parsed cJSON tree and frees it on destruction.
class JsonDocument {
public:
    explicit JsonDocument(const std::string& file_path);
    ~JsonDocument();
    JsonDocument(const JsonDocument&) = delete;
    JsonDocument& operator=(const JsonDocument&) = delete;

    JsonNode root() const;

private:
    cJSON* root_ = nullptr;
    std::string file_;
};

// Opens a file, parses it, and passes the root to f. The tree is freed after f returns.
template <typename F>
void with_json_file(const std::string& path, F&& f) {
    JsonDocument doc(path);
    f(doc.root());
}