//==========================================================================
//  DQM4hep a data quality monitoring software for high energy physics 
//--------------------------------------------------------------------------
//
// For the licensing terms see $DQM4hep_DIR/LICENSE.
// For the list of contributors see $DQM4hep_DIR/AUTHORS.
//
// Author     : R.Ete
//====================================================================

#ifndef DQM4HEP_WEBSOCKETSERVER_H
#define DQM4HEP_WEBSOCKETSERVER_H

// -- dqm4hep headers
#include <dqm4hep/Internal.h>
#include <dqm4hep/Signal.h>

// -- std headers
#include <functional>

// -- websocketpp headers
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

namespace dqm4hep {

  namespace net {
    
    typedef websocketpp::server<websocketpp::config::asio> server;
    typedef websocketpp::connection_hdl connection_hdl;
    typedef std::set<server::connection_ptr> connection_set;
    typedef std::map<std::string, connection_set> connection_map;
    typedef server::message_ptr message_ptr;
    typedef server::connection_type::message_type message_type;
    typedef message_ptr WsMessage;
    typedef server::connection_ptr WsConnection;

    class WsServer;
    
    //-------------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------------
    
    /**
     *  @brief  ServiceType enumerator
     */
    enum ServiceType {
      UNKNOWN_TYPE = 0,     ///< Unkown service type (initial value)
      SERVICE_TYPE = 1,     ///< Service type: server -> client
      COMMAND_TYPE = 2,     ///< Command type: server <- client 
      RPC_TYPE = 3          ///< Remote procedure call type: server <-> client
    };
    
    //-------------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------------
    
    /**
     *  @brief  WsServiceBase class
     *          Base class for the different service types.
     *          Used internally by the websocket server
     */
    class WsServiceBase {
      WsServiceBase(const WsServiceBase &) = delete;
      WsServiceBase &operator=(const WsServiceBase&) = delete;
    public:
      /**
       *  @brief  Constructor
       *
       *  @param  s the websocket server owning the service
       *  @param  n the service name
       *  @param  t the service type
       */
      WsServiceBase(WsServer *s, const std::string &n, ServiceType t);
      
      /**
       *  @brief  Destructor
       */
      virtual ~WsServiceBase();
        
      /**
       *  @brief  Get the server owning the service
       */
      WsServer* server() const;      
      
      /**
       *  @brief  Get the service name
       */
      const std::string &name() const;
      
      /**
       *  @brief  Get the service type
       */
      ServiceType type() const;
        
    private:
      /// The server owning the service
      WsServer*              m_server = {nullptr};
      /// The service name
      std::string            m_name = {""};
      /// The service type
      ServiceType            m_type = {UNKNOWN_TYPE};
    };
    
    typedef std::map<std::string, WsServiceBase*> ServiceMap;
    
    //-------------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------------
    
    /**
     *  @brief  WsService class
     *          Implementation of a server service.
     *          A service send data to client whenever the server needs it.
     */
    class WsService : public WsServiceBase {
    public:
      /**
       *  @brief  Constructor
       *
       *  @param  s the server owning the service
       *  @param  n the service name
       */
      WsService(WsServer *s, const std::string &n);
      
      /**
       *  @brief  Send data to all listening clients.
       *  
       *  The data is converted to string using the
       *  dqm4hep::core::typeToString() function.
       *  std::string data type is not converted .
       *  Note that the final converted string must not
       *  contain binary characters. For binary data, use
       *  the dedicated method with buffer instead.
       *
       *  @param  value the value to send
       */
      template <typename T>
      void send(const T &value);
      
      /**
       *  @brief  Send data to all listening clients
       *
       *  @param  buffer a buffer of data to send
       *  @param  size the buffer size
       *  @param  containsBinary whether the buffer contains binary data
       */
      void send(const char *buffer, size_t size, bool containsBinary = false);
    };
    
    //-------------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------------
    
    /**
     *  @brief  WsCommandHandler class
     *
     *  Implement a command handler pattern where a client sends data
     *  to the server that receives it. No response is sent back to the client.
     *  To receive the data from the client you can use the signa provided by 
     *  the onCommand() method:
     *
     *  @code{.cpp}
     *  auto handler = server->createCommandHandler();
     *  handler->onCommand().connect(&object, &Object::method);
     *  @endcode
     */
    class WsCommandHandler : public WsServiceBase {
    public:
      typedef core::Signal<const WsConnection&, const WsMessage&> signal_type;
      
      /**
       *  @brief  Constructor
       *
       *  @param  s the server owning the command handler
       *  @param  n the command name
       */
      WsCommandHandler(WsServer *s, const std::string &n);
      
      /**
       *  @brief  Get the signal on command received
       */
      signal_type &onCommand();
      
    private:
      /// The signal on command received
      signal_type            m_signal = {};
    };
    
    //-------------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------------
    
    /**
     *  @brief  WsRequestHandler class
     *
     *  Implement a request pattern. A client sends data to the server
     *  and the server sends back a response with data in its turn.
     */
    class WsRequestHandler : public WsServiceBase {
    public:
      typedef core::Signal<const WsConnection&, const WsMessage&, WsMessage&> signal_type;
      
