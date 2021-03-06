
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <pompeii/pompeii.h>
#include <pompeiiUtils/pompeiiUtils.h>
using namespace pompeii;

#include <routes.h>

#include <glslang/SPIRV/GlslangToSpv.h>

EShLanguage FindLanguage(const vk::ShaderStageFlagBits shaderType) {
  switch (shaderType) {
    case vk::ShaderStageFlagBits::eVertex:
      return EShLangVertex;

    case vk::ShaderStageFlagBits::eTessellationControl:
      return EShLangTessControl;

    case vk::ShaderStageFlagBits::eTessellationEvaluation:
      return EShLangTessEvaluation;

    case vk::ShaderStageFlagBits::eGeometry:
      return EShLangGeometry;

    case vk::ShaderStageFlagBits::eFragment:
      return EShLangFragment;

    case vk::ShaderStageFlagBits::eCompute:
      return EShLangCompute;

    default:
      return EShLangVertex;
  }
}

void init_resources(TBuiltInResource &_resource) {
  _resource.maxLights = 32;
  _resource.maxClipPlanes = 6;
  _resource.maxTextureUnits = 32;
  _resource.maxTextureCoords = 32;
  _resource.maxVertexAttribs = 64;
  _resource.maxVertexUniformComponents = 4096;
  _resource.maxVaryingFloats = 64;
  _resource.maxVertexTextureImageUnits = 32;
  _resource.maxCombinedTextureImageUnits = 80;
  _resource.maxTextureImageUnits = 32;
  _resource.maxFragmentUniformComponents = 4096;
  _resource.maxDrawBuffers = 32;
  _resource.maxVertexUniformVectors = 128;
  _resource.maxVaryingVectors = 8;
  _resource.maxFragmentUniformVectors = 16;
  _resource.maxVertexOutputVectors = 16;
  _resource.maxFragmentInputVectors = 15;
  _resource.minProgramTexelOffset = -8;
  _resource.maxProgramTexelOffset = 7;
  _resource.maxClipDistances = 8;
  _resource.maxComputeWorkGroupCountX = 65535;
  _resource.maxComputeWorkGroupCountY = 65535;
  _resource.maxComputeWorkGroupCountZ = 65535;
  _resource.maxComputeWorkGroupSizeX = 1024;
  _resource.maxComputeWorkGroupSizeY = 1024;
  _resource.maxComputeWorkGroupSizeZ = 64;
  _resource.maxComputeUniformComponents = 1024;
  _resource.maxComputeTextureImageUnits = 16;
  _resource.maxComputeImageUniforms = 8;
  _resource.maxComputeAtomicCounters = 8;
  _resource.maxComputeAtomicCounterBuffers = 1;
  _resource.maxVaryingComponents = 60;
  _resource.maxVertexOutputComponents = 64;
  _resource.maxGeometryInputComponents = 64;
  _resource.maxGeometryOutputComponents = 128;
  _resource.maxFragmentInputComponents = 128;
  _resource.maxImageUnits = 8;
  _resource.maxCombinedImageUnitsAndFragmentOutputs = 8;
  _resource.maxCombinedShaderOutputResources = 8;
  _resource.maxImageSamples = 0;
  _resource.maxVertexImageUniforms = 0;
  _resource.maxTessControlImageUniforms = 0;
  _resource.maxTessEvaluationImageUniforms = 0;
  _resource.maxGeometryImageUniforms = 0;
  _resource.maxFragmentImageUniforms = 8;
  _resource.maxCombinedImageUniforms = 8;
  _resource.maxGeometryTextureImageUnits = 16;
  _resource.maxGeometryOutputVertices = 256;
  _resource.maxGeometryTotalOutputComponents = 1024;
  _resource.maxGeometryUniformComponents = 1024;
  _resource.maxGeometryVaryingComponents = 64;
  _resource.maxTessControlInputComponents = 128;
  _resource.maxTessControlOutputComponents = 128;
  _resource.maxTessControlTextureImageUnits = 16;
  _resource.maxTessControlUniformComponents = 1024;
  _resource.maxTessControlTotalOutputComponents = 4096;
  _resource.maxTessEvaluationInputComponents = 128;
  _resource.maxTessEvaluationOutputComponents = 128;
  _resource.maxTessEvaluationTextureImageUnits = 16;
  _resource.maxTessEvaluationUniformComponents = 1024;
  _resource.maxTessPatchComponents = 120;
  _resource.maxPatchVertices = 32;
  _resource.maxTessGenLevel = 64;
  _resource.maxViewports = 16;
  _resource.maxVertexAtomicCounters = 0;
  _resource.maxTessControlAtomicCounters = 0;
  _resource.maxTessEvaluationAtomicCounters = 0;
  _resource.maxGeometryAtomicCounters = 0;
  _resource.maxFragmentAtomicCounters = 8;
  _resource.maxCombinedAtomicCounters = 8;
  _resource.maxAtomicCounterBindings = 1;
  _resource.maxVertexAtomicCounterBuffers = 0;
  _resource.maxTessControlAtomicCounterBuffers = 0;
  _resource.maxTessEvaluationAtomicCounterBuffers = 0;
  _resource.maxGeometryAtomicCounterBuffers = 0;
  _resource.maxFragmentAtomicCounterBuffers = 1;
  _resource.maxCombinedAtomicCounterBuffers = 1;
  _resource.maxAtomicCounterBufferSize = 16384;
  _resource.maxTransformFeedbackBuffers = 4;
  _resource.maxTransformFeedbackInterleavedComponents = 64;
  _resource.maxCullDistances = 8;
  _resource.maxCombinedClipAndCullDistances = 8;
  _resource.maxSamples = 4;
  _resource.limits.nonInductiveForLoops = 1;
  _resource.limits.whileLoops = 1;
  _resource.limits.doWhileLoops = 1;
  _resource.limits.generalUniformIndexing = 1;
  _resource.limits.generalAttributeMatrixVectorIndexing = 1;
  _resource.limits.generalVaryingIndexing = 1;
  _resource.limits.generalSamplerIndexing = 1;
  _resource.limits.generalVariableIndexing = 1;
  _resource.limits.generalConstantMatrixVectorIndexing = 1;
}

