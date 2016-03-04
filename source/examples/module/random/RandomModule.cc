  /// \file TestModule.cc
/*
 *
 * TestModule.cc source template automatically generated by a class generator
 * Creation date : sam. nov. 1 2014
 *
 * This file is part of LCDQM libraries.
 * 
 * LCDQM is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * based upon these libraries are permitted. Any copy of these libraries
 * must include this copyright notice.
 * 
 * LCDQM is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with LCDQM.  If not, see <http://www.gnu.org/licenses/>.
 * 
 * @author Remi Ete
 * @copyright CNRS , IPNL
 */


#include "random/RandomModule.h"

// -- std headers
#include <ctime>
#include <cstdlib>
#include <limits>

// -- dqm4hep headers
#include "dqm4hep/DQMMonitorElement.h"
#include "dqm4hep/DQMCoreTool.h"
#include "dqm4hep/DQMRun.h"
#include "dqm4hep/DQMXmlHelper.h"
#include "dqm4hep/DQMQualityTest.h"
#include "dqm4hep/DQMModuleApi.h"
#include "dqm4hep/DQMPlugin.h"

// -- root headers
#include "TRandom.h"

namespace dqm4hep
{

class RandomQualityTest : public DQMQualityTest
{
public:
	class Factory : public DQMQualityTestFactory
	{
	public:
		DQMQualityTest *createQualityTest(const std::string &name) const
		{
			return new RandomQualityTest(name);
		}
	};

	RandomQualityTest(const std::string &name) :
		DQMQualityTest(name)
	{
		/* nop */
	}

	StatusCode readSettings(const TiXmlHandle)
	{
		/* nop */
		return STATUS_CODE_SUCCESS;
	}

	StatusCode init()
	{
		/* nop */
		return STATUS_CODE_SUCCESS;
	}

	/** Runs a quality test on the given monitor element
	 */
	StatusCode run(DQMMonitorElement *pMonitorElement)
	{
		m_isSuccessful = true;

		m_message = "Random quality test performed on '" + pMonitorElement->getName() +
				"'. The test is a random test that evaluate nothing and just assign a random quality !";

		unsigned int randReentrant = time(0);
		m_quality = DQM4HEP::scaleToQuality(rand_r(&randReentrant)/float(RAND_MAX));

		return STATUS_CODE_SUCCESS;
	}

