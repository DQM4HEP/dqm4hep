/// \file RequestHandler.h
/*
 *
 * RequestHandler.h header template automatically generated by a class generator
 * Creation date : sam. d�c. 3 2016
 *
 * This file is part of DQM4HEP libraries.
 *
 * DQM4HEP is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * based upon these libraries are permitted. Any copy of these libraries
 * must include this copyright notice.
 *
 * DQM4HEP is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with DQM4HEP.  If not, see <http://www.gnu.org/licenses/>.
 *
 * @author Remi Ete
 * @copyright CNRS , IPNL
 */


#ifndef REQUESTHANDLER_H
#define REQUESTHANDLER_H

// -- std headers
#include <string>

// -- json headers
#include "json/json.h"

// -- dim headers
#include "dis.hxx"

// -- dqm4hep headers
#include "dqm4hep/DQMNet.h"

namespace dqm4hep {

  namespace net {

    class Server;
    template <typename T, typename S>
    class RequestHandlerT;

    /**
     * BaseRequestHandler class
     *
     * Base class for request handlers
     */
    class BaseRequestHandler
    {
      friend class Server;
    public:
      /**
       * Get the request type
       */
      const std::string &getType() const;

      /**
       * Get the request name
       */
      const std::string &getName() const;

      /**
       * Get the request full name, as it is allocated on the network
       */
      const std::string &getFullName() const;

      /**
       * Get the server in which the request handler is declared
       */
      Server *getServer() const;

      /**
       * Get the full request handle name from the request handler type and name
       *
       * @param type the request handler type
       * @param name the request handler name
       */
      static std::string getFullRequestHandlerName(const std::string &type, const std::string &name);

    protected:
      /**
       * Constructor
       *
       * @param pServer the server managing the request handler
       * @param type the request handler type
       * @param name the request handler name
       */
      BaseRequestHandler(Server *pServer, const std::string &type, const std::string &name);

      /**
       * Destructor
       */
      virtual ~BaseRequestHandler();

      /**
       * Create the actual request handler connection
       */
      virtual void startHandlingRequest() = 0;

      /**
       * Remove the actual request handler connection
       */
      virtual void stopHandlingRequest() = 0;

      /**
       * Whether the request handler is connected
       */
      virtual bool isHandlingRequest() const = 0;

    private:
      std::string           m_type;             ///< The request handler type
      std::string           m_name;             ///< The request handler name
      std::string           m_fullName;         ///< The request handler full name
      Server               *m_pServer;          ///< The server in which the request handler is declared
    };

    //-------------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------------

    /**
     * CommandHandler template class
     *
     * Base template class for request handlers. Can only by create by a Server
     * using the Server::createRequestHandler() method
     */
    template <typename T>
    class CommandHandler : public BaseRequestHandler
    {
      friend class Server;
      template <typename S, typename U> friend class CommandHandlerT;
    public:
      /**
       * Constructor
       *
       * @param pServer the server managing the command handler
       * @param type the command handler type
       * @param name the command handler name
       */
      CommandHandler(Server *pServer, const std::string &type, const std::string &name);

      /**
       * Destructor
       */
      virtual ~CommandHandler();

      /**
       * Process the command.
       * Supported response types are :
       *  - int
       *  - float
       *  - double
       *  - std::string
       *  - Json::Value
       *  - Buffer (see Buffer struct)
       *
       * @param command the command to process
       */
      virtual void processCommand(const T &command) = 0;

     private:
      /** Command class.
      *
      *  The concrete dim command implementation
      */
      class Command : public DimCommand
      {
      public:
        /**
         * Contructor
         */
        Command(CommandHandler<T> *pHandler);

        /**
         * The dim rpc handler
         */
        void commandHandler();

      private:
        CommandHandler<T>        *m_pHandler;     ///< The command handler owner instance
      };

      /**
       * Process the dim command request
       *
       * @param pCommand the command pointer to process
       */
      void processCommand(Command *pCommand);

