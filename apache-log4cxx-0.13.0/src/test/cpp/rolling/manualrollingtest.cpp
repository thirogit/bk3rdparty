/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "../util/compare.h"
#include "../insertwide.h"
#include "../logunit.h"
#include <apr_time.h>
#include <log4cxx/logmanager.h>
#include <log4cxx/xml/domconfigurator.h>
#include <log4cxx/patternlayout.h>
#include <log4cxx/rolling/fixedwindowrollingpolicy.h>
#include <log4cxx/rolling/sizebasedtriggeringpolicy.h>
#include <log4cxx/filter/levelrangefilter.h>
#include <log4cxx/helpers/pool.h>
#include <log4cxx/logger.h>
#include <log4cxx/propertyconfigurator.h>
#include <log4cxx/rolling/rollingfileappender.h>
#include <log4cxx/helpers/stringhelper.h>
#include <log4cxx/consoleappender.h>
#include <log4cxx/helpers/exception.h>
#include <log4cxx/helpers/fileoutputstream.h>
#include <random>

using namespace log4cxx;
using namespace log4cxx::xml;
using namespace log4cxx::filter;
using namespace log4cxx::helpers;
using namespace log4cxx::rolling;

/**
 *   Tests of explicit manual rolling of RollingFileAppenders.
 *
 *
 *
 */