	bool canRun(DQMMonitorElement *pMonitorElement) const
	{
		return true;
	}
};

// module plugin declaration
DQM_PLUGIN_DECL( RandomModule ,  "RandomModule" )

//-------------------------------------------------------------------------------------------------

RandomModule::RandomModule()
 : DQMAnalysisModule()
{
	setDetectorName("random module detector");
	setVersion(1, 0, 0);

#ifdef DQM4HEP_USE_ELOG
	m_eLog.setHost("localhost");
	m_eLog.setPort(8080);
	m_eLog.setLogbook("DQM SDHCAL");
	m_eLog.setUser("elog");
	m_eLog.setPassword("elog");
#endif
}

//-------------------------------------------------------------------------------------------------

RandomModule::~RandomModule()
{
}

//-------------------------------------------------------------------------------------------------

StatusCode RandomModule::readSettings(const TiXmlHandle xmlHandle)
{
	RETURN_RESULT_IF_AND_IF(STATUS_CODE_SUCCESS, STATUS_CODE_NOT_FOUND, !=, DQMModuleApi::registerQualityTestFactory(this,
			"RandomQualityTest", new RandomQualityTest::Factory()));

	LOG4CXX_INFO( dqmMainLogger , "Module : " << getName() << " -- readSettings()" );

	m_histoXMin = -400;
	RETURN_RESULT_IF_AND_IF(STATUS_CODE_SUCCESS, STATUS_CODE_NOT_FOUND, !=, DQMXmlHelper::readParameterValue(xmlHandle,
			"HistoXMin", m_histoXMin));

	m_histoXMax = 400;
	RETURN_RESULT_IF_AND_IF(STATUS_CODE_SUCCESS, STATUS_CODE_NOT_FOUND, !=, DQMXmlHelper::readParameterValue(xmlHandle,
			"HistoXMax", m_histoXMax));

	// max must be greater than min ...
	if(m_histoXMin >= m_histoXMax)
		return STATUS_CODE_INVALID_PARAMETER;

	RETURN_RESULT_IF_AND_IF(STATUS_CODE_SUCCESS, STATUS_CODE_NOT_FOUND, !=, DQMXmlHelper::createQualityTest(this, xmlHandle,
			"RandomQTest1"));

	RETURN_RESULT_IF_AND_IF(STATUS_CODE_SUCCESS, STATUS_CODE_NOT_FOUND, !=, DQMXmlHelper::createQualityTest(this, xmlHandle,
			"RandomQTest2"));

	RETURN_RESULT_IF_AND_IF(STATUS_CODE_SUCCESS, STATUS_CODE_NOT_FOUND, !=, DQMXmlHelper::createQualityTest(this, xmlHandle,
			"RandomQTest3"));

	return STATUS_CODE_SUCCESS;
}

//-------------------------------------------------------------------------------------------------

StatusCode RandomModule::initModule()
{
	LOG4CXX_INFO( dqmMainLogger , "Module : " << getName() << " -- init()" );

	srand(time(NULL));

	short shortMinLimit = -100;
	short shortMaxLimit = 100;

	RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, DQMModuleApi::mkdir(this, "/Histograms"));
	RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, DQMModuleApi::mkdir(this, "/Histograms/1D"));
	RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, DQMModuleApi::mkdir(this, "/Histograms/2D"));
	RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, DQMModuleApi::mkdir(this, "/Scalars"));

	// booking of 1D histogram
	RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, DQMModuleApi::cd(this, "/Histograms/1D"));

	RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, DQMModuleApi::bookRealHistogram1D(this,
			m_pRealHistogram1DElement, "RealHisto1D", "A real histogram in 1D", 100, -10, 10));
	m_pRealHistogram1DElement->setDescription(
			"This is rather long description of this real histogram 1D element (TH1F) that is filled "
			"using a random gaussian function");
	m_pRealHistogram1DElement->setResetPolicy(END_OF_CYCLE_RESET_POLICY);

	RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, DQMModuleApi::bookIntHistogram1D(this,
			m_pIntHistogram1DElement, "IntHisto1D", "A int histogram in 1D", 100, m_histoXMin, m_histoXMax));
	m_pIntHistogram1DElement->setDescription(
			"This is rather long description of this int histogram 1D element (TH1I) that is filled "
			"using a random gaussian function");
	m_pIntHistogram1DElement->setResetPolicy(END_OF_RUN_RESET_POLICY);

	RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, DQMModuleApi::bookCharHistogram1D(this,
			m_pCharHistogram1DElement, "CharHisto1D", "A char histogram in 1D", 26, 96, 123));
	m_pCharHistogram1DElement->setDescription(
			"This is rather long description of this char histogram 1D element (TH1C) that is filled "
			"using random char values picked in the ASCII table");
	m_pCharHistogram1DElement->setResetPolicy(END_OF_CYCLE_RESET_POLICY);

	RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, DQMModuleApi::bookShortHistogram1D(this,
			m_pShortHistogram1DElement, "ShortHisto1D", "A short histogram in 1D", 100, shortMinLimit, shortMaxLimit));
	m_pShortHistogram1DElement->setDescription(
			"This is rather long description of this short histogram 1D element (TH1S) that is filled "
			"using random gaussian function. The limits of the histogram may have been "
			"shrinked to fit a short numeric limits");
	m_pShortHistogram1DElement->setResetPolicy(END_OF_RUN_RESET_POLICY);

	// booking of 2D histograms
	RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, DQMModuleApi::cd(this, "/Histograms/2D"));

	RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, DQMModuleApi::bookRealHistogram2D(this,
			m_pRealHistogram2DElement, "RealHisto2D", "A real histogram in 2D", 100, m_histoXMin, m_histoXMax, 100, m_histoXMin, m_histoXMax));
	m_pRealHistogram2DElement->setDescription(
			"This is rather long description of this float histogram 2D element (TH2F) that is filled "
			"using a random gaussian function with 100 points");
	m_pRealHistogram2DElement->setResetPolicy(END_OF_CYCLE_RESET_POLICY);

	RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, DQMModuleApi::bookIntHistogram2D(this,
			m_pIntHistogram2DElement, "IntHisto2D", "A int histogram in 2D", 100, m_histoXMin, m_histoXMax, 100, m_histoXMin, m_histoXMax));
	m_pIntHistogram2DElement->setDescription(
			"This is rather long description of this int histogram 2D element (TH2I) that is filled "
			"using a random gaussian function with 100 points");
	m_pRealHistogram2DElement->setResetPolicy(END_OF_RUN_RESET_POLICY);

	RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, DQMModuleApi::bookShortHistogram2D(this,
			m_pShortHistogram2DElement, "ShortHisto2D", "A short histogram in 2D", 100, shortMinLimit, shortMaxLimit, 100, shortMinLimit, shortMaxLimit));
	m_pShortHistogram2DElement->setDescription(
			"This is rather long description of this short histogram 2D element (TH2S) that is filled "
			"using random gaussian function with 100 points. The limits of the histogram may have been "
			"shrinked to fit a short numeric limits");
	m_pShortHistogram2DElement->setResetPolicy(END_OF_RUN_RESET_POLICY);
	m_pShortHistogram2DElement->setDrawOption("colz");

	// booking of scalars
	RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, DQMModuleApi::cd(this, "/Scalars"));

	RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, DQMModuleApi::bookInt(this,
			m_pIntElement, "IntValue", "A random int value", 42));
	m_pIntElement->setDescription(
			"This is rather long description of this integer scalar value determined "
			"by a random in range [0,100]");
	m_pIntElement->setResetPolicy(END_OF_CYCLE_RESET_POLICY);

	RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, DQMModuleApi::bookFloat(this,
			m_pFloatElement, "FloatValue", "A random float value", 145.1f));
	m_pFloatElement->setDescription(
			"This is rather long description of this float scalar value determined "
			"by a random in range [-42,42]");
	m_pFloatElement->setResetPolicy(END_OF_CYCLE_RESET_POLICY);

	RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, DQMModuleApi::bookShort(this,
			m_pShortElement, "ShortValue", "A random short value", static_cast<short>(10)));
	m_pShortElement->setDescription(
			"This is rather long description of this short scalar value determined "
			"by a random in range [-10,10]");
	m_pShortElement->setResetPolicy(END_OF_CYCLE_RESET_POLICY);

	RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, DQMModuleApi::bookString(this,
			m_pStringElement, "StringValue", "A ramdom string", "random value !!"));
	m_pStringElement->setDescription(
			"This is rather long description of this string value determined "
			"by a random choice between [baba, bebe, bibi, bobo, bubu, byby] which "
			"is a rather stupid monitored element with no merit ...");
	m_pStringElement->setResetPolicy(END_OF_CYCLE_RESET_POLICY);

	RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, DQMModuleApi::addQualityTest(this,
			m_pRealHistogram1DElement, "RandomQTest1"));

	RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, DQMModuleApi::addQualityTest(this,
			m_pRealHistogram1DElement, "RandomQTest2"));

	RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, DQMModuleApi::addQualityTest(this,
			m_pRealHistogram1DElement, "RandomQTest3"));

	DQMModuleApi::cd(this);

	// print directory structure of monitor element storage in console
	DQMModuleApi::ls(this, true);

	return STATUS_CODE_SUCCESS;
}

