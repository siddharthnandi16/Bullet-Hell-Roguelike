#include "data/json_loader.h"
#include "data/json_loader.h"

#include "cJSON.h"

#include <fstream>
#include <sstream>

JsonNode::JsonNode(const cJSON* node, std::string file, std::string path)
    : node_(node), file_(std::move(file)), path_(std::move(path)) {}

std::string JsonNode::where() const {
    return path_.empty() ? file_ : file_ + ": " + path_;
}

void JsonNode::fail(const std::string& message) const {
    throw DataError(where() + ": " + message);
}

void JsonNode::require_only(std::initializer_list<const char*> allowed) const {
    if (!cJSON_IsObject(node_)) fail("expected an object");
    for (const cJSON* c = node_->child; c; c = c->next) {
        bool ok = false;
        for (const char* a : allowed)
            if (std::string(c->string) == a) ok = true;
        if (!ok) fail(std::string("unknown key '") + c->string + "'");
    }
}

bool JsonNode::has(const char* key) const {
    return cJSON_IsObject(node_) && cJSON_GetObjectItemCaseSensitive(node_, key) != nullptr;
}

JsonNode JsonNode::child(const char* key) const {
    if (!cJSON_IsObject(node_)) fail("expected an object");
    const cJSON* item = cJSON_GetObjectItemCaseSensitive(node_, key);
    if (!item) fail(std::string("missing key '") + key + "'");
    return JsonNode(item, file_, path_.empty() ? key : path_ + "." + key);
}

std::vector<JsonNode> JsonNode::items() const {
    if (!cJSON_IsArray(node_)) fail("expected an array");
    std::vector<JsonNode> out;
    int i = 0;
    for (const cJSON* e = node_->child; e; e = e->next, ++i)
        out.emplace_back(e, file_, path_ + "[" + std::to_string(i) + "]");
    return out;
}

std::vector<std::pair<std::string, JsonNode>> JsonNode::members() const {
    if (!cJSON_IsObject(node_)) fail("expected an object");
    std::vector<std::pair<std::string, JsonNode>> out;
    for (const cJSON* c = node_->child; c; c = c->next) {
        std::string key = c->string;
        out.emplace_back(key, JsonNode(c, file_, path_.empty() ? key : path_ + "." + key));
    }
    return out;
}

int JsonNode::as_int() const {
    if (!cJSON_IsNumber(node_) || node_->valuedouble != static_cast<double>(node_->valueint))
        fail("expected an integer");
    return node_->valueint;
}

double JsonNode::as_number() const {
    if (!cJSON_IsNumber(node_)) fail("expected a number");
    return node_->valuedouble;
}

std::string JsonNode::as_string() const {
    if (!cJSON_IsString(node_)) fail("expected a string");
    return node_->valuestring;
}

bool JsonNode::as_bool() const {
    if (!cJSON_IsBool(node_)) fail("expected true or false");
    return cJSON_IsTrue(node_);
}

std::string JsonNode::as_raw() const {
    char* text = cJSON_PrintUnformatted(node_);
    std::string out = text ? text : "";
    cJSON_free(text);
    return out;
}

std::vector<std::string> JsonNode::get_strings(const char* key) const {
    std::vector<std::string> out;
    for (const JsonNode& item : child(key).items()) out.push_back(item.as_string());
    return out;
}

JsonDocument::JsonDocument(const std::string& file_path) : file_(file_path) {
    std::ifstream in(file_path, std::ios::binary);
    if (!in) throw DataError(file_path + ": cannot open file");

    std::stringstream buffer;
    buffer << in.rdbuf();
    const std::string text = buffer.str();

    root_ = cJSON_Parse(text.c_str());
    if (!root_) {
        const char* at = cJSON_GetErrorPtr();
        const std::string snippet = at ? std::string(at).substr(0, 40) : "";
        throw DataError(file_path + ": invalid JSON near '" + snippet + "'");
    }
}

JsonDocument::~JsonDocument() {
    cJSON_Delete(root_);
}

JsonNode JsonDocument::root() const {
    return JsonNode(root_, file_, "");
}