bool GLSLtoSPV(vk::ShaderStageFlagBits shaderType, const char *pshader, 
  std::vector<uint32_t> &spirv)
{
  glslang::InitializeProcess();


  EShLanguage stage = FindLanguage(shaderType);
  glslang::TShader* shader = new glslang::TShader(stage);
  glslang::TProgram* program = new glslang::TProgram( );

  const char *shaderStrings[1];

  TBuiltInResource Resources;
  init_resources(Resources);

  // Enable SPIR-V and Vulkan rules when parsing GLSL
  EShMessages messages = (EShMessages)(EShMsgSpvRules | EShMsgVulkanRules);

  shaderStrings[0] = pshader;

  shader->setStrings(shaderStrings, 1);

  std::cout << "Parsing " << std::endl;
  if (!shader->parse(&Resources, 100, false, messages)) {
    puts(shader->getInfoLog());
    puts(shader->getInfoDebugLog());
    return false;
  }
  std::cout << "Parsing OK" << std::endl;

  program->addShader(shader);
  
  // Link the program and report if errors...
  std::cout << "Linking " << std::endl;
  if (!program->link(messages)) {
    puts(shader->getInfoLog());
    puts(shader->getInfoDebugLog());
    return false;
  }
  std::cout << "Linking OK" << std::endl;

  glslang::GlslangToSpv(*program->getIntermediate(stage), spirv);

  glslang::FinalizeProcess();
  
  return true;
}

