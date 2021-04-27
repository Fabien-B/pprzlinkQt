/*
 * Copyright 2019 garciafa
 * This file is part of PprzLinkCPP
 *
 * PprzLinkCPP is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * PprzLinkCPP is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with ModemTester.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

/** \file IvyLink.cpp
 *
 *
 */


#include <pprzlink/IvyLink.h>
#include <pprzlink/exceptions/pprzlink_exception.h>
#include <ivy-c++/IvyApplication.h>
#include <iostream>
#include <regex>

namespace pprzlink {

  long IvyLink::BindOnSrcAc(std::string ac_id, messageCallback_t cb)
  {
  auto mcb = new AircraftCallback(dictionary, cb);
    std::stringstream regexp;
    regexp << "^(" << ac_id << ") " << "([^ ]*)( .*)?$";
    //std::cout << "Binding to " << regexp.str() << std::endl;
    auto id = bus->BindMsg(regexp.str().c_str(), mcb);
    aircraftCallbackMap[id] = mcb;
    return id;
  }

  long IvyLink::registerRequestAnswerer(const MessageDefinition &def, answererCallback_t cb) {
    if(!def.isRequest()) {
      throw message_is_not_request("Message " + def.getName() + " is not a request message. Use sendMessage instead!");
    }

    // remove last 4 characters (_REQ) from request name to get the answer message name
    auto ansName = def.getName().substr(0, def.getName().size() - 4);

    std::stringstream regexpStream;
    regexpStream << "^([^ ]*) ([^ ]*) " << messageRegexp(def);

    auto mcb = new RequestCallback(dictionary, [=](std::string ac_id,Message msg) {
      auto answerMsg = cb(std::move(ac_id), std::move(msg));
      //check message name
      if(ansName != answerMsg.getDefinition().getName()) {
        throw wrong_answer_to_request("Wrong answer " + answerMsg.getDefinition().getName() + " to request " + def.getName());
      }
      sendMessage(answerMsg);
    });
    auto id = bus->BindMsg(regexpStream.str().c_str(), mcb);
    requestCallbackMap[id] = mcb;

  return 0;
  }


  void RequestCallback::OnMessage(IvyApplication *app, int argc, const char **argv) {
    (void)app;

    if (argc < 3)
    {
      throw bad_message_file("Not enough fields to be a valid request!");
    }

    requestId = std::string(argv[1]);

    MessageCallback::OnMessage(app, argc-1, argv+1);
  }

  void AircraftCallback::OnMessage(IvyApplication *app, int argc, const char **argv)
  {
    (void)app;
    MessageDefinition def = dictionary.getDefinition(argv[1]);
    Message msg(def);

    if (argc==3) // If we have fields to parse
    {
      std::regex fieldRegex("([^ ]+|\"[^\"]+\")");

      std::smatch results;
      std::vector<std::string> fields;
      std::string fieldsStr(argv[2]);
      while (std::regex_search(fieldsStr, results, fieldRegex))
      {
        fields.push_back(results.str());
        fieldsStr = results.suffix();
      }

      if (def.getNbFields()!=fields.size()) // check that the number of fields is correct
      {
        throw wrong_message_format("Wrong number of fields in message " + std::string(argv[1]));
      }

      for (size_t i = 0; i < def.getNbFields(); ++i)
      {
        const auto &field = def.getField(i);

        // For char arrays and strings remove possible quotes
        if ((field.getType().getBaseType() == BaseType::STRING ||
             (field.getType().getBaseType() == BaseType::CHAR && field.getType().isArray())) && argv[i][0] == '"')
        {
          std::string &str(fields[i]);
          //std::cout << str.substr(1,str.size()-2) << std::endl;
          msg.addField(field.getName(), str.substr(1, str.size() - 2));
        }
        else
        {
          std::stringstream sstr(fields[i]);
          if (field.getType().isArray())
          {
            switch (field.getType().getBaseType())
            {
              case BaseType::NOT_A_TYPE:
                throw std::logic_error("NOT_A_TYPE for field " + field.getName() + " in message " + argv[1]);
                break;
              case BaseType::CHAR:
                throw wrong_message_format("Wrong field format for a char[] " + fields[i]);
                break;
              case BaseType::INT8:
              case BaseType::INT16:
              case BaseType::INT32:
              case BaseType::UINT8:
              case BaseType::UINT16:
              case BaseType::UINT32:
              case BaseType::FLOAT:
              {
                // Parse all numbers as a double
                std::vector<double> values;
                while (!sstr.eof())
                {
                  double val;
                  char c;
                  sstr >> val >> c;
                  if (c != ',')
                  {
                    throw wrong_message_format("Wrong format for array \"" + fields[i] + "\"");
                  }
                  values.push_back(val);
                }
                msg.addField(field.getName(), values); // The value will be statically cast to the right type
              }
                break;
              case BaseType::STRING:
                msg.addField(field.getName(), fields[i]);
                break;
            }
          }
          else
          {
            switch (field.getType().getBaseType())
            {
              case BaseType::NOT_A_TYPE:
                throw std::logic_error("NOT_A_TYPE for field " + field.getName() + " in message " + argv[1]);
                break;
              case BaseType::CHAR:
              {
                char val;
                sstr >> val;
                msg.addField(field.getName(), val);
              }
                break;
              case BaseType::INT8:
              case BaseType::INT16:
              case BaseType::INT32:
              case BaseType::UINT8:
              case BaseType::UINT16:
              case BaseType::UINT32:
              case BaseType::FLOAT:
              {
                // Parse all numbers as a double
                double val;
                sstr >> val;
                msg.addField(field.getName(), val); // The value will be statically cast to the right type
              }
                break;
              case BaseType::STRING:
                msg.addField(field.getName(), fields[i]);
                break;
            }
          }
        }
      }
    }
    std::string sender(argv[0]);
    if (argv[0][0]=='"')
    {
      // Remove quotes from string if needed
      sender=sender.substr(1,sender.size()-2);
    }
    msg.setSenderId(sender);

    cb(sender, msg);
  }
}
