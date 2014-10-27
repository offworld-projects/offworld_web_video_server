/*********************************************************************
 *
 * Software License Agreement (BSD License)
 *
 *  Copyright (c) 2014, Worcester Polytechnic Institute
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *   * Neither the name of the Worcester Polytechnic Institute nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *
 *********************************************************************/

#include <boost/regex.hpp>
#include "web_video_server/http_server/request_handler.hpp"

namespace web_video_server {
namespace http_server {

RequestHandlerGroup::RequestHandlerGroup(HttpServerRequestHandler default_handler)
  : default_handler_(default_handler) {}


class PathMatcher {
public:
  explicit PathMatcher(const std::string& path_regex_string)
    : path_regex_(boost::regex(path_regex_string)) {}
  bool operator()(const HttpRequest& request){
    return regex_match(request.path, path_regex_);
  }
private:
  const boost::regex path_regex_;
};
void RequestHandlerGroup::addHandlerForPath(const std::string& path_regex, HttpServerRequestHandler handler){
  addHandler(PathMatcher(path_regex), handler);
}
void RequestHandlerGroup::addHandler(HandlerPredicate predicate, HttpServerRequestHandler handler) {
  handlers_.push_back(std::make_pair(predicate, handler));
}


void RequestHandlerGroup::operator()(const HttpRequest& request, boost::shared_ptr<HttpConnection> connection){
  for(int i = 0; i < handlers_.size(); ++i) {
    std::pair<HandlerPredicate, HttpServerRequestHandler>& handler = handlers_[i];
    if(handler.first(request)) {
      handler.second(request, connection);
      return;
    }
  }
  default_handler_(request, connection);
}

}
}