#ifndef SUPER_BOT_HPP
#define SUPER_BOT_HPP

#include <glog/logging.h>
#include <sc2api/sc2_api.h>
#include <sc2renderer/sc2_renderer.h>

#include <algorithm>
#include <array>

class CustomRenderer {
  char const *CONFIG_OBJECT = "render";

  static const char *CONFIG_INT_MEMBERS[];
  static const int CONFIG_INT_MEMBERS_NUM;
  char const *CONFIG_BOOL_MEMBER = "on";

  enum IntOptions { MAP_X, MAP_Y, MINI_MAP_X, MINI_MAP_Y, MAX };

  std::array<int, IntOptions::MAX> IntOpts;
  bool On;

  bool LoadOpts(void);

public:
  CustomRenderer() {
    // Default values
    IntOpts[MAP_X] = 800;
    IntOpts[MAP_Y] = 600;
    IntOpts[MINI_MAP_X] = 300;
    IntOpts[MINI_MAP_Y] = 300;
    On = false;

    LoadOpts();
  }

  void Init(void);

  void Close(void) {
    if (On == false)
      return;

    sc2::renderer::Shutdown();
  }
  void Render(const SC2APIProtocol::Observation *observation);

  int GetSettings(sc2::RenderSettings &settings) {
    if (On == false)
      return 0;

    settings.map_x = IntOpts[MAP_X];
    settings.map_y = IntOpts[MAP_Y];
    settings.minimap_x = IntOpts[MINI_MAP_X];
    settings.minimap_y = IntOpts[MINI_MAP_Y];
    return 1;
  }
};

class SuperBot : public sc2::Agent {
  char const *CONFIG_OBJECT = "bot";
  static const char *CONFIG_INT_MEMBERS[];
  static const int CONFIG_INT_MEMBERS_NUM;

  enum IntOptions { MAX_NUM_WORKERS, MAX_FOOD_CAP, FOOD_GAP, MAX };

  CustomRenderer CustRender;
  sc2::Race MyRace;
  std::array<int, IntOptions::MAX> IntOpts;

  SuperBot() : sc2::Agent() {
    MyRace = sc2::Race::Terran;
    IntOpts[MAX_NUM_WORKERS] = 70;
    IntOpts[MAX_FOOD_CAP] = 200;
	IntOpts[FOOD_GAP] = 12;

    LoadConfig();
  }

  // Called from OnUnitIdle::TERRAN_COMMANDCENTER
  void BuildMoreWorkers(const sc2::Unit *unit);
  void LoadConfig();
  bool TryBuildSupplyDepot();
  const sc2::Unit *FindNearestUnit(const sc2::Point2D &start,
                                   sc2::UNIT_TYPEID unit_type);
  const sc2::Unit *GetUnit(sc2::UNIT_TYPEID unit_type);
  const sc2::Unit *GetFreeWorker(sc2::UNIT_TYPEID unit_type);
  void SendWorkerToMine(const sc2::Unit *worker);
  void BuildStructure(const sc2::Unit *unit_to_build,
                      sc2::ABILITY_ID ability_type_for_structure,
                      const sc2::Point2D &location);
  bool IsWorker(const sc2::Unit *worker);
  sc2::Point2D GetRandom2dPointNear(const sc2::Point2D &location);
  int GetNumOfBuildingUnderConstr(sc2::ABILITY_ID ability_type_for_structure);

public:
  static SuperBot &GetSuperBot() {
    static SuperBot rc;
    return rc;
  }

  SuperBot(SuperBot const &) = delete;
  void operator=(SuperBot const &) = delete;

  virtual void OnGameStart() final;

  virtual void OnStep() final;

  virtual void OnGameEnd() final { CustRender.Close(); }

  virtual void OnUnitIdle(const sc2::Unit *unit) final;

  // Called by Coordinator::SetMyRenderer
  int LoadRendererSettings(sc2::RenderSettings &settings) {
    return CustRender.GetSettings(settings);
  }
};

#endif
