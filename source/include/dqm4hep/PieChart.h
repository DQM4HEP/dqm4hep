  /// \file PieChart.h
/*
 *
 * PieChart.h header template automatically generated by a class generator
 * Creation date : jeu. f�vr. 2 2017
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


#ifndef PIECHART_H
#define PIECHART_H

// -- dqm4hep headers
#include "dqm4hep/DQM4HEP.h"
#include "dqm4hep/MonitorObject.h"

// -- std headers
#include <bitset>

namespace dqm4hep {

  namespace core {

    /**
     * PieChart class
     */
    class PieChart final : public MonitorObject
    {
    public:
     /**
      * Constructor
      */
     PieChart();

     /**
      * Destructor
      */
     ~PieChart();

     /**
      * [create description]
      * @param  value [description]
      * @return       [description]
      */
     static PieChart *create(const Json::Value &value);

     /**
      * [setTitle description]
      * @param title [description]
      */
     void setTitle(const std::string &title);

     /**
      * [getTitle description]
      * @return [description]
      */
     const std::string &getTitle() const;

     /**
      * [setDrawLegend description]
      * @param draw [description]
      */
     void setDrawLegend(bool draw = true);

     /**
      * [shouldDrawLegend description]
      * @return [description]
      */
     bool shouldDrawLegend() const;

     /**
      * [addEntry description]
      * @param name  [description]
      * @param color [description]
      * @param value [description]
      */
     void addEntry(const std::string &name, Color color, const float &value);

     /**
      * [setEntryColor description]
      * @param name  [description]
      * @param color [description]
      */
     void setEntryColor(const std::string &name, Color color);

     /**
      * [setEntryValue description]
      * @param name  [description]
      * @param value [description]
      */
     void setEntryValue(const std::string &name, const float &value);

     /**
      * [removeEntry description]
      * @param name [description]
      */
     void removeEntry(const std::string &name);

     /**
      * [clear description]
      */
     void clear();

     /**
      * [normalize description]
      */
     void normalize();

   private:
     void fromJson(const Json::Value &value);
     void toJson(Json::Value &value, bool full = true);
     void resetCache();

      /**
       * EntryMetadata struct
       */
      struct EntryMetadata
      {
        Color                  m_color;
        float                  m_value;
        float                  m_percentage;
      };

      enum Cache
      {
        TITLE = 0,
        DRAW_LEGEND = 1,
        ENTRIES = 2
      };

      typedef std::map<std::string, EntryMetadata>   EntryMap;

      std::bitset<3>                                 m_updateCache;
      std::string                                    m_title;        ///< The pie chart title
      EntryMap                                       m_entries;      ///< The pie chart entries
      bool                                           m_drawLegend;   ///< Whether to draw the legend
    };

  }

}

#endif  //  PIECHART_H