      /**
       *  @brief  Constructor
       *
       *  @param  s th server owning the request handler
       *  @param  n the request name
       */
      WsRequestHandler(WsServer *s, const std::string &n);
      
      /**
       *  @brief  Get the signal on request received
       */
      signal_type &onRequest();
      
    private:
      /// The signal on request received
      signal_type            m_signal = {};
    };
    
    //-------------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------------

    /**
     *  @brief  WsServer class
     *
     *  Implements a websocket server with which users can:
     *  - create services to broadcast data to all clients
     *  - receive commands from clients
     *  - receive requests from clients
     */
    class WsServer {
      WsServer(const WsServer&) = delete;
      WsServer& operator=(const WsServer&) = delete;
      friend class WsService;
      
    public:
      /**
       *  @brief  Constructor
       */
      WsServer();
      
      /**
       *  @brief  Destructor
       */
      ~WsServer();
      
      /**
       *  @brief  Set the server on which to listen
       *          Can be done only before calling start()
       *
       *  @param  port the server port
       */
      void setPort(int port);
      
      /**
       *  @brief  Create a new service.
       *          If the service already exists, a nullptr is returned.
       *          Use findService() to get an already existsing service.
       *          Services can also be created after server start.
       *
       *  @param  name the service name
       */
      WsService *createService(const std::string &name);
      
      /**
       *  @brief  Create a command handler.
       *          If the command already exists, a nullptr is returned.
       *          Use findCommandHandler() to get an already existsing command handler.
       *          Command handlers can also be created after server start.
       *
       *  @param  name the command name
       */
      WsCommandHandler *createCommandHandler(const std::string &name);
      
      /**
       *  @brief  Create a request handler.
       *          If the request handler already exists, a nullptr is returned.
       *          Use findRequestHandler() to get an already existsing request handler.
       *          Request handlers can also be created after server start.
       *
       *  @param  name the request handler name
       */      
      WsRequestHandler *createRequestHandler(const std::string &name);
      
      /**
       *  @brief  Find an existing service
       *
       *  @param  name the service name
       */
      WsService *findService(const std::string &name);
      
      /**
       *  @brief  Find an existing command handler
       *
       *  @param  name the command handler name
       */
      WsCommandHandler *findCommandHandler(const std::string &name);
      
      /**
       *  @brief  Find an exisiting request handler
       *
       *  @param  name the request handler name
       */
      WsRequestHandler *findRequestHandler(const std::string &name);
      
      /**
       *  @brief  Start the server. 
       *          Start listening to client connections.
       *          Note that the server is started in a separate thread.
       *          To synchronize an operation with the server to avoid
       *          data race condition, use the synchronize() method.
       */
      void start();
      
      /**
       *  @brief  Stop the server.
       *          The server thread is stopped and client connections
       *          closed. The list of services, command and request handlers
       *          are NOT cleared and can be re-used. The server can be re-started
       *          using the start() method again.
       */
      void stop();
      
      /**
       *  @brief  Synchronize a user operation with the server thread.
       *  
       *  This can be used for updating shared data between the user
       *  code and the request and command handlers. Typical use is by
       *  using a lambda function.
       *    
       *  Example:
       *  @code{.cpp}
       *  server->synchronize([&](){
       *    shared_data = update();
       *  });
       *  @endcode
       *
       *  The performed operation has to be fast in order to avoid 
       *  long server operation blocking.
       */
      template <typename Operation>
      void synchronize(Operation operation);
      
    private:
      void onOpen(connection_hdl hdl);
      void onClose(connection_hdl hdl);
      void onMessage(connection_hdl hdl, message_ptr msg);
      void send(WsService *service, const char *buffer, size_t size, bool containsBinary);
      void onServiceMessage(const std::string &serviceName, connection_hdl hdl, message_ptr msg);
      void onCommandMessage(const std::string &serviceName, connection_hdl hdl, message_ptr msg);
      void onRpcMessage(const std::string &serviceName, connection_hdl hdl, message_ptr msg);

    private:
      /// The list of service connections
      connection_map             m_serviceConnections = {};
      /// The real server implementation
      server                     m_server;
      /// The server port on which to listen
      int                        m_port = {5555};
      /// The server thread in which it runs
      std::thread                m_thread = {};
      /// The map of all services (services, command and request handlers)
      ServiceMap                 m_serviceMap = {};
      /// Whether the server is running
      std::atomic_bool           m_running = {false};
      /// The mutex to synchronize operations
      std::recursive_mutex       m_mutex = {};
    };
    
    //-------------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------------
    
    template <typename T>
    inline void WsService::send(const T &value) {
      std::string valueStr = dqm4hep::core::typeToString(value);
      send(valueStr.c_str(), valueStr.size(), false);
    }
    
    //-------------------------------------------------------------------------------------------------
    
    template <>
    inline void WsService::send(const std::string &value) {
      send(value.c_str(), value.size(), false);
    }
    
    //-------------------------------------------------------------------------------------------------

    template <typename Operation>
    inline void WsServer::synchronize(Operation operation) {
      std::lock_guard<std::recursive_mutex> lock(m_mutex);
      operation();
    }

  }
  
}

#endif //  DQM4HEP_WEBSOCKETSERVER_H