int main( )
{
  std::shared_ptr<Instance> instance;
  std::shared_ptr<PhysicalDevice> physicalDevice;
  std::shared_ptr<Device> device;
  uint32_t queueFamilyIdx;
  std::shared_ptr<Queue> queue;
  std::shared_ptr<CommandPool> cmdPool;
  std::shared_ptr<CommandBuffer> cmdBuffer;
  std::shared_ptr<DescriptorSetLayout> descriptorSetLayout;
  std::shared_ptr<PipelineLayout> pipelineLayout;
  std::shared_ptr<Pipeline> pipeline;
  std::vector<std::shared_ptr<ShaderModule>> shaderModules;
  std::shared_ptr<Buffer> vertexBuffer, indexBuffer;

  struct FramebufferAttachment
  {
    std::shared_ptr<Image> image;
    std::shared_ptr<ImageView> imageView;
  };
  uint32_t width, height;
  std::shared_ptr<Framebuffer> framebuffer;
  FramebufferAttachment colorAttachment, depthAttachment;
  std::shared_ptr<RenderPass> renderPass;

  //vk::DebugReportCallbackEXT


  std::cout << "Create Vulkan Instance...";
  instance = Instance::createDefault( "renderHeadless" );
  std::cout << "OK" << std::endl;

  std::cout << "Find Vulkan physical device...";

  // Find a physical device with presentation support
  assert( instance->getPhysicalDeviceCount( ) != 0 );
  physicalDevice = instance->getPhysicalDevice( 0 );
  if ( !physicalDevice )
  {
    POMPEII_RUNTIME_ERROR( "Failed to find a device with presentation support" );
  }
  std::cout << "OK" << std::endl;

  // Request a single graphics queue
  const float defaultQueuePriority( 0.0f );

  auto queueFamilyProperties = physicalDevice->getQueueFamilyProperties( );

  vk::DeviceQueueCreateInfo queueCreateInfo;
  for ( uint32_t i = 0; i < queueFamilyProperties.size( ); ++i )
  {
    if ( queueFamilyProperties[ i ].queueFlags & vk::QueueFlagBits::eGraphics )
    {
      queueFamilyIdx = i;
      queueCreateInfo.queueFamilyIndex = i;
      queueCreateInfo.queueCount = 1;
      queueCreateInfo.pQueuePriorities = &defaultQueuePriority;
      break;
    }
  }
  std::vector< vk::DeviceQueueCreateInfo > queueCreateInfos;
  queueCreateInfos.push_back( queueCreateInfo );

  // Create logical device
  device = physicalDevice->createDevice( queueCreateInfos, { }, { }, { } );

  // Get a graphics queue
  queue = device->getQueue( queueFamilyIdx, 0 );

  // Command pool
  cmdPool = device->createCommandPool(
    vk::CommandPoolCreateFlagBits::eResetCommandBuffer, queueFamilyIdx );

  // Prepare vertex and index buffers
  struct Vertex
  {
    glm::vec3 position;
    glm::vec3 color;
  };

  std::vector<Vertex> vertices =
  {
    { {  1.0f,  1.0f, 0.0f }, { 1.0f, 0.0f, 0.0f } },
    { { -1.0f,  1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f } },
    { {  0.0f, -1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f } }
  };
  std::vector<uint32_t> indices = { 0, 1, 2 };

  // Copy input data to VRAM using staging buffer
  {
    {
      auto cmd = cmdPool->allocateCommandBuffer( );
      cmd->begin( );

      uint32_t vertexBufferSize = vertices.size( ) * sizeof( Vertex );
      auto stagingBuffer = device->createBuffer( vertexBufferSize,
        vk::BufferUsageFlagBits::eTransferSrc,
        vk::MemoryPropertyFlagBits::eHostVisible |
        vk::MemoryPropertyFlagBits::eHostCoherent );
      stagingBuffer->writeData( 0, vertexBufferSize, vertices.data( ) );

      vertexBuffer = device->createBuffer( vertexBufferSize,
        vk::BufferUsageFlagBits::eVertexBuffer |
        vk::BufferUsageFlagBits::eTransferDst,
        vk::MemoryPropertyFlagBits::eDeviceLocal );
      stagingBuffer->copy( cmd, vertexBuffer, 0, 0, vertexBufferSize );

      cmd->end( );
      queue->submitAndWait( cmd );
    }
    {
      auto cmd = cmdPool->allocateCommandBuffer( );
      cmd->begin( );

      uint32_t indexBufferSize = indices.size( ) * sizeof( uint32_t );

      auto stagingBuffer = device->createBuffer( indexBufferSize,
        vk::BufferUsageFlagBits::eTransferSrc,
        vk::MemoryPropertyFlagBits::eHostVisible |
        vk::MemoryPropertyFlagBits::eHostCoherent );
      stagingBuffer->writeData( 0, indexBufferSize, indices.data( ) );

      indexBuffer = device->createBuffer( indexBufferSize,
        vk::BufferUsageFlagBits::eIndexBuffer |
        vk::BufferUsageFlagBits::eTransferDst,
        vk::MemoryPropertyFlagBits::eDeviceLocal );

      stagingBuffer->copy( cmd, indexBuffer, 0, 0, indexBufferSize );

      cmd->end( );
      queue->submitAndWait( cmd );
    }
  }

  // Create framebuffer attachments
  width = 1024;
  height = 1024;
  vk::Format colorFormat = vk::Format::eR8G8B8A8Unorm;
  vk::Format depthFormat;
  pompeii::utilities::getSupportedDepthFormat( physicalDevice, depthFormat );

  {
    // Color attachment
    colorAttachment.image = device->createImage( { }, vk::ImageType::e2D,
      colorFormat, vk::Extent3D( width, height, 1 ), 1, 1,
      vk::SampleCountFlagBits::e1, vk::ImageTiling::eOptimal,
      vk::ImageUsageFlagBits::eColorAttachment |
      vk::ImageUsageFlagBits::eTransferSrc, vk::SharingMode::eExclusive,
      { }, vk::ImageLayout::eUndefined,
      vk::MemoryPropertyFlagBits::eDeviceLocal );

    colorAttachment.imageView = colorAttachment.image->createImageView(
      vk::ImageViewType::e2D, colorFormat,
      vk::ComponentMapping(
        vk::ComponentSwizzle::eR, vk::ComponentSwizzle::eG,
        vk::ComponentSwizzle::eB, vk::ComponentSwizzle::eA ),
      vk::ImageSubresourceRange( vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1 ) );

    // Depth stencil attachment
    depthAttachment.image = device->createImage( { }, vk::ImageType::e2D,
      depthFormat, vk::Extent3D( width, height, 1 ), 1, 1,
      vk::SampleCountFlagBits::e1, vk::ImageTiling::eOptimal,
      vk::ImageUsageFlagBits::eDepthStencilAttachment, vk::SharingMode::eExclusive,
      { }, vk::ImageLayout::eUndefined,
      vk::MemoryPropertyFlagBits::eDeviceLocal );

    depthAttachment.imageView = depthAttachment.image->createImageView(
      vk::ImageViewType::e2D, depthFormat,
      vk::ComponentMapping(
        vk::ComponentSwizzle::eR, vk::ComponentSwizzle::eG,
        vk::ComponentSwizzle::eB, vk::ComponentSwizzle::eA ),
      vk::ImageSubresourceRange( vk::ImageAspectFlagBits::eDepth |
        vk::ImageAspectFlagBits::eStencil, 0, 1, 0, 1 ) );
  }

  // Create renderpass
  {
    std::array<vk::AttachmentDescription, 2 > attachmentDescriptions;
    // Color attachment
    attachmentDescriptions[ 0 ].format = colorFormat;
    attachmentDescriptions[ 0 ].samples = vk::SampleCountFlagBits::e1;
    attachmentDescriptions[ 0 ].loadOp = vk::AttachmentLoadOp::eClear;
    attachmentDescriptions[ 0 ].storeOp = vk::AttachmentStoreOp::eStore;
    attachmentDescriptions[ 0 ].stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
    attachmentDescriptions[ 0 ].stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
    attachmentDescriptions[ 0 ].initialLayout = vk::ImageLayout::eUndefined;
    attachmentDescriptions[ 0 ].finalLayout = vk::ImageLayout::eTransferSrcOptimal;
    // Depth attachment
    attachmentDescriptions[ 1 ].format = depthFormat;
    attachmentDescriptions[ 1 ].samples = vk::SampleCountFlagBits::e1;
    attachmentDescriptions[ 1 ].loadOp = vk::AttachmentLoadOp::eClear;
    attachmentDescriptions[ 1 ].storeOp = vk::AttachmentStoreOp::eDontCare;
    attachmentDescriptions[ 1 ].stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
    attachmentDescriptions[ 1 ].stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
    attachmentDescriptions[ 1 ].initialLayout = vk::ImageLayout::eUndefined;
    attachmentDescriptions[ 1 ].finalLayout =
      vk::ImageLayout::eDepthStencilAttachmentOptimal;

    vk::AttachmentReference colorReference = { 0,
      vk::ImageLayout::eColorAttachmentOptimal };
    vk::AttachmentReference depthReference = { 1,
      vk::ImageLayout::eDepthStencilAttachmentOptimal };

    vk::SubpassDescription subpassDescription = { };
    subpassDescription.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;
    subpassDescription.colorAttachmentCount = 1;
    subpassDescription.pColorAttachments = &colorReference;
    subpassDescription.pDepthStencilAttachment = &depthReference;

    // Use subpass dependencies for layout transitions
    std::array<vk::SubpassDependency, 2> dependencies;

    dependencies[ 0 ].srcSubpass = VK_SUBPASS_EXTERNAL;
    dependencies[ 0 ].dstSubpass = 0;
    dependencies[ 0 ].srcStageMask = vk::PipelineStageFlagBits::eBottomOfPipe;
    dependencies[ 0 ].dstStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
    dependencies[ 0 ].srcAccessMask = vk::AccessFlagBits::eMemoryRead;
    dependencies[ 0 ].dstAccessMask = vk::AccessFlagBits::eColorAttachmentRead |
      vk::AccessFlagBits::eColorAttachmentWrite;
    dependencies[ 0 ].dependencyFlags = vk::DependencyFlagBits::eByRegion;

    dependencies[ 1 ].srcSubpass = 0;
    dependencies[ 1 ].dstSubpass = VK_SUBPASS_EXTERNAL;
    dependencies[ 1 ].srcStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
    dependencies[ 1 ].dstStageMask = vk::PipelineStageFlagBits::eBottomOfPipe;
    dependencies[ 1 ].srcAccessMask = vk::AccessFlagBits::eColorAttachmentRead |
      vk::AccessFlagBits::eColorAttachmentWrite;
    dependencies[ 1 ].dstAccessMask = vk::AccessFlagBits::eMemoryRead;
    dependencies[ 1 ].dependencyFlags = vk::DependencyFlagBits::eByRegion;

    renderPass = device->createRenderPass(
      attachmentDescriptions, subpassDescription, dependencies );

    std::vector<std::shared_ptr<ImageView>> attachments = {
      colorAttachment.imageView, depthAttachment.imageView
    };
    framebuffer = device->createFramebuffer( renderPass, attachments,
      vk::Extent2D( width, height ), 1 );
  }

  {
    // Prepare graphics pipeline
    descriptorSetLayout = device->createDescriptorSetLayout( { } );

    vk::PushConstantRange pushConstantRange(
      vk::ShaderStageFlagBits::eVertex, 0, sizeof( glm::mat4 )
    );

    pipelineLayout = device->createPipelineLayout(
      descriptorSetLayout, pushConstantRange );

    

    std::vector<uint32_t> vtx_spv, frg_spv;
    pompeii::utils::GLSLtoSPV( vk::ShaderStageFlagBits::eVertex, R"(#version 450

    layout( location = 0 ) in vec3 inPos;
    layout( location = 1 ) in vec3 inColor;

    layout( location = 0 ) out vec3 outColor;

    layout( push_constant ) uniform PushConsts
    {
      mat4 MVP;
    } pcte;

    void main( ) 
    {
      gl_Position = pcte.MVP * vec4( inPos.xyz, 1.0 );
      outColor = inColor;
    } )", vtx_spv );
    pompeii::utils::GLSLtoSPV( vk::ShaderStageFlagBits::eFragment, R"(#version 450

    layout( location = 0 ) in vec3 inColor;

    layout( location = 0 ) out vec4 outFragColor;

    void main( ) 
    {
      outFragColor = vec4( inColor, 1.0 );
    } )", frg_spv );
      

    auto vertexStage =
    /**/
    device->createShaderPipelineShaderStage(
      device->createShaderModule( vtx_spv ),
      vk::ShaderStageFlagBits::eVertex
    );

    auto fragmentStage = device->createShaderPipelineShaderStage(
      device->createShaderModule( frg_spv ),
      vk::ShaderStageFlagBits::eFragment
    );

    vk::VertexInputBindingDescription binding(
      0, sizeof( Vertex ), vk::VertexInputRate::eVertex );

    PipelineVertexInputStateCreateInfo vertexInput( binding, {
      vk::VertexInputAttributeDescription(
        0, 0, vk::Format::eR32G32B32Sfloat, offsetof( Vertex, position )
      ),
      vk::VertexInputAttributeDescription(
        1, 0, vk::Format::eR32G32B32Sfloat, offsetof( Vertex, color )
      )
    } );
    vk::PipelineInputAssemblyStateCreateInfo assembly( { },
      vk::PrimitiveTopology::eTriangleList, VK_FALSE );
    PipelineViewportStateCreateInfo viewport( 1, 1 );
    vk::PipelineRasterizationStateCreateInfo rasterization( { }, false,
      false, vk::PolygonMode::eFill, vk::CullModeFlagBits::eBack,
      vk::FrontFace::eClockwise, false, 0.0f, 0.0f, 0.0f, 1.0f );
    PipelineMultisampleStateCreateInfo multisample( vk::SampleCountFlagBits::e1,
      false, 0.0f, nullptr, false, false );
    vk::StencilOpState stencilOpState( vk::StencilOp::eKeep,
      vk::StencilOp::eKeep, vk::StencilOp::eKeep, vk::CompareOp::eAlways,
      0, 0, 0 );
    vk::PipelineDepthStencilStateCreateInfo depthStencil( { }, true, true,
      vk::CompareOp::eLessOrEqual, false, false, stencilOpState, stencilOpState,
      0.0f, 0.0f );
    vk::PipelineColorBlendAttachmentState colorBlendAttachment( false,
      vk::BlendFactor::eZero, vk::BlendFactor::eZero, vk::BlendOp::eAdd,
      vk::BlendFactor::eZero, vk::BlendFactor::eZero, vk::BlendOp::eAdd,
      vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG |
      vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA );
    PipelineColorBlendStateCreateInfo colorBlend( false, vk::LogicOp::eNoOp,
      colorBlendAttachment, { 1.0f, 1.0f, 1.0f, 1.0f } );
    PipelineDynamicStateCreateInfo dynamic( {
      vk::DynamicState::eViewport, vk::DynamicState::eScissor
    } );

    pipeline = device->createGraphicsPipeline( nullptr, { },
    { vertexStage, fragmentStage }, vertexInput, assembly, nullptr,
      viewport, rasterization, multisample, depthStencil, colorBlend,
      dynamic, pipelineLayout, renderPass );
  }

  {
    // Command buffer creation
    cmdBuffer = cmdPool->allocateCommandBuffer( );

    cmdBuffer->begin( );
    std::array<vk::ClearValue, 2> clearValues;
    std::array<float, 4> ccv = { 0.2f, 0.3f, 0.3f, 1.0f };
    clearValues[ 0 ].color = vk::ClearColorValue( ccv );
    clearValues[ 1 ].depthStencil = vk::ClearDepthStencilValue( 1.0f, 0 );

    cmdBuffer->beginRenderPass( renderPass, framebuffer,
      vk::Rect2D( { }, vk::Extent2D( width, height ) ), clearValues,
      vk::SubpassContents::eInline );

    cmdBuffer->setViewport( 0, vk::Viewport( 0.0f, 0.0f,
      ( float ) width, float( height ), 0.0f, 1.0f ) );
    cmdBuffer->setScissor( 0, vk::Rect2D( { }, { width, height } ) );

    cmdBuffer->bindGraphicsPipeline( pipeline );

    // Render scene
    cmdBuffer->bindVertexBuffer( 0, vertexBuffer, 0 );
    cmdBuffer->bindIndexBuffer( indexBuffer, 0, vk::IndexType::eUint32 );

    std::vector<glm::vec3> pos =
    {
      glm::vec3( -1.5f, 0.0f, -4.0f ),
      glm::vec3( 0.0f, 0.0f, -2.5f ),
      glm::vec3( 1.5f, 0.0f, -4.0f ),
    };
    for ( auto v : pos )
    {
      glm::mat4 mvpMatrix = glm::perspective(
        glm::radians( 60.0f ), ( float ) width / ( float ) height,
        0.1f, 256.0f ) * glm::translate( glm::mat4( 1.0f ), v );
      /*cmdBuffer->pushConstant( pipelineLayout,
        vk::ShaderStageFlagBits::eVertex, &mvpMatrix );*/
      cmdBuffer->pushConstants<glm::mat4>( pipelineLayout,
        vk::ShaderStageFlagBits::eVertex, 0, mvpMatrix );
      cmdBuffer->drawIndexed( 3, 1, 0, 0, 0 );
    }

    cmdBuffer->endRenderPass( );

    cmdBuffer->end( );

    queue->submit( cmdBuffer );
    device->waitIdle( );
  }

  {
    // Copy framebuffer image to host visible image

    // Create the linear tiled destination image to copy to 
    //    and to read the memory from
    auto dstImage = device->createImage( { }, vk::ImageType::e2D,
      vk::Format::eR8G8B8A8Unorm, vk::Extent3D( width, height, 1 ), 1, 1,
      vk::SampleCountFlagBits::e1, vk::ImageTiling::eLinear,
      vk::ImageUsageFlagBits::eTransferDst, vk::SharingMode::eExclusive, { },
      vk::ImageLayout::eUndefined,
      // Memory must be host visible to copy from
      vk::MemoryPropertyFlagBits::eHostVisible |
      vk::MemoryPropertyFlagBits::eHostCoherent
    );


    // Do the actual blit from the offscreen image to our host visible destination image
    auto copyCmd = cmdPool->allocateCommandBuffer( );
    copyCmd->begin( );

    pompeii::utilities::insertImageMemoryBarrier( copyCmd, dstImage,
      vk::AccessFlagBits( ), vk::AccessFlagBits::eTransferWrite,
      vk::ImageLayout::eUndefined, vk::ImageLayout::eTransferDstOptimal,
      vk::PipelineStageFlagBits::eTransfer, vk::PipelineStageFlagBits::eTransfer,
      vk::ImageSubresourceRange( vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1 )
    );

    // colorAttachment.image is already in VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, 
    //    and does not need to be transitioned

    vk::ImageCopy imageCopyRegion;
    imageCopyRegion.srcSubresource.aspectMask = vk::ImageAspectFlagBits::eColor;
    imageCopyRegion.srcSubresource.layerCount = 1;
    imageCopyRegion.dstSubresource.aspectMask = vk::ImageAspectFlagBits::eColor;
    imageCopyRegion.dstSubresource.layerCount = 1;
    imageCopyRegion.extent.width = width;
    imageCopyRegion.extent.height = height;
    imageCopyRegion.extent.depth = 1;

    copyCmd->copyImage(
      colorAttachment.image, vk::ImageLayout::eTransferSrcOptimal,
      dstImage, vk::ImageLayout::eTransferDstOptimal, imageCopyRegion );

    // Transition destination image to general layout, which is the 
    //    required layout for mapping the image memory later on
    pompeii::utilities::insertImageMemoryBarrier( copyCmd, dstImage,
      vk::AccessFlagBits::eTransferWrite, vk::AccessFlagBits::eMemoryRead,
      vk::ImageLayout::eTransferDstOptimal, vk::ImageLayout::eGeneral,
      vk::PipelineStageFlagBits::eTransfer, vk::PipelineStageFlagBits::eTransfer,
      vk::ImageSubresourceRange( vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1 )
    );

    copyCmd->end( );
    queue->submit( copyCmd );

    // Get layout of the image (including row pitch)
    // Get layout of the image (including row pitch)
    vk::ImageSubresource isr;
    isr.aspectMask = vk::ImageAspectFlagBits::eColor;
    vk::SubresourceLayout subResourceLayout;

    vk::Device dev = static_cast< vk::Device > ( *device );

    dev.getImageSubresourceLayout(
      static_cast< vk::Image >( *dstImage ), &isr, &subResourceLayout
    );

    // Map image memory so we can start copying from it
    const char* imagedata = ( const char* ) dev.mapMemory( dstImage->imageMemory, 0,
      VK_WHOLE_SIZE, { } );
    imagedata += subResourceLayout.offset;

    std::ofstream file( "headless.ppm", std::ios::out | std::ios::binary );

    // ppm header
    file << "P6\n" << width << "\n" << height << "\n" << 255 << "\n";

    // If source is BGR (destination is always RGB) and we can't use blit 
    //    (which does automatic conversion), we'll have to manually swizzle 
    //    color components
    bool colorSwizzle = false;
    // Check if source is BGR and needs swizzle
    std::vector< vk::Format > formatsBGR = {
      vk::Format::eB8G8R8A8Srgb, vk::Format::eB8G8R8A8Unorm,
      vk::Format::eB8G8R8A8Snorm
    };
    colorSwizzle = ( std::find( formatsBGR.begin( ), formatsBGR.end( ),
      colorFormat ) != formatsBGR.end( ) );

    // ppm binary pixel data
    for ( uint32_t y = 0; y < height; ++y )
    {
      unsigned int* row = ( unsigned int* ) imagedata;
      for ( uint32_t x = 0; x < width; ++x )
      {
        if ( colorSwizzle )
        {
          file.write( ( char* ) row + 2, 1 );
          file.write( ( char* ) row + 1, 1 );
          file.write( ( char* ) row, 1 );
        }
        else
        {
          file.write( ( char* ) row, 3 );
        }
        ++row;
      }
      imagedata += subResourceLayout.rowPitch;
    }
    file.close( );

    queue->waitIdle( );
  }
  return 0;
}