//-------------------------------------------------------------------------------------------------

StatusCode RandomModule::endModule()
{
	LOG4CXX_INFO( dqmMainLogger , "Module : " << getName() << " -- end()" );
	return STATUS_CODE_SUCCESS;
}

//-------------------------------------------------------------------------------------------------

StatusCode RandomModule::processEvent(DQMEvent */*pEvent*/)
{
//	EVENT::LCEvent *pLCEvent = pEvent->getEvent<EVENT::LCEvent>();

	TRandom r(time(0));

	m_pRealHistogram1DElement->get<TH1F>()->FillRandom("gaus", 1000);
	m_pIntHistogram1DElement->get<TH1I>()->FillRandom("gaus", 100);

	unsigned int randReentrant = time(0);
	char c = round( rand_r(&randReentrant)/static_cast<float>(RAND_MAX)*26 + 97);
	m_pCharHistogram1DElement->get<TH1C>()->Fill(c);

	short shortMin = m_pShortHistogram1DElement->get<TH1S>()->GetXaxis()->GetXmin();
	short shortMax = m_pShortHistogram1DElement->get<TH1S>()->GetXaxis()->GetXmax();
	m_pShortHistogram1DElement->get<TH1S>()->Fill( r.Gaus((shortMin-shortMax)/2.f, (shortMin-shortMax)/20.f) );

	return STATUS_CODE_SUCCESS;
}

