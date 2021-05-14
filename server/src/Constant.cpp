#include "server/include/Constant.hpp"

namespace payhttp {

    namespace mimeTypes {

        class Mapping {
        public:
            const char* extension;
            const char* mime_type;
        };

        Mapping mappings[] =
                {
                        {"gif",  "image/gif"},
                        {"htm",  "text/html; charset=utf-8"},
                        {"html", "text/html; charset=utf-8"},
                        {"jpg",  "image/jpeg"},
                        {"png",  "image/png"}
                };

        // 将文件扩展名转为http的type
        std::string extensionToType(const std::string& extension) {
            for (Mapping m: mappings) {
                if (m.extension == extension) {
                    return m.mime_type;
                }
            }
            return "text/plain; charset=utf-8";
        }
    }

// 对应状态的html默认页面
    namespace stockReplies {
        std::string toHtml(const unsigned short status) {
            switch (status) {
                case statusCode::ok:
                    return ok;
                case statusCode::created:
                    return created;
                case statusCode::accepted:
                    return accepted;
                case statusCode::no_content:
                    return no_content;
                case statusCode::multiple_choices:
                    return multiple_choices;
                case statusCode::moved_permanently:
                    return moved_permanently;
                case statusCode::moved_temporarily:
                    return moved_temporarily;
                case statusCode::not_modified:
                    return not_modified;
                case statusCode::bad_request:
                    return bad_request;
                case statusCode::unauthorized:
                    return unauthorized;
                case statusCode::forbidden:
                    return forbidden;
                case statusCode::not_found:
                    return not_found;
                case statusCode::internal_server_error:
                    return internal_server_error;
                case statusCode::not_implemented:
                    return not_implemented;
                case statusCode::bad_gateway:
                    return bad_gateway;
                case statusCode::service_unavailable:
                    return service_unavailable;
                default:
                    return internal_server_error;
            }
        }

        std::string toString(const unsigned short status) {
            switch (status) {
                case statusCode::ok:
                    return "OK";
                case statusCode::created:
                    return "Created";
                case statusCode::accepted:
                    return "Accepted";
                case statusCode::no_content:
                    return "No Content";
                case statusCode::multiple_choices:
                    return "Multiple Choices";
                case statusCode::moved_permanently:
                    return "Moved Permanently";
                case statusCode::moved_temporarily:
                    return "Moved Temporarily";
                case statusCode::not_modified:
                    return "Not Modified";
                case statusCode::bad_request:
                    return "Bad Request";
                case statusCode::unauthorized:
                    return "Unauthorized";
                case statusCode::forbidden:
                    return "Forbidden";
                case statusCode::not_found:
                    return "Not Found";
                case statusCode::internal_server_error:
                    return "Internal Server Error";
                case statusCode::not_implemented:
                    return "Not Implemented";
                case statusCode::bad_gateway:
                    return "Bad Gateway";
                case statusCode::service_unavailable:
                    return "Service Unavailable";
                default:
                    return "Internal Server Error";
            }
        } // namespace stockReplies
    }
}