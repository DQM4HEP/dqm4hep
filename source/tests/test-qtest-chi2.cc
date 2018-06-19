/// \file test-qtest-exact-ref-comp.cc
/*
 *
 * test-qtest-exact-ref-comp.cc main source file template automatically generated
 * Creation date : mer. nov. 5 2014
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

// -- dqm4hep headers
#include <dqm4hep/Internal.h>
#include <dqm4hep/Logging.h>
#include <dqm4hep/MonitorElementManager.h>
#include <dqm4hep/StatusCodes.h>
#include <dqm4hep/PluginManager.h>

#include <TH1.h>
#include <TGraph.h>
#include <TGraphErrors.h>

// -- std headers
#include <iostream>
#include <signal.h>

using namespace std;
using namespace dqm4hep::core;

#define assert_test(Command)                                                                                           \
  if (!(Command)) {                                                                                                    \
    dqm_error("Assertion failed : {0}, line {1}", #Command, __LINE__);                                                 \
    exit(1);                                                                                                           \
  }
  
void fillTest(TH1 *histogram) {
  histogram->Fill(1);
  histogram->Fill(50);
  histogram->Fill(50);
  histogram->Fill(53);
  histogram->Fill(42);
  histogram->Fill(-2); // underflow
  histogram->Fill(-6); // underflow
  histogram->Fill(102);// overflow
  histogram->Fill(105);// overflow
  histogram->Fill(106);// overflow
}

int main(int /*argc*/, char ** /*argv*/) {
  
  DQM4HEP_NO_EXCEPTION( Logger::createLogger("test-qtest-chi2", {Logger::coloredConsole()}); );
  Logger::setMainLogger("test-qtest-chi2");
  Logger::setLogLevel(spdlog::level::debug);
  
  std::unique_ptr<MonitorElementManager> meMgr = std::unique_ptr<MonitorElementManager>(new MonitorElementManager());
  
  // create test element TH1
  MonitorElementPtr testElement;
  meMgr->bookHisto<TH1F>("/", "TestHisto", "A test histogram", testElement, 100, 0.f, 99.f);
  TH1F *histogram = testElement->objectTo<TH1F>();
  assert_test(nullptr != histogram);
  fillTest(histogram);

  // create reference TH1
  PtrHandler<TObject> reference(new TH1F("", "A good reference", 100, 0.f, 99.f), true);
  fillTest((TH1F*)reference.ptr());

  // configure reporting
  QReportStorage storage; QReport report; json jsonReport;
  
  // creating the qtest XML config
  const std::string qtestType("Chi2Test");
  const std::string qtestName("UnitTestChi2");
  TiXmlElement *qtestElement = new TiXmlElement("qtest");
  std::shared_ptr<TiXmlElement> sharedQtest(qtestElement);
  qtestElement->SetAttribute("type", qtestType);
  qtestElement->SetAttribute("name", qtestName);

  // configuring the qtest
  assert_test(STATUS_CODE_SUCCESS == meMgr->createQualityTest(sharedQtest.get()));
  assert_test(STATUS_CODE_SUCCESS == meMgr->addQualityTest(testElement->path(), testElement->name(), qtestName));

  // invalid test - no reference
  assert_test(STATUS_CODE_SUCCESS == meMgr->runQualityTest(testElement->path(), testElement->name(), qtestName, storage));
  assert_test(STATUS_CODE_SUCCESS == storage.report(testElement->path(), testElement->name(), qtestName, report));
  report.toJson(jsonReport);
  DQM4HEP_NO_EXCEPTION( std::cout << jsonReport.dump(2) << std::endl; );
  assert_test(report.m_qualityFlag == INVALID);

  // valid test - same histogram
  storage.clear();
  testElement->setReferenceObject(reference);
  assert_test(STATUS_CODE_SUCCESS == meMgr->runQualityTest(testElement->path(), testElement->name(), qtestName, storage));
  assert_test(STATUS_CODE_SUCCESS == storage.report(testElement->path(), testElement->name(), qtestName, report));
  report.toJson(jsonReport);
  DQM4HEP_NO_EXCEPTION( std::cout << jsonReport.dump(2) << std::endl; );
  assert_test(report.m_qualityFlag == SUCCESS);

}
