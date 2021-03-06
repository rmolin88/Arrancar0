// File:           coordinator.hpp
// Description:    Wrapper around sc2::Coordinator
// Author:		    Reinaldo Molina
// Email:          rmolin88 at gmail dot com
// Revision:	    0.0.0
// Created:        Fri Jun 01 2018 19:25
// Last Modified:  Fri Jun 01 2018 19:25

#ifndef COORDINATOR_HPP
#define COORDINATOR_HPP

#include <sc2api/sc2_api.h>

#include <unordered_map>
#include <string>
#include <vector>

class Coordinator : public sc2::Coordinator {
  enum StringOptions { MAP = 0, BOT_RACE, OPPONENT_RACE, MAX };
  char const *CONFIG_OBJECT = "coordinator";
  char const *CONFIG_CLI_MEMBER = "cli_option_";
  static const std::unordered_map<std::string, sc2::Race> CONFIG_RACE_MAP;
  static const char *CONFIG_STRING_MEMBERS[];
  static const int CONFIG_STRING_MEMBERS_NUM;
  static const char *CONFIG_STRING_DEFAULT_MEMBERS[];
  static const int CONFIG_STRING_DEFAULT_MEMBERS_NUM;

  const int CLI_OPTIONS_MAX = 10;

  std::string StrOpts[StringOptions::MAX];
  sc2::Race BotRace;
  sc2::Race OpponentRace;

  Coordinator() : sc2::Coordinator() {
    for (int k = 0; k < CONFIG_STRING_DEFAULT_MEMBERS_NUM; k++)
      StrOpts[k] = CONFIG_STRING_DEFAULT_MEMBERS[k];
	BotRace = sc2::Race::Terran;
	OpponentRace = sc2::Race::Random;
  }

public:
  static Coordinator &GetCoordinator(void) {
    static Coordinator rc;
    return rc;
  }

  Coordinator(Coordinator const &) = delete;
  void operator=(Coordinator const &) = delete;

  void LoadMyConfiguration(int argc, const char **argv);
  void SetMyRenderer();
  void AddMyCommandLineOpts();
  void SetMyParticipants();

  bool LaunchGame();
  sc2::Race GetBotRace() { return BotRace; }
};

#endif
