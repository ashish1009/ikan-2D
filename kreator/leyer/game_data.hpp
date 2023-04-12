//
//  game_data.hpp
//  kreator
//
//  Created by Ashish . on 05/04/23.
//

#pragma once

namespace kreator {
  
  using namespace ikan;
  
  enum class GameType {
    Editor
  };
  
  class GameData {
  public:
    virtual ~GameData() = default;
    
    /// Initilize your game data here. This function triggers when scene in Open, Play or Edit in game
    /// - Note: Whenever Renderer Layer change the Scene pointer this function calls
    /// - Parameters scene: This is the reference of created scene
    virtual void Init(const std::shared_ptr<Scene> scene) = 0;

    /// This function returns name of your game
    virtual std::string GameName() const = 0;
    /// This function return your viewport background color
    virtual glm::vec4 GetBgColor() const = 0;
    /// This function returns the Regular Font information
    virtual Font RegularFontData() const = 0;
    /// This function returns the Bold Font information
    virtual Font BoldFontData() const = 0;
    /// This function returns the paths of directory you want in content browser panel
    virtual std::string CbpRootDir() const = 0;
    /// This function returns the paths of directory you want in content browser panel
    virtual std::vector<std::filesystem::path> FavDirecotries() const = 0;
    /// This function returns the path of folder where cliet scenes are saved
    virtual std::string GetScenePath() const = 0;
    
    void SetPlaying(bool playing_flag) { is_playing_ = playing_flag; }
    bool IsPlaying() { return is_playing_; }
    
  protected:
    bool is_playing_ = false;
  };
  
  class GameDataImpl : public GameData {
  public:
    void Init(const std::shared_ptr<Scene> scene) override {};

    std::string GameName() const override { return "Kreator Editor"; }
    glm::vec4 GetBgColor() const override { return {0.5f, 0.2f, 0.2f, 1.0f}; }
    std::string GetScenePath() const override { return DM::ClientAsset("scenes/"); }
    std::string CbpRootDir() const override { return GetScenePath(); };
    std::vector<std::filesystem::path> FavDirecotries() const override {
      return {
        DM::ClientAsset("scenes"),
        DM::ClientAsset("texture"),
      };
    };
    Font RegularFontData() const override {
      return {DM::ClientAsset("fonts/Opensans/Regular.ttf"), 14};
    };
    Font BoldFontData() const override {
      return {DM::ClientAsset("fonts/Opensans/Bold.ttf"), 14};
    };
  };
  
  /// This function is the defination of game data. Implementaiton should be at client
  std::unique_ptr<GameData> CreateGameData(GameType game_type);
  
} // namespace kreator
