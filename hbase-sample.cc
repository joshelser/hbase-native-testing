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
DEFINE_string(families, "f", "comma separated list of column family names");

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

  std::vector<std::string> families;
  std::size_t pos = 0, found;
  while ((found = FLAGS_families.find_first_of(',', pos)) != std::string::npos) {
    families.push_back(FLAGS_families.substr(pos, found - pos));
    pos = found + 1;
  }
  families.push_back(FLAGS_families.substr(pos));

  LOG(INFO) << "Sending put requests";
  auto table = client->Table(*tn);
  std::string row = "asdf";
  auto put = Put{row};
  for (auto family : families) {
    put.AddColumn(family, "foo", "1");
  }
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
  for (auto family : families) {
    LOG(INFO) << "Row: " << results[0]->Row() << " " << family << ":foo -> " << results[0]->Value(family, "foo").value_or("FAILED!");
  }

  client->Close();

  return 0;
}
