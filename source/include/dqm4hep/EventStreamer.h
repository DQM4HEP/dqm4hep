/// \file DQMEventStreamer.h
/*
 *
 * DQMEventStreamer.h header template automatically generated by a class generator
 * Creation date : mar. d�c. 15 2015
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


#ifndef DQM4HEP_EVENTSTREAMER_H
#define DQM4HEP_EVENTSTREAMER_H

// -- dqm4hep headers
#include <dqm4hep/StatusCodes.h>

namespace xdrstream {
  class IODevice;
}

namespace dqm4hep {

  namespace core {

    class Event;

    /** EventStreamer class
     */
    class EventStreamer
    {
    public:
      /** Destructor
       */
      virtual ~EventStreamer() {}

      /** Factory method to create the corresponding event to this streamer.
       *  The event is expected to contains an allocated wrapped event
       */
      virtual Event *createEvent() const = 0;

      /** Serialize a Event object and store it in the data stream
       */
      virtual StatusCode write(const Event *const pObject, xdrstream::IODevice *pDevice) = 0;

      /** De-serialize a Event given from the data stream
       */
      virtual StatusCode read(Event *&pObject, xdrstream::IODevice *pDevice) = 0;

      /** Serialize a part of a Event object identified by the reg exp 'subEventIdentifier' and store it in the data stream
       *
       *  Example : subEventIdentifier = "MyCollection1:MyCollection2"
       *            subEventIdentifier = "*TpcCollection"
       *
       *  The identifier decoding has to be performed by the user, based on the event contents itself
       */
      virtual StatusCode write(const Event *const pObject, const std::string &subEventIdentifier, xdrstream::IODevice *pDevice);
    };

    //-------------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------------

    StatusCode EventStreamer::write(const Event *const pObject, const std::string &subEventIdentifier, xdrstream::IODevice *pDevice)
    {
      return this->write(pObject, pDevice);
    }

  }

}

#endif  //  DQM4HEP_EVENTSTREAMER_H