      void startHandlingRequest();
      void stopHandlingRequest();
      bool isHandlingRequest() const;

    private:
      Command                   *m_pCommand;           ///< The concrete dim command implementation
    };

    //-------------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------------

    /**
     * CommandHandlerT template class
     *
     * Final implementation for handling commands
     * Supported response T types are :
     *  - int
     *  - float
     *  - double
     *  - std::string
     *  - Json::Value
     *  - Buffer (see Buffer struct)
     *
     *  The template argument S can be any class.
     */
    template <typename T, typename S>
    class CommandHandlerT : public CommandHandler<T>
    {
      friend class Server;
    public:
      typedef void (S::*CommandFunction)(const T &command);

      /**
       * Process the command.
       * Forward the command processing to the controller.
       */
      void processCommand(const T &response);

    private:
      /**
       * Constructor with command type and name
       *
       * @param pServer the server owning the command handler
       * @param type the command handler type
       * @param name the command handler name
       * @param pController the class instance that will handle the command
       * @param function the class method that will treat the command
       */
      CommandHandlerT(Server *pServer, const std::string &type, const std::string &name,
          S *controller, CommandFunction callback);

    private:
      S                    *m_pController;         ///< The command controller
      CommandFunction       m_function;            ///< The controller callback function
    };

    //-------------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------------

    /**
     * RequestHandler template class
     *
     * Base template class for request handlers. Can only by create by a Server
     * using the Server::createRequestHandler() method
     */
    template <typename T>
    class RequestHandler : public BaseRequestHandler
    {
      friend class Server;
      template <typename S, typename U> friend class RequestHandlerT;
    public:
      /**
       * Constructor
       *
       * @param pServer the server managing the request handler
       * @param type the request handler type
       * @param name the request handler name
       */
      RequestHandler(Server *pServer, const std::string &type, const std::string &name);

      /**
       * Destructor
       */
      virtual ~RequestHandler();

      /**
       * Process the request and fill the response
       * Supported response types are :
       *  - int
       *  - float
       *  - double
       *  - std::string
       *  - Json::Value
       *  - Buffer (see Buffer struct)
       *
       * @param request the json value describing the request to process
       * @param response the reponse the send back
       */
      virtual void processRequest(const Json::Value &request, T &response) = 0;

     private:
      /** Rpc class.
      *
      *  The concrete dim rpc implementation
      */
      class Rpc : public DimRpc
      {
      public:
        /**
         * Contructor
         */
        Rpc(RequestHandler<T> *pHandler);

        /**
         * The dim rpc handler
         */
        void rpcHandler();

      private:
        RequestHandler<T>        *m_pHandler;     ///< The request handler owner instance
      };

      /**
       * Process the dim rpc request
       *
       * @param pRpc the rpc pointer to process
       */
      void processRequest(Rpc *pRpc);

      void startHandlingRequest();
      void stopHandlingRequest();
      bool isHandlingRequest() const;

    private:
      Rpc                   *m_pRpc;              ///< The concrete dim rpc implementation
    };

    //-------------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------------

    /**
     * RequestHandlerT template class
     *
     * Final implementation for handling requests
     * Supported response T types are :
     *  - int
     *  - float
     *  - double
     *  - std::string
     *  - Json::Value
     *  - Buffer (see Buffer struct)
     *
     *  The template argument S can be any class.
     */
    template <typename T, typename S>
    class RequestHandlerT : public RequestHandler<T>
    {
      friend class Server;
    public:
      typedef void (S::*RequestFunction)(const Json::Value &request, T &response);

      /**
       * Process the request and fill the response.
       * Forward the request processing to the controller.
       */
      void processRequest(const Json::Value &request, T &response);

    private:
      /**
       * Constructor with request type and name
       *
       * @param pServer the server owning the request handler
       * @param type the request handler type
       * @param name the request handler name
       * @param pController the class instance that will handle the request
       * @param function the class method that will treat the request and provide a response
       */
      RequestHandlerT(Server *pServer, const std::string &type, const std::string &name,
          S *controller, RequestFunction callback);

