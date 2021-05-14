#ifndef payhttp_CONSTANT_HPP
#define payhttp_CONSTANT_HPP


#include <string>

namespace payhttp {
    namespace statusCode {
        const unsigned short ok = 200;
        const unsigned short created = 201;
        const unsigned short accepted = 202;
        const unsigned short no_content = 204;
        const unsigned short multiple_choices = 300;
        const unsigned short moved_permanently = 301;
        const unsigned short moved_temporarily = 302;
        const unsigned short not_modified = 304;
        const unsigned short bad_request = 400;
        const unsigned short unauthorized = 401;
        const unsigned short forbidden = 403;
        const unsigned short not_found = 404;
        const unsigned short internal_server_error = 500;
        const unsigned short not_implemented = 501;
        const unsigned short bad_gateway = 502;
        const unsigned short service_unavailable = 503;
    }

    namespace mimeTypes {
        std::string extensionToType(const std::string& extension);
    }

    namespace stockReplies {

        const char ok[] = "<html>"
                          "<head><title>ok</title></head>"
                          "<body><h1>200 ok</h1></body>"
                          "<a href=\"javascript:history.back(-1)\">返回上一页</a>"
                          "<br/><br/><a href=\"/index.html\">返回登录页</a>"
                          "</html>";
        const char created[] =
                "<html>"
                "<head><title>Created</title></head>"
                "<body><h1>201 Created</h1></body>"
                "<a href=\"javascript:history.back(-1)\">返回上一页</a>"
                "<br/><br/><a href=\"/index.html\">返回登录页</a>"
                "</html>";
        const char accepted[] =
                "<html>"
                "<head><title>Accepted</title></head>"
                "<body><h1>202 Accepted</h1></body>"
                "<a href=\"javascript:history.back(-1)\">返回上一页</a>"
                "</html>";
        const char no_content[] =
                "<html>"
                "<head><title>No Content</title></head>"
                "<body><h1>204 Content</h1></body>"
                "<a href=\"javascript:history.back(-1)\">返回上一页</a>"
                "</html>";
        const char multiple_choices[] =
                "<html>"
                "<head><title>Multiple Choices</title></head>"
                "<body><h1>300 Multiple Choices</h1></body>"
                "<a href=\"javascript:history.back(-1)\">返回上一页</a>"
                "</html>";
        const char moved_permanently[] =
                "<html>"
                "<head><title>Moved Permanently</title></head>"
                "<body><h1>301 Moved Permanently</h1></body>"
                "<a href=\"javascript:history.back(-1)\">返回上一页</a>"
                "</html>";
        const char moved_temporarily[] =
                "<html>"
                "<head><title>Moved Temporarily</title></head>"
                "<body><h1>302 Moved Temporarily</h1></body>"
                "<a href=\"javascript:history.back(-1)\">返回上一页</a>"
                "</html>";
        const char not_modified[] =
                "<html>"
                "<head><title>Not Modified</title></head>"
                "<body><h1>304 Not Modified</h1></body>"
                "<a href=\"javascript:history.back(-1)\">返回上一页</a>"
                "</html>";
        const char bad_request[] =
                "<html>"
                "<head><title>Bad Request</title></head>"
                "<body><h1>400 Bad Request</h1></body>"
                "<a href=\"javascript:history.back(-1)\">返回上一页</a>"
                "<br/><br/><a href=\"/index.html\">返回登录页</a>"
                "</html>";
        const char unauthorized[] =
                "<html>"
                "<head><title>Unauthorized</title></head>"
                "<body><h1>401 Unauthorized</h1></body>"
                "<a href=\"javascript:history.back(-1)\">返回上一页</a>"
                "</html>";
        const char forbidden[] =
                "<html>"
                "<head><title>Forbidden</title></head>"
                "<body><h1>403 Forbidden</h1></body>"
                "<a href=\"javascript:history.back(-1)\">返回上一页</a>"
                "</html>";
        const char not_found[] =
                "<html>"
                "<head><title>Not Found</title></head>"
                "<body><h1>404 Not Found</h1></body>"
                "<a href=\"javascript:history.back(-1)\">返回上一页</a>"
                "<br/><br/><a href=\"/index.html\">返回登录页</a>"
                "</html>";
        const char internal_server_error[] =
                "<html>"
                "<head><title>Internal Server Error</title></head>"
                "<body><h1>500 Internal Server Error</h1></body>"
                "<a href=\"javascript:history.back(-1)\">返回上一页</a>"
                "</html>";
        const char not_implemented[] =
                "<html>"
                "<head><title>Not Implemented</title></head>"
                "<body><h1>501 Not Implemented</h1></body>"
                "<a href=\"javascript:history.back(-1)\">返回上一页</a>"
                "</html>";
        const char bad_gateway[] =
                "<html>"
                "<head><title>Bad Gateway</title></head>"
                "<body><h1>502 Bad Gateway</h1></body>"
                "<a href=\"javascript:history.back(-1)\">返回上一页</a>"
                "</html>";
        const char service_unavailable[] =
                "<html>"
                "<head><title>Service Unavailable</title></head>"
                "<body><h1>503 Service Unavailable</h1></body>"
                "<a href=\"javascript:history.back(-1)\">返回上一页</a>"
                "</html>";

        std::string toHtml(const unsigned short status);

        std::string toString(const unsigned short status);
    } // namespace stockReplies
}
#endif //payhttp_CONSTANT_HPP