LOGUNIT_CLASS(ManualRollingTest)
{
	LOGUNIT_TEST_SUITE(ManualRollingTest);
	LOGUNIT_TEST(test1);
	LOGUNIT_TEST(test2);
	//           TODO: Compression not yet implemented
	//           LOGUNIT_TEST(test3);
	LOGUNIT_TEST(test4);
	LOGUNIT_TEST(test5);
	LOGUNIT_TEST(create_directories);
	LOGUNIT_TEST_SUITE_END();

	LoggerPtr root;
	LoggerPtr logger;

public:
	void setUp()
	{
		logger = Logger::getLogger("org.apache.log4j.rolling.ManualRollingTest");
		root = Logger::getRootLogger();
	}

	void tearDown()
	{
		LogManager::shutdown();
	}

	void common(RollingFileAppenderPtr & rfa,
		Pool & pool,
		LoggerPtr & logger1)
	{
		char msg[] = { 'H', 'e', 'l', 'l', 'o', '-', '-', '-', 'N', 0 };

		// Write exactly 10 bytes with each log
		for (int i = 0; i < 25; i++)
		{
			if (i < 10)
			{
				msg[8] = '0' + i;
			}
			else if (i < 100)
			{
				int digit = i % 10;

				if (digit == 0)
				{
					rfa->rollover(pool);
				}

				msg[7] = '0' + i / 10;
				msg[8] = '0' + digit;
			}

			LOG4CXX_DEBUG(logger1, msg);
		}
	}


	/**
	 * Tests that the lack of an explicit active file will use the
	 * low index as the active file.
	 *
	 */
	void test1()
	{
		PatternLayoutPtr layout = PatternLayoutPtr(new PatternLayout(LOG4CXX_STR("%m\n")));
		RollingFileAppenderPtr rfa = RollingFileAppenderPtr(new RollingFileAppender());
		rfa->setName(LOG4CXX_STR("ROLLING"));
		rfa->setAppend(false);
		rfa->setLayout(layout);

		FixedWindowRollingPolicyPtr swrp = FixedWindowRollingPolicyPtr(new FixedWindowRollingPolicy());
		swrp->setMinIndex(0);

		swrp->setFileNamePattern(LOG4CXX_STR("output/manual-test1.%i"));
		Pool p;
		swrp->activateOptions(p);

		rfa->setRollingPolicy(swrp);
		rfa->activateOptions(p);
		root->addAppender(rfa);


		common(rfa, p, logger);

		LOGUNIT_ASSERT_EQUAL(true, File("output/manual-test1.0").exists(p));
		LOGUNIT_ASSERT_EQUAL(true, File("output/manual-test1.1").exists(p));
		LOGUNIT_ASSERT_EQUAL(true, File("output/manual-test1.2").exists(p));
		LOGUNIT_ASSERT_EQUAL(true, Compare::compare(File("output/manual-test1.0"),
				File("witness/rolling/sbr-test2.log")));
		LOGUNIT_ASSERT_EQUAL(true, Compare::compare(File("output/manual-test1.1"),
				File("witness/rolling/sbr-test2.0")));
		LOGUNIT_ASSERT_EQUAL(true, Compare::compare(File("output/manual-test1.2"),
				File("witness/rolling/sbr-test2.1")));
	}

	/**
	 * Test basic rolling functionality with explicit setting of FileAppender.file.
	 */
	void test2()
	{
		PatternLayoutPtr layout = PatternLayoutPtr(new PatternLayout(LOG4CXX_STR("%m\n")));
		RollingFileAppenderPtr rfa = RollingFileAppenderPtr(new RollingFileAppender());
		rfa->setName(LOG4CXX_STR("ROLLING"));
		rfa->setAppend(false);
		rfa->setLayout(layout);
		rfa->setFile(LOG4CXX_STR("output/manual-test2.log"));

		Pool p;
		rfa->activateOptions(p);
		root->addAppender(rfa);

		common(rfa, p, logger);

		LOGUNIT_ASSERT_EQUAL(true, File("output/manual-test2.log").exists(p));
		LOGUNIT_ASSERT_EQUAL(true, File("output/manual-test2.log.1").exists(p));
		LOGUNIT_ASSERT_EQUAL(true, File("output/manual-test2.log.2").exists(p));

		LOGUNIT_ASSERT_EQUAL(true, Compare::compare(File("output/manual-test2.log"),
				File("witness/rolling/sbr-test2.log")));
		LOGUNIT_ASSERT_EQUAL(true, Compare::compare(File("output/manual-test2.log.1"),
				File("witness/rolling/sbr-test2.0")));
		LOGUNIT_ASSERT_EQUAL(true, Compare::compare(File("output/manual-test2.log.2"),
				File("witness/rolling/sbr-test2.1")));
	}

	/**
	 * Same as testBasic but also with GZ compression.
	 */
	void test3()
	{
		PatternLayoutPtr layout = PatternLayoutPtr(new PatternLayout(LOG4CXX_STR("%m\n")));
		RollingFileAppenderPtr rfa = RollingFileAppenderPtr(new RollingFileAppender());
		rfa->setAppend(false);
		rfa->setLayout(layout);

		FixedWindowRollingPolicyPtr  fwrp = FixedWindowRollingPolicyPtr(new FixedWindowRollingPolicy());

		fwrp->setMinIndex(0);
		rfa->setFile(LOG4CXX_STR("output/manual-test3.log"));
		fwrp->setFileNamePattern(LOG4CXX_STR("output/sbr-test3.%i.gz"));
		Pool p;
		fwrp->activateOptions(p);
		rfa->setRollingPolicy(fwrp);
		rfa->activateOptions(p);
		root->addAppender(rfa);

		common(rfa, p, logger);

		LOGUNIT_ASSERT_EQUAL(true, File("output/manual-test3.log").exists(p));
		LOGUNIT_ASSERT_EQUAL(true, File("output/manual-test3.0.gz").exists(p));
		LOGUNIT_ASSERT_EQUAL(true, File("output/manual-test3.1.gz").exists(p));

		LOGUNIT_ASSERT_EQUAL(true, Compare::compare(File("output/manual-test3.log"),  File("witness/rolling/sbr-test3.log")));
		LOGUNIT_ASSERT_EQUAL(File("witness/rolling/sbr-test3.0.gz").length(p), File("output/manual-test3.0.gz").length(p));
		LOGUNIT_ASSERT_EQUAL(File("witness/rolling/sbr-test3.1.gz").length(p), File("output/manual-test3.1.gz").length(p));
	}

	/**
	 * Test basic rolling functionality with bogus path in file name pattern.
	 */
	void test4()
	{
		PatternLayoutPtr layout = PatternLayoutPtr(new PatternLayout(LOG4CXX_STR("%m\n")));
		RollingFileAppenderPtr rfa = RollingFileAppenderPtr(new RollingFileAppender());
		rfa->setName(LOG4CXX_STR("ROLLING"));
		rfa->setAppend(false);
		rfa->setLayout(layout);
		rfa->setFile(LOG4CXX_STR("output/manual-test4.log"));

		FixedWindowRollingPolicyPtr swrp = FixedWindowRollingPolicyPtr(new FixedWindowRollingPolicy());
		swrp->setCreateIntermediateDirectories(false);
		swrp->setMinIndex(0);

		//
		//   test4 directory should not exists.  Should cause all rollover attempts to fail.
		//
		swrp->setFileNamePattern(LOG4CXX_STR("output/test4/manual-test4.%i"));
		Pool p;
		swrp->activateOptions(p);

		rfa->setRollingPolicy(swrp);
		rfa->activateOptions(p);
		root->addAppender(rfa);

		common(rfa, p, logger);

		LOGUNIT_ASSERT_EQUAL(true, File("output/manual-test4.log").exists(p));

		LOGUNIT_ASSERT_EQUAL(true, Compare::compare(File("output/manual-test4.log"),
				File("witness/rolling/sbr-test4.log")));
	}

	/**
	 * Checking handling of rename failures due to other access
	 * to the indexed files.
	 */
	void test5()
	{
		PatternLayoutPtr layout = PatternLayoutPtr(new PatternLayout(LOG4CXX_STR("%m\n")));
		RollingFileAppenderPtr rfa = RollingFileAppenderPtr(new RollingFileAppender());
		rfa->setName(LOG4CXX_STR("ROLLING"));
		rfa->setAppend(false);
		rfa->setLayout(layout);
		rfa->setFile(LOG4CXX_STR("output/manual-test5.log"));

		FixedWindowRollingPolicyPtr swrp = FixedWindowRollingPolicyPtr(new FixedWindowRollingPolicy());

		swrp->setMinIndex(0);

		swrp->setFileNamePattern(LOG4CXX_STR("output/manual-test5.%i"));
		Pool p;
		swrp->activateOptions(p);

		rfa->setRollingPolicy(swrp);
		rfa->activateOptions(p);
		root->addAppender(rfa);

		//
		//   put stray file about locked file
		FileOutputStream os1(LOG4CXX_STR("output/manual-test5.1"), false);
		os1.close(p);


		FileOutputStream os0(LOG4CXX_STR("output/manual-test5.0"), false);

		common(rfa, p, logger);

		os0.close(p);

		if (File("output/manual-test5.3").exists(p))
		{
			//
			//    looks like platform where open files can be renamed
			//
			LOGUNIT_ASSERT_EQUAL(true, File("output/manual-test5.log").exists(p));
			LOGUNIT_ASSERT_EQUAL(true, File("output/manual-test5.0").exists(p));
			LOGUNIT_ASSERT_EQUAL(true, File("output/manual-test5.1").exists(p));
			LOGUNIT_ASSERT_EQUAL(true, File("output/manual-test5.2").exists(p));
			LOGUNIT_ASSERT_EQUAL(true, File("output/manual-test5.3").exists(p));

			LOGUNIT_ASSERT_EQUAL(true, Compare::compare(File("output/manual-test5.log"),
					File("witness/rolling/sbr-test2.log")));
			LOGUNIT_ASSERT_EQUAL(true, Compare::compare(File("output/manual-test5.0"),
					File("witness/rolling/sbr-test2.0")));
			LOGUNIT_ASSERT_EQUAL(true, Compare::compare(File("output/manual-test5.1"),
					File("witness/rolling/sbr-test2.1")));

		}
		else
		{
			//
			//  rollover attempts should all fail
			//    so initial log file should have all log content
			//    open file should be unaffected
			//    stray file should have only been moved one slot.
			LOGUNIT_ASSERT_EQUAL(true, File("output/manual-test5.log").exists(p));
			LOGUNIT_ASSERT_EQUAL(true, File("output/manual-test5.0").exists(p));
			LOGUNIT_ASSERT_EQUAL(true, File("output/manual-test5.2").exists(p));

			LOGUNIT_ASSERT_EQUAL(true, Compare::compare(File("output/manual-test5.log"),
					File("witness/rolling/sbr-test4.log")));
		}
	}

	void create_directories()
	{
		PatternLayoutPtr layout = PatternLayoutPtr(new PatternLayout(LOG4CXX_STR("%m\n")));
		RollingFileAppenderPtr rfa = RollingFileAppenderPtr(new RollingFileAppender());
		rfa->setName(LOG4CXX_STR("ROLLING"));
		rfa->setAppend(false);
		rfa->setLayout(layout);
		rfa->setFile(LOG4CXX_STR("output/create-directory-file.log"));

		FixedWindowRollingPolicyPtr swrp = FixedWindowRollingPolicyPtr(new FixedWindowRollingPolicy());
		swrp->setMinIndex(0);

		std::random_device dev;
		std::mt19937 rng(dev());
		std::uniform_int_distribution<std::mt19937::result_type> dist(1,100000);
		LogString filenamePattern = LOG4CXX_STR("output/directory-");

#if LOG4CXX_LOGCHAR_IS_WCHAR
		LogString dirNumber = std::to_wstring(dist(rng));
#else
		LogString dirNumber = std::to_string(dist(rng));
#endif

		filenamePattern.append( dirNumber );
		LogString filenamePatternPrefix = filenamePattern;
		filenamePattern.append( LOG4CXX_STR("/file-%i.gz") );
		swrp->setFileNamePattern(filenamePattern);
		Pool p;
		swrp->activateOptions(p);

		rfa->setRollingPolicy(swrp);
		rfa->activateOptions(p);
		root->addAppender(rfa);


		common(rfa, p, logger);

		LOGUNIT_ASSERT_EQUAL(true, File(filenamePatternPrefix + LOG4CXX_STR("/file-0.gz")).exists(p));
		LOGUNIT_ASSERT_EQUAL(true, File(filenamePatternPrefix + LOG4CXX_STR("/file-1.gz")).exists(p));
	}

};


LOGUNIT_TEST_SUITE_REGISTRATION(ManualRollingTest);