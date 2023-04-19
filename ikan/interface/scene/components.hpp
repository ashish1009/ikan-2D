//
//  components.hpp
//  ikan
//
//  Created by Ashish . on 10/04/23.
//

#pragma once

#include "renderer/graphics/texture.hpp"
#include "camera/scene_camera.hpp"

namespace ikan {
  
#define UPDATE_TRANSFORM(param) \
  switch (axis) { \
    case Axis::X: param.x = value; break; \
    case Axis::Y: param.y = value; break; \
    case Axis::Z: param.z = value; break; \
    default: IK_ASSERT(false) \
  } \
  transform = Math::GetTransformMatrix(position, rotation, scale);

  
#define ADD_TRANSFORM(param) \
  switch (axis) { \
    case Axis::X: param.x += value; break; \
    case Axis::Y: param.y += value; break; \
    case Axis::Z: param.z += value; break; \
    default: IK_ASSERT(false) \
  } \
  transform = Math::GetTransformMatrix(position, rotation, scale);

  struct IDComponent {
    UUID id = 0;
    void Copy(const IDComponent& other);
    IDComponent(const UUID& id);
    ~IDComponent();
    DEFINE_COPY_MOVE_CONSTRUCTORS(IDComponent);
  };
  
  struct TagComponent {
    std::string tag = "Default Entity";
    void Copy(const TagComponent& other);
    TagComponent(const std::string& tag);
    ~TagComponent();
    DEFINE_COPY_MOVE_CONSTRUCTORS(TagComponent);
  };
  
  struct TransformComponent {
    const glm::mat4& Transform() const { return transform; }
    const glm::vec3& Position() const { return position; }
    const glm::vec3& Rotation() const { return rotation; }
    const glm::vec3& Scale() const { return scale; }
    const glm::quat& Quaternion() const { return quaternion; }

    void UpdatePosition(Axis axis, float value) { UPDATE_TRANSFORM(position) }
    void UpdateRotation(Axis axis, float value) { UPDATE_TRANSFORM(rotation) }
    void UpdateScale(Axis axis, float value) { UPDATE_TRANSFORM(scale) }

    void UpdatePosition(const glm::vec3& value) { position = value;
      transform = Math::GetTransformMatrix(position, rotation, scale);
    }
    void UpdateRotation(const glm::vec3& value) { rotation = value;
      transform = Math::GetTransformMatrix(position, rotation, scale);
    }
    void UpdateScale(const glm::vec3& value) { scale = value;
      transform = Math::GetTransformMatrix(position, rotation, scale);
    }

    void AddPosition(Axis axis, float value) { ADD_TRANSFORM(position) }
    void AddRotation(Axis axis, float value) { ADD_TRANSFORM(rotation) }
    void AddScale(Axis axis, float value) { ADD_TRANSFORM(scale) }

    void Copy(const TransformComponent& other);
    void RenderGui();
    TransformComponent();
    ~TransformComponent();
    DEFINE_COPY_MOVE_CONSTRUCTORS(TransformComponent);

  private:
    glm::quat quaternion;
    glm::mat4 transform;
    glm::vec3 position{0.0f}, rotation{0.0f}, scale{1.0f};
  };
  
  struct CameraComponent {
    bool is_fixed_aspect_ratio = false;
    bool is_primary = false;
    std::shared_ptr<SceneCamera> camera;
    
    void Copy(const CameraComponent& other);
    void RenderGui();
    CameraComponent(SceneCamera::ProjectionType proj_type = SceneCamera::ProjectionType::Orthographic);
    ~CameraComponent();
    DEFINE_COPY_MOVE_CONSTRUCTORS(CameraComponent);
  };

  struct QuadComponent {
    SpriteComponent sprite;
    glm::vec4 color{1.0f};
    
    void Copy(const QuadComponent& other);
    void RenderGui();
    QuadComponent();
    ~QuadComponent();
    DEFINE_COPY_MOVE_CONSTRUCTORS(QuadComponent);
  };
  
  struct CircleComponent {
    TextureComponent texture_comp;
    glm::vec4 color{1.0f};
    
    float thickness = 1.0f;
    float fade = 0.005f;
    
    void Copy(const CircleComponent& other);
    void RenderGui();
    CircleComponent();
    ~CircleComponent();
    DEFINE_COPY_MOVE_CONSTRUCTORS(CircleComponent);
  };
  
  template<typename... Component>
  struct ComponentGroup {
  };
  
#define ALL_COPY_COMPONENTS TransformComponent, QuadComponent, CircleComponent, CameraComponent

  // Stores all the components present in Engine
  using AllComponents =
  ComponentGroup<IDComponent, TagComponent, ALL_COPY_COMPONENTS>;
  
  // Stores the components that need to be copied
  using AllCopyComponents =
  ComponentGroup<ALL_COPY_COMPONENTS>;
  
} // namespace ikan
