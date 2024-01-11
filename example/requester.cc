/*
 * Copyright (C) 2014 Open Source Robotics Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
*/

#include <chrono>
#include <iostream>
#include <gz/msgs.hh>
#include <gz/transport.hh>

//////////////////////////////////////////////////
/// \brief Provide an "echo" service.
bool srvEcho(const gz::msgs::StringMsg &_req,
  gz::msgs::StringMsg &_rep)
{
  // Set the response's content.
  _rep.set_data(_req.data());

  // The response succeed.
  return true;
}

class Responder {
public:
  Responder() {
    // Request the "/echo" service.
    std::string service = "/echo";

    // Advertise a service.
    if (!this->node.Advertise(service, srvEcho)) {
      std::cerr << "Error advertising service [" << service << "]" << std::endl;
    }
  }

private:
  gz::transport::Node node;
};

void runResponder()
{
  using namespace std::literals;
  std::this_thread::sleep_for(2s);
  std::cout << "Responder started" << std::endl;
  Responder responder;
  std::this_thread::sleep_for(10s);
  std::cout << "Responder finished" << std::endl;
}

//////////////////////////////////////////////////
int main(int argc, char **argv)
{

  // Prepare the input parameters.
  gz::msgs::StringMsg req;
  req.set_data("HELLO");

  gz::msgs::StringMsg rep;
  bool result;
  unsigned int timeout = 5000;

  auto th = std::thread(runResponder);

  // Create a transport node.
  gz::transport::Node node;
  bool executed = node.Request("/echo", req, timeout, rep, result);

  if (executed)
  {
    if (result)
      std::cout << "Response: [" << rep.data() << "]" << std::endl;
    else
      std::cout << "Service call failed" << std::endl;
  }
  else
    std::cerr << "Service call timed out" << std::endl;

  th.join();
}