//-------------------------------------------------------------------------------------------------

StatusCode RandomModule::startOfCycle()
{
	LOG4CXX_INFO( dqmMainLogger , "Module : " << getName() << " -- startOfCycle()" );
	return STATUS_CODE_SUCCESS;
}

//-------------------------------------------------------------------------------------------------

StatusCode RandomModule::endOfCycle()
{
	LOG4CXX_INFO( dqmMainLogger , "Module : " << getName() << " -- endOfCycle()" );

	std::vector<std::string> randomStringValues;

	randomStringValues.push_back("baba");
	randomStringValues.push_back("bebe");
	randomStringValues.push_back("bibi");
	randomStringValues.push_back("bobo");
	randomStringValues.push_back("bubu");
	randomStringValues.push_back("byby");

	unsigned int randReentrant = time(0);
	size_t i = round(rand_r(&randReentrant)/static_cast<float>(RAND_MAX)*5);

	// pEvent is not used here since it's a random module, no need data ...
	int randomInt = round(rand_r(&randReentrant)/static_cast<float>(RAND_MAX)*100);
	m_pIntElement->get<TScalarInt>()->Set(randomInt);

	float randomFloat = round( rand_r(&randReentrant)/static_cast<float>(RAND_MAX)*84 - 42);
	m_pFloatElement->get<TScalarFloat>()->Set(randomFloat);

	short randomShort = static_cast<short>(round( rand_r(&randReentrant)/static_cast<float>(RAND_MAX)*20 - 10));
	m_pShortElement->get<TScalarShort>()->Set(randomShort);

	std::string randomString = randomStringValues.at(i);
	m_pStringElement->get<TScalarString>()->Set(randomString);

	m_pIntHistogram2DElement->get<TH2I>()->FillRandom("gaus", 1000);
	m_pRealHistogram2DElement->get<TH2F>()->FillRandom("gaus", 1000);
	m_pShortHistogram2DElement->get<TH2S>()->FillRandom("gaus", 1000);

	// run all quality tests on all monitor elements
	RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, DQMModuleApi::runQualityTests(this));

	return STATUS_CODE_SUCCESS;
}

//-------------------------------------------------------------------------------------------------

StatusCode RandomModule::startOfRun(DQMRun *pRun)
{
	LOG4CXX_INFO( dqmMainLogger , "Module : " << getName() << " -- startOfRun()" );
	LOG4CXX_INFO( dqmMainLogger , "Run no " << pRun->getRunNumber() );
	time_t startTime = pRun->getStartTime();

	std::string timeStr;
	DQMCoreTool::timeToHMS(startTime, timeStr);
	LOG4CXX_INFO( dqmMainLogger , "Start time " << timeStr );
	LOG4CXX_INFO( dqmMainLogger , "Detector is " << pRun->getDetectorName() );
	LOG4CXX_INFO( dqmMainLogger , "Description " << pRun->getDescription() );

#ifdef DQM4HEP_USE_ELOG
	m_eLog.addEntry("Module", getName());
	m_eLog.addEntry("Level", "DEBUG");
	m_eLog.addEntry("Module host", "localhost");
	m_eLog.setMessage("Random module run no " + DQM4HEP::typeToString(pRun->getRunNumber()) + "started !");
	RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, m_eLog.log());
#endif

	return STATUS_CODE_SUCCESS;
}

//-------------------------------------------------------------------------------------------------

StatusCode RandomModule::endOfRun(DQMRun *pRun)
{
	LOG4CXX_INFO( dqmMainLogger , "Module : " << getName() << " -- endOfRun()" );
	LOG4CXX_INFO( dqmMainLogger , "Run no " << pRun->getRunNumber() );

	time_t endTime = pRun->getEndTime();
	std::string timeStr;
	DQMCoreTool::timeToHMS(endTime, timeStr);

	LOG4CXX_INFO( dqmMainLogger , "End time " << timeStr );

	return STATUS_CODE_SUCCESS;
}

} 