    private:
      S                    *m_pController;         ///< The request controller
      RequestFunction       m_function;            ///< The controller callback function
    };

    //-------------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------------

    template <typename T>
    inline CommandHandler<T>::Command::Command(CommandHandler<T> *pHandler) :
        DimCommand(pHandler->getFullName().c_str(), "C"),
        m_pHandler(pHandler)
    {
      /* nop */
    }

    //-------------------------------------------------------------------------------------------------

    template <>
    inline CommandHandler<int>::Command::Command(CommandHandler<int> *pHandler) :
        DimCommand(pHandler->getFullName().c_str(), "I"),
        m_pHandler(pHandler)
    {
      /* nop */
    }

    //-------------------------------------------------------------------------------------------------

    template <>
    inline CommandHandler<float>::Command::Command(CommandHandler<float> *pHandler) :
        DimCommand(pHandler->getFullName().c_str(), "F"),
        m_pHandler(pHandler)
    {
      /* nop */
    }

    //-------------------------------------------------------------------------------------------------

    template <>
    inline CommandHandler<double>::Command::Command(CommandHandler<double> *pHandler) :
        DimCommand(pHandler->getFullName().c_str(), "D"),
        m_pHandler(pHandler)
    {
      /* nop */
    }

    //-------------------------------------------------------------------------------------------------

    template <>
    inline CommandHandler<Buffer>::Command::Command(CommandHandler<Buffer> *pHandler) :
        DimCommand(pHandler->getFullName().c_str(), "I:C"),
        m_pHandler(pHandler)
    {
      /* nop */
    }

    //-------------------------------------------------------------------------------------------------

    template <typename T>
    inline void CommandHandler<T>::Command::commandHandler()
    {
      m_pHandler->processCommand(this);
    }

    //-------------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------------

    template <typename T>
    inline RequestHandler<T>::Rpc::Rpc(RequestHandler<T> *pHandler) :
        DimRpc(pHandler->getFullName().c_str(), "C", "C"),
        m_pHandler(pHandler)
    {
      /* nop */
    }

    //-------------------------------------------------------------------------------------------------

    template <>
    inline RequestHandler<int>::Rpc::Rpc(RequestHandler<int> *pHandler) :
        DimRpc(pHandler->getFullName().c_str(), "C", "I"),
        m_pHandler(pHandler)
    {
      /* nop */
    }

    //-------------------------------------------------------------------------------------------------

    template <>
    inline RequestHandler<float>::Rpc::Rpc(RequestHandler<float> *pHandler) :
        DimRpc(pHandler->getFullName().c_str(), "C", "F"),
        m_pHandler(pHandler)
    {
      /* nop */
    }

    //-------------------------------------------------------------------------------------------------

    template <>
    inline RequestHandler<double>::Rpc::Rpc(RequestHandler<double> *pHandler) :
        DimRpc(pHandler->getFullName().c_str(), "C", "D"),
        m_pHandler(pHandler)
    {
      /* nop */
    }

    //-------------------------------------------------------------------------------------------------

    template <>
    inline RequestHandler<Buffer>::Rpc::Rpc(RequestHandler<Buffer> *pHandler) :
        DimRpc(pHandler->getFullName().c_str(), "C", "I:C"),
        m_pHandler(pHandler)
    {
      /* nop */
    }

    //-------------------------------------------------------------------------------------------------

    template <typename T>
    inline void RequestHandler<T>::Rpc::rpcHandler()
    {
      m_pHandler->processRequest(this);
    }

    //-------------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------------

    template <typename T>
    inline CommandHandler<T>::CommandHandler(Server *pServer, const std::string &type, const std::string &name) :
      BaseRequestHandler(pServer, type, name),
      m_pCommand(nullptr)
    {
      /* nop */
    }

