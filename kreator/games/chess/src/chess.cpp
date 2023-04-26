//
//  chess.cpp
//  kreator
//
//  Created by Ashish . on 19/04/23.
//

#include "chess.hpp"

namespace chess {
  
  Chess::Chess() {
    CHESS_LOG("Creating Chess Game Data ... ");
    IK_ASSERT(BlockSize > 0);
    
    init_cam_pos_ = {(MaxCols / 2) * BlockSize, (MaxRows / 2) * BlockSize};
    
    Batch2DRenderer::AddLineData(50);
    Batch2DRenderer::AddQuadData(100);
  }
  
  Chess::~Chess() {
    CHESS_LOG("Destroying Chess Game Data ... ");
  }
  
  void Chess::Init(const std::shared_ptr<Scene> scene, Viewport* viewport) {
    CHESS_LOG("Initialising Chess Game Data ... ");
    scene_ = scene;
    viewport_ = viewport;
    scene_->GetSetting().use_editor_camera = false;
  }
  
  void Chess::Update(Timestep ts) {
    if (!scene_) return;
    
    static bool change_camera_pos = true;
    if (change_camera_pos) {
      if (auto& cam_data = scene_->GetPrimaryCameraData(); cam_data.scene_camera) {
        cam_data.transform_comp->UpdatePosition({init_cam_pos_.x, init_cam_pos_.y, 0.0f});
      }
    }
    
    RenderBackgroundAndBorder();
    RenderText();
        
    if (is_playing_) {
      HighlightHoveredBlock();
    }
    else {
      RenderChessGrids();
    }
  }
  
  void Chess::SetViewportSize(uint32_t width, uint32_t height) {
    viewport_width_ = width;
    viewport_height_ = height;
  }
  
  void Chess::RenderGui() {
    if (auto& cam_data = scene_->GetPrimaryCameraData(); cam_data.scene_camera) {
      ImGui::Begin("Zoom");
      cam_data.scene_camera->ZoomWidget();
      ImGui::End();
    }
  }
  
  void Chess::SetPlaying(bool playing_flag) {
    is_playing_ = playing_flag;
    if (is_playing_) {
      CreateBlocks();
    }
  }
  
  void Chess::LoadPrefab(const std::string &path) {
    const auto& cam_data = scene_->GetPrimaryCameraData();
    if (!cam_data.scene_camera) return;

    if (cam_data.scene_camera->GetProjectionType() == SceneCamera::ProjectionType::Orthographic) {
      float zoom = viewport_->height / cam_data.scene_camera->GetZoom();
      float x_pos = (viewport_->mouse_pos_x - (float)viewport_->width / 2) / zoom;
      float y_pos = (viewport_->mouse_pos_y - (float)viewport_->height / 2) / zoom;

      // Getting position relating to camera position
      x_pos += cam_data.position.x;
      y_pos += cam_data.position.y;
      
      // If Position is outside board then return
      if (x_pos < 0 or x_pos > BlockSize * MaxCols) return;
      if (y_pos < 0 or y_pos > BlockSize * MaxRows) return;
  
      // Round of the values in INT
      int32_t x_pos_int = int32_t(x_pos / BlockSize);
      int32_t y_pos_int = int32_t(y_pos / BlockSize);
      
      // Getting position relating to block size
      x_pos = x_pos_int * BlockSize + BlockSize / 2;
      y_pos = y_pos_int * BlockSize + BlockSize / 2;
      
      Entity e = Prefab::Deserialize(path, scene_.get());
      auto& tc = e.GetComponent<TransformComponent>();
      tc.UpdatePosition(X, x_pos);
      tc.UpdatePosition(Y, y_pos);
    }
  }
  
  void Chess::RenderChessGrids() {
    const auto& cam_data = scene_->GetPrimaryCameraData();
    if (!cam_data.scene_camera) return;
    
    static const glm::vec4 color = { 1.0f, 1.0f, 1.0f, 0.6f};
    static const float z = -0.1f;
    Batch2DRenderer::BeginBatch(cam_data.scene_camera->GetProjection() * glm::inverse(cam_data.transform_comp->Transform()));
    for (int32_t row = 0; row <= MaxRows; row++) {
      Batch2DRenderer::DrawLine({0, row * BlockSize, z}, {MaxCols * BlockSize, row * BlockSize, z}, color);
    }
    for (int32_t col = 0; col <= MaxCols; col++) {
      Batch2DRenderer::DrawLine({col * BlockSize, 0, z}, {col * BlockSize, MaxRows * BlockSize, z}, color);
    }
    Batch2DRenderer::EndBatch();
  }
  
