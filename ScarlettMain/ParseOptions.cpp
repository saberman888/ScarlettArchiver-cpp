#include "ParseOptions.hpp"

namespace ScarlettOptions
{
  struct POptions ParseOptions(int argc, char* argv[])
  {
    namespace bpo = boost::program_options;
    POptions options;

    bpo::positional_options_description pOptions;
    pOptions.add("subreddit", 1);
    pOptions.add("start-date", 1);
    pOptions.add("end-date", 1);

    bpo::options_description description;
    description.add_options()
        ("help", "display a help message")
        ("subreddit", bpo::value<std::string>(&options.Subreddit), "A reddit subreddit")
        ("start-date", bpo::value<std::string>(&options.StartTime), "The date to start from")
        ("end-date", bpo::value<std::string>(&options.EndTime), "The date to stop")
    ;
    bpo::variables_map vm;
    bpo::store(bpo::command_line_parser(argc, argv).options(description).positional(pOptions).run(), vm);
    bpo::notify(vm);
    return options;
  }
}
