#include <gflags/gflags.h>

int main(int argc, char *argv[]) {
  gflags::SetUsageMessage("Sample application");
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  return 0;
}