  void Chess::RenderBackgroundAndBorder() {
    const auto& cam_data = scene_->GetPrimaryCameraData();
    if (!cam_data.scene_camera) return;
    
    static const std::shared_ptr<Texture> bg = Renderer::GetTexture(DM::ClientAsset("textures/background.png"));
    static const std::shared_ptr<Texture> border = Renderer::GetTexture(DM::ClientAsset("textures/border.png"));
    
    Batch2DRenderer::BeginBatch(cam_data.scene_camera->GetProjection() * glm::inverse(cam_data.transform_comp->Transform()));
    
    // Background
    static glm::mat4 bg_transform = Math::GetTransformMatrix({init_cam_pos_.x, init_cam_pos_.y, -0.9}, {0, 0, 0}, {100, 100, 1});
    Batch2DRenderer::DrawQuad(bg_transform, bg);

    // Border
    static glm::mat4 border_transform = Math::GetTransformMatrix({init_cam_pos_.x, init_cam_pos_.y, -0.5},
                                                                 {0, 0, 0},
                                                                 {(BlockSize * (MaxCols + 1.6)), (BlockSize * (MaxRows + 1.6)), 1});
    Batch2DRenderer::DrawQuad(border_transform, border, { 0.641860485, 0.468707442, 0.468707442, 1 });
    
    Batch2DRenderer::EndBatch();
  }
  
  void Chess::RenderText() {
    const auto& cam_data = scene_->GetPrimaryCameraData();
    if (!cam_data.scene_camera) return;
    
    static glm::vec2 size = {0.3f, 0.3f};
    static glm::vec4 color = { 0.7, 0.7, 0.7, 1};

    TextRenderer::BeginBatch(cam_data.scene_camera->GetProjection() * glm::inverse(cam_data.transform_comp->Transform()));
    for (int32_t row = 0;  row < MaxCols; row++) {
      TextRenderer::RenderText(std::to_string(row + 1), { -1, 0.5 + (BlockSize * row), 0.3f }, size, color);
    }
    for (int32_t col = 0;  col < MaxCols; col++) {
      TextRenderer::RenderText(std::string(1, 'A' + col), { 0.7f + (BlockSize * col), -1.0f, 0.3f }, size, color);
    }
    TextRenderer::EndBatch();
  }
  
  void Chess::HighlightHoveredBlock() {
    const auto& cam_data = scene_->GetPrimaryCameraData();
    if (!cam_data.scene_camera) return;

    int32_t hovered_entity_id = -1;
    Renderer::GetEntityIdFromPixels(viewport_->mouse_pos_x, viewport_->mouse_pos_y, viewport_->framebuffer->GetPixelIdIndex(), hovered_entity_id);
    
    if (hovered_entity_id == -1) return;
    
    Entity* hovered_entity = (hovered_entity_id > (int32_t)scene_->GetMaxEntityId()) ? nullptr : scene_->GetEnitityFromId(hovered_entity_id);
    if (!hovered_entity)
      return;
    
    auto& tc = hovered_entity->GetComponent<TransformComponent>();
    
    static const std::shared_ptr<Texture> hovered = Renderer::GetTexture(DM::ClientAsset("textures/hovered.png"));
    
    Batch2DRenderer::BeginBatch(cam_data.scene_camera->GetProjection() * glm::inverse(cam_data.transform_comp->Transform()));
    
    const auto& p = tc.Position();
    glm::mat4 transform = Math::GetTransformMatrix({p.x, p.y, 0.2}, tc.Rotation(), {BlockSize, BlockSize, 1});
    Batch2DRenderer::DrawQuad(transform, hovered);
    
    Batch2DRenderer::EndBatch();
  }
  
  void Chess::CreateBlocks() {
    auto view = scene_->GetEntitesWith<TagComponent, TransformComponent>();
    for (auto e : view) {
      const auto& [tag_comp, transform_comp] = view.get<TagComponent, TransformComponent>(e);
      if (tag_comp.tag == "block") {
        const auto &p = transform_comp.Position();
        
        // Getting Row and column number from Position relative to block size
        float row = (p.y - BlockSize / 2) / BlockSize;
        float col = (p.x - BlockSize / 2) / BlockSize;
        
        // If Row and column out of bond
        IK_ASSERT(row >= 0 and row < MaxRows);
        IK_ASSERT(col >= 0 and col < MaxCols);
        
        blocks_[row][col] = std::make_shared<Block>();
      }
    }
  }
  
} // namespace chess
