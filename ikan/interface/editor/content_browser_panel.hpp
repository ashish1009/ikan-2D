//
//  content_browser_panel.hpp
//  ikan
//
//  Created by Ashish . on 07/04/23.
//

#pragma once

namespace ikan {
  
  /// This class renders imgui window to access contents of folders
  class ContentBrowserPanel {
  public:
    /// This constructor creates content Browser Panel instance with root path
    /// - Parameters:
    ///   - root_path: root path of Content browser panel
    ///   - favourite_paths: favourit paths in side bar
    ContentBrowserPanel(const std::string& root_path, const std::vector<std::filesystem::path>& favourite_paths = {});
    /// This destructor destroy Content browser panel instance
    ~ContentBrowserPanel();
    
  private:
    std::filesystem::path root_path_, current_directory_;
  };
  
} // namespace ikan
