#include "gtest/gtest.h"
#include <socket/tcp.hpp>
#include <span>
#include <string>

class TcpSocketTest : public testing::Test {};

TEST_F(TcpSocketTest, Open)
{
  Socket::Tcp socket;
  int fd = socket.handle();
  ASSERT_LE(0, fd);
  printf("FD: %d\n", fd);
}

TEST_F(TcpSocketTest, Connect)
{
  Socket::Tcp socket;

  using namespace std::string_view_literals;
  Socket::Address addr("104.18.26.120"sv, 80); // >nslookup example.com
  const auto connect_result = socket.connect(addr);
  ASSERT_FALSE(connect_result.has_value()) << to_string(*connect_result);
  printf("Passed connect\n");
}

TEST_F(TcpSocketTest, ClientServer)
{
  Socket::Tcp client;
  Socket::Tcp server;
  ASSERT_TRUE(client);
  ASSERT_TRUE(server);
  Socket::Address addr("127.0.0.1", 8008);

  const auto bind_result = server.bind(addr);
  EXPECT_FALSE(bind_result.has_value()) << to_string(*bind_result);
  EXPECT_FALSE(server.listen(5).has_value());
  const auto connect_result = client.connect(addr);
  EXPECT_FALSE(connect_result.has_value()) << to_string(*connect_result);
  auto res = server.accept();
  EXPECT_TRUE(res.has_value()) << to_string(res.error());
  if (!res.has_value()) return;
  Socket::Tcp connection(std::move(res.value()));
  EXPECT_EQ(client.peername().value(), addr);
  EXPECT_EQ(client.sockname().value(), connection.peername().value());
  EXPECT_EQ(server.sockname().value(), connection.sockname().value());

  std::string msg("ping");
  client.send(std::as_bytes(std::span{msg}));
  const auto receive_result = connection.receive(msg.size());
  ASSERT_TRUE(receive_result.has_value()) << to_string(receive_result.error());
  std::string received_message {reinterpret_cast<const char*>(receive_result.value().data()), receive_result.value().size()};
  ASSERT_EQ(received_message, msg);
}

TEST_F(TcpSocketTest, ClientServer_Large)
{
  std::string example_json = R"json(
{"web-app": {
  "servlet": [
    {
      "servlet-name": "cofaxCDS",
      "servlet-class": "org.cofax.cds.CDSServlet",
      "init-param": {
        "configGlossary:installationAt": "Philadelphia, PA",
        "configGlossary:adminEmail": "ksm@pobox.com",
        "configGlossary:poweredBy": "Cofax",
        "configGlossary:poweredByIcon": "/images/cofax.gif",
        "configGlossary:staticPath": "/content/static",
        "templateProcessorClass": "org.cofax.WysiwygTemplate",
        "templateLoaderClass": "org.cofax.FilesTemplateLoader",
        "templatePath": "templates",
        "templateOverridePath": "",
        "defaultListTemplate": "listTemplate.htm",
        "defaultFileTemplate": "articleTemplate.htm",
        "useJSP": false,
        "jspListTemplate": "listTemplate.jsp",
        "jspFileTemplate": "articleTemplate.jsp",
        "cachePackageTagsTrack": 200,
        "cachePackageTagsStore": 200,
        "cachePackageTagsRefresh": 60,
        "cacheTemplatesTrack": 100,
        "cacheTemplatesStore": 50,
        "cacheTemplatesRefresh": 15,
        "cachePagesTrack": 200,
        "cachePagesStore": 100,
        "cachePagesRefresh": 10,
        "cachePagesDirtyRead": 10,
        "searchEngineListTemplate": "forSearchEnginesList.htm",
        "searchEngineFileTemplate": "forSearchEngines.htm",
        "searchEngineRobotsDb": "WEB-INF/robots.db",
        "useDataStore": true,
        "dataStoreClass": "org.cofax.SqlDataStore",
        "redirectionClass": "org.cofax.SqlRedirection",
        "dataStoreName": "cofax",
        "dataStoreDriver": "com.microsoft.jdbc.sqlserver.SQLServerDriver",
        "dataStoreUrl": "jdbc:microsoft:sqlserver://LOCALHOST:1433;DatabaseName=goon",
        "dataStoreUser": "sa",
        "dataStorePassword": "dataStoreTestQuery",
        "dataStoreTestQuery": "SET NOCOUNT ON;select test='test';",
        "dataStoreLogFile": "/usr/local/tomcat/logs/datastore.log",
        "dataStoreInitConns": 10,
        "dataStoreMaxConns": 100,
        "dataStoreConnUsageLimit": 100,
        "dataStoreLogLevel": "debug",
        "maxUrlLength": 500}},
    {
      "servlet-name": "cofaxEmail",
      "servlet-class": "org.cofax.cds.EmailServlet",
      "init-param": {
      "mailHost": "mail1",
      "mailHostOverride": "mail2"}},
    {
      "servlet-name": "cofaxAdmin",
      "servlet-class": "org.cofax.cds.AdminServlet"},
 
    {
      "servlet-name": "fileServlet",
      "servlet-class": "org.cofax.cds.FileServlet"},
    {
      "servlet-name": "cofaxTools",
      "servlet-class": "org.cofax.cms.CofaxToolsServlet",
      "init-param": {
        "templatePath": "toolstemplates/",
        "log": 1,
        "logLocation": "/usr/local/tomcat/logs/CofaxTools.log",
        "logMaxSize": "",
        "dataLog": 1,
        "dataLogLocation": "/usr/local/tomcat/logs/dataLog.log",
        "dataLogMaxSize": "",
        "removePageCache": "/content/admin/remove?cache=pages&id=",
        "removeTemplateCache": "/content/admin/remove?cache=templates&id=",
        "fileTransferFolder": "/usr/local/tomcat/webapps/content/fileTransferFolder",
        "lookInContext": 1,
        "adminGroupID": 4,
        "betaServer": true}}],
  "servlet-mapping": {
    "cofaxCDS": "/",
    "cofaxEmail": "/cofaxutil/aemail/*",
    "cofaxAdmin": "/admin/*",
    "fileServlet": "/static/*",
    "cofaxTools": "/tools/*"},
 
  "taglib": {
    "taglib-uri": "cofax.tld",
    "taglib-location": "/WEB-INF/tlds/cofax.tld"}}}
  )json";
  Socket::Tcp client;
  Socket::Tcp server;
  ASSERT_TRUE(client);
  ASSERT_TRUE(server);
  Socket::Address addr("127.0.0.1", 8008);

  const auto bind_result = server.bind(addr);
  EXPECT_FALSE(bind_result.has_value()) << to_string(*bind_result);
  EXPECT_FALSE(server.listen(5).has_value());
  const auto connect_result = client.connect(addr);
  EXPECT_FALSE(connect_result.has_value()) << to_string(*connect_result);
  auto res = server.accept();
  EXPECT_TRUE(res.has_value()) << to_string(res.error());
  if (!res.has_value()) return;
  Socket::Tcp connection(std::move(res.value()));
  EXPECT_EQ(client.peername().value(), addr);
  EXPECT_EQ(client.sockname().value(), connection.peername().value());
  EXPECT_EQ(server.sockname().value(), connection.sockname().value());

  EXPECT_FALSE(client.send(std::as_bytes(std::span{example_json})).has_value());
  const auto receive_result = connection.receive(example_json.size());
  ASSERT_TRUE(receive_result.has_value()) << to_string(receive_result.error());
  std::string received_message {reinterpret_cast<const char*>(receive_result.value().data()), receive_result.value().size()};
  ASSERT_EQ(received_message, example_json);
}
