#include <cli/Logger.hpp>
#include <abyssutils/os/File.hpp>

#include <gtest/gtest.h>

#include <filesystem>

using namespace Abyssutils::CLI;
using namespace Abyssutils;

TEST(CLI, Logger) {
    CLogger logger;

    logger.setEnableRolling(true);

    logger.log(Abyssutils::CLI::LOG_DEBUG, "Hello!");

    EXPECT_EQ(logger.rollingLog(), "DEBUG ]: Hello!");

    logger.log(Abyssutils::CLI::LOG_TRACE, "Hello!");

    EXPECT_EQ(logger.rollingLog(), "DEBUG ]: Hello!");

    logger.setLogLevel(LOG_TRACE);

    logger.log(Abyssutils::CLI::LOG_TRACE, "Hello, {}!", "Trace");

    EXPECT_EQ(logger.rollingLog(), "DEBUG ]: Hello!\nTRACE ]: Hello, Trace!");

    CLoggerConnection connection(logger);
    connection.setName("conn");

    connection.log(Abyssutils::CLI::LOG_TRACE, "Hello from connection!");

    EXPECT_EQ(logger.rollingLog(), "DEBUG ]: Hello!\nTRACE ]: Hello, Trace!\nTRACE from conn ]: Hello from connection!");

    connection.setLogLevel(Abyssutils::CLI::LOG_WARN);

    connection.log(Abyssutils::CLI::LOG_DEBUG, "Hello from connection!");

    EXPECT_EQ(logger.rollingLog(), "DEBUG ]: Hello!\nTRACE ]: Hello, Trace!\nTRACE from conn ]: Hello from connection!");

    logger.setEnableRolling(false);

    connection.log(Abyssutils::CLI::LOG_ERR, "Err!");

    EXPECT_EQ(logger.rollingLog(), "DEBUG ]: Hello!\nTRACE ]: Hello, Trace!\nTRACE from conn ]: Hello from connection!");

    logger.setEnableStdout(false);

    logger.log(Abyssutils::CLI::LOG_ERR, "Error");

    EXPECT_EQ(logger.rollingLog(), "DEBUG ]: Hello!\nTRACE ]: Hello, Trace!\nTRACE from conn ]: Hello from connection!");

    auto res = logger.setOutputFile("./loggerFile.log");
    EXPECT_TRUE(res);

    logger.log(LOG_DEBUG, "Hi file!");

    res = logger.setOutputFile(""); // clear
    EXPECT_TRUE(res);

    auto fileRead = File::readFileAsString("./loggerFile.log");
    EXPECT_TRUE(fileRead);

    EXPECT_EQ(fileRead.value_or(""), "DEBUG ]: Hi file!\n");

    std::error_code ec;
    std::filesystem::remove("./loggerFile.log", ec);

    // TODO: maybe find a way to test the times and color?
    logger.setEnableStdout(true);
    logger.setTime(true);

    logger.log(Abyssutils::CLI::LOG_WARN, "Timed warning!");

    logger.setEnableColor(false);

    logger.log(Abyssutils::CLI::LOG_CRIT, "rip");

    logger.setEnableRolling(true);

    // spam some logs to check rolling
    for (size_t i = 0; i < 200; ++i) {
        logger.log(LOG_ERR, "Oh noes!!!");
    }

    EXPECT_TRUE(logger.rollingLog().size() < 4096);
    EXPECT_TRUE(logger.rollingLog().starts_with("ERR")); // test the breaking is done correctly

    // test scoping
    CLogger*           pLogger     = new CLogger();
    CLoggerConnection* pConnection = new CLoggerConnection(*pLogger);

    pLogger->setEnableStdout(false);

    pConnection->log(LOG_DEBUG, "This shouldn't log anything.");

    EXPECT_TRUE(pLogger->rollingLog().empty());

    delete pLogger;

    pConnection->log(LOG_DEBUG, "This shouldn't do anything, or crash.");
}