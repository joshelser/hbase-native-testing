#include <folly/Logging.h>
#include <folly/Random.h>
#include <gflags/gflags.h>

#include <atomic>
#include <chrono>
#include <iostream>
#include <thread>

#include "hbase/client/client.h"
#include "hbase/client/get.h"
#include "hbase/client/put.h"
#include "hbase/client/table.h"
#include "hbase/serde/table-name.h"
#include "hbase/utils/time-util.h"

using hbase::Client;
using hbase::Configuration;
using hbase::Get;
using hbase::Put;
using hbase::Table;
using hbase::pb::TableName;
using hbase::TimeUtil;
using folly::Random;

DEFINE_string(zookeeper, "localhost:2181", "What zk quorum to talk to");
DEFINE_string(znode, "/hbase", "parent znode");
DEFINE_string(table, "load_test_table", "What table to do the reads and writes with");

int main(int argc, char *argv[]) {
  gflags::SetUsageMessage("Sample application");
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  google::InitGoogleLogging(argv[0]);
  google::InstallFailureSignalHandler();
  FLAGS_logtostderr = 1;
  FLAGS_stderrthreshold = 1;

  // Configuration
  std::shared_ptr<Configuration> conf = std::make_shared<Configuration>();
  conf->Set("hbase.zookeeper.quorum", FLAGS_zookeeper);
  conf->Set("zookeeper.znode.parent", FLAGS_znode);

  auto tn = std::make_shared<TableName>(folly::to<TableName>(FLAGS_table));
  auto num_puts = 1000;

  auto client = std::make_unique<Client>(*conf);
  const std::string family = "f";

  LOG(INFO) << "Sending put requests";
  auto table = client->Table(*tn);
  std::string row = "asdf";
  auto put = Put{row};
  put.AddColumn(family, "foo", "1");
  table->Put(put);

  LOG(INFO) << "Sending get requests";
  std::vector<hbase::Get> gets;

  hbase::Get get(row);
  gets.push_back(get);
  auto results = table->Get(gets);
  if (results.size() != 1) {
    LOG(ERROR) << "Expected one result but got " << results.size();
    return 1;
  }
  std::shared_ptr<hbase::Result> result = results[0];
  if (result == nullptr || result->IsEmpty()) {
    LOG(ERROR) << "Could not find the record!";
  } else {
    LOG(INFO) << result->DebugString();
  }
  //    LOG(INFO) << "For family: " << family;
  //    LOG(INFO) << "Row: " << result->Row();
  //    if (result->Value(family, "foo")) {
  //      LOG(INFO) << "Found the record!";
  //      LOG(INFO) << "Row: " << result->Row() << " " << family << ":foo -> " << *(result->Value(family, "foo"));
  //    } else {
  //      LOG(ERROR) << "Could not find the record!";
  //    }
  //LOG(INFO) << "Row: " << results[0]->Row() << " " << family << ":foo -> " << results[0]->Value(family, "foo").value_or("FAILED!");

  client->Close();

  return 0;
}