    //-------------------------------------------------------------------------------------------------

    template <typename T>
    inline CommandHandler<T>::~CommandHandler()
    {
      if(this->isHandlingRequest())
        this->stopHandlingRequest();
    }

    //-------------------------------------------------------------------------------------------------

    template <typename T>
    inline void CommandHandler<T>::processCommand(Command *pCommand)
    {
      char *pContentStr = pCommand->getString();

      if(!pContentStr)
        return;

      std::string command(pContentStr);
      this->processCommand(command);
    }

    //-------------------------------------------------------------------------------------------------

    template <>
    inline void CommandHandler<Json::Value>::processCommand(Command *pCommand)
    {
      char *pContentStr = pCommand->getString();

      if(!pContentStr)
        return;

      Json::Reader reader;
      Json::Value command;

      bool parsingSuccessful = reader.parse(pContentStr, command);

      if(!parsingSuccessful)
        return;

      this->processCommand(command);
    }

    //-------------------------------------------------------------------------------------------------

    template <>
    inline void CommandHandler<Buffer>::processCommand(Command *pCommand)
    {
      const Buffer *pBuffer = (Buffer*) pCommand->getData();

      if(!pBuffer)
        return;

      if(!pBuffer->m_pBuffer || pBuffer->m_bufferSize == 0)
        return;

      this->processCommand(*pBuffer);
    }

    //-------------------------------------------------------------------------------------------------

    template <>
    inline void CommandHandler<int>::processCommand(Command *pCommand)
    {
      this->processCommand(pCommand->getInt());
    }

    //-------------------------------------------------------------------------------------------------

    template <>
    inline void CommandHandler<float>::processCommand(Command *pCommand)
    {
      this->processCommand(pCommand->getFloat());
    }

    //-------------------------------------------------------------------------------------------------

    template <>
    inline void CommandHandler<double>::processCommand(Command *pCommand)
    {
      this->processCommand(pCommand->getDouble());
    }

    //-------------------------------------------------------------------------------------------------

    template <typename T>
    inline void CommandHandler<T>::startHandlingRequest()
    {
      if(this->isHandlingRequest())
        return;

      m_pCommand = new Command(this);
    }

    //-------------------------------------------------------------------------------------------------

    template <typename T>
    inline void CommandHandler<T>::stopHandlingRequest()
    {
      if(!this->isHandlingRequest())
        return;

      delete m_pCommand;
      m_pCommand = nullptr;
    }

    //-------------------------------------------------------------------------------------------------

    template <typename T>
    inline bool CommandHandler<T>::isHandlingRequest() const
    {
      return (m_pCommand != nullptr);
    }

    //-------------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------------

    template <typename T>
    inline RequestHandler<T>::RequestHandler(Server *pServer, const std::string &type, const std::string &name) :
      BaseRequestHandler(pServer, type, name),
      m_pRpc(nullptr)
    {
      /* nop */
    }

    //-------------------------------------------------------------------------------------------------

    template <typename T>
    inline RequestHandler<T>::~RequestHandler()
    {
      if(this->isHandlingRequest())
        this->stopHandlingRequest();
    }

    //-------------------------------------------------------------------------------------------------

    template <typename T>
    inline void RequestHandler<T>::processRequest(Rpc *pRpc)
    {
      char *pContentStr = pRpc->getString();

      if(!pContentStr)
        return;

      Json::Reader reader;
      Json::Value request;

      bool parsingSuccessful = reader.parse(pContentStr, request);

      if(!parsingSuccessful)
        return;

      bool requireResponse(request.get("response", true).asBool());
      Json::Value userRequest(request.get("request", Json::Value()));
      T response;

      this->processRequest(userRequest, response);

      if(requireResponse)
        pRpc->setData(response);
    }

    //-------------------------------------------------------------------------------------------------

    template <>
    inline void RequestHandler<Json::Value>::processRequest(Rpc *pRpc)
    {
      char *pContentStr = pRpc->getString();

      if(!pContentStr)
        return;

      Json::Reader reader;
      Json::Value request;

      bool parsingSuccessful = reader.parse(pContentStr, request);

      if(!parsingSuccessful)
        return;

      bool requireResponse(request.get("response", true).asBool());
      Json::Value userRequest(request.get("request", Json::Value()));
      Json::Value response;

      this->processRequest(userRequest, response);

      if(requireResponse)
      {
        Json::FastWriter writer;
        std::string responseStr = writer.write(response);
        pRpc->setData(const_cast<char*>(responseStr.c_str()));
      }
    }

    //-------------------------------------------------------------------------------------------------

    template <>
    inline void RequestHandler<Buffer>::processRequest(Rpc *pRpc)
    {
      char *pContentStr = pRpc->getString();

      if(!pContentStr)
        return;

      Json::Reader reader;
      Json::Value request;

      bool parsingSuccessful = reader.parse(pContentStr, request);

      if(!parsingSuccessful)
        return;

      bool requireResponse(request.get("response", true).asBool());
      Json::Value userRequest(request.get("request", Json::Value()));
      Buffer response;

      this->processRequest(userRequest, response);

      if(requireResponse)
        pRpc->setData((void*)&response, sizeof(uint32_t) + response.m_bufferSize);
    }

    //-------------------------------------------------------------------------------------------------

    template <>
    inline void RequestHandler<std::string>::processRequest(Rpc *pRpc)
    {
      char *pContentStr = pRpc->getString();

      if(!pContentStr)
        return;

      Json::Reader reader;
      Json::Value request;

      bool parsingSuccessful = reader.parse(pContentStr, request);

      if(!parsingSuccessful)
        return;

      bool requireResponse(request.get("response", true).asBool());
      Json::Value userRequest(request.get("request", Json::Value()));
      std::string response;

      this->processRequest(userRequest, response);

      if(requireResponse)
        pRpc->setData(const_cast<char*>(response.c_str()));
    }

    //-------------------------------------------------------------------------------------------------

    template <typename T>
    inline void RequestHandler<T>::startHandlingRequest()
    {
      if(this->isHandlingRequest())
        return;

      m_pRpc = new Rpc(this);
    }

    //-------------------------------------------------------------------------------------------------

    template <typename T>
    inline void RequestHandler<T>::stopHandlingRequest()
    {
      if(!this->isHandlingRequest())
        return;

      delete m_pRpc;
      m_pRpc = nullptr;
    }

    //-------------------------------------------------------------------------------------------------

    template <typename T>
    inline bool RequestHandler<T>::isHandlingRequest() const
    {
      return (m_pRpc != nullptr);
    }

    //-------------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------------

    template <typename T, typename S>
    inline RequestHandlerT<T,S>::RequestHandlerT(Server *pServer, const std::string &type, const std::string &name, S *pController, RequestFunction function) :
        RequestHandler<T>(pServer, type, name),
        m_pController(pController),
        m_function(function)
    {
      /* nop */
    }

    //-------------------------------------------------------------------------------------------------

    template <typename T, typename S>
    inline void RequestHandlerT<T,S>::processRequest(const Json::Value &request, T &response)
    {
      (m_pController->*m_function)(request, response);
    }

    //-------------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------------

    template <typename T, typename S>
    inline CommandHandlerT<T,S>::CommandHandlerT(Server *pServer, const std::string &type, const std::string &name, S *pController, CommandFunction function) :
        CommandHandler<T>(pServer, type, name),
        m_pController(pController),
        m_function(function)
    {
      /* nop */
    }

    //-------------------------------------------------------------------------------------------------

    template <typename T, typename S>
    inline void CommandHandlerT<T,S>::processCommand(const T &command)
    {
      (m_pController->*m_function)(command);
    }

  }

}

#endif  //  REQUESTHANDLER_H
