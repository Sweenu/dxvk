#pragma once

#include "dxvk_buffer.h"
#include "dxvk_image.h"
#include "dxvk_sampler.h"

namespace dxvk {
  
  /**
   * \brief Unbound resources
   * 
   * Creates dummy resources that will be used
   * for descriptor sets when the client API did
   * not bind a compatible resource to a slot.
   */
  class DxvkUnboundResources {
    
  public:
    
    DxvkUnboundResources(DxvkDevice* dev);
    ~DxvkUnboundResources();
    
    /**
     * \brief Dummy buffer descriptor
     * 
     * Points to a tiny buffer with undefined
     * values. Do not access this buffer.
     * \returns Dummy buffer descriptor
     */
    VkDescriptorBufferInfo bufferDescriptor() const {
      auto slice = m_buffer->slice();
      
      VkDescriptorBufferInfo result;
      result.buffer = slice.handle();
      result.offset = slice.offset();
      result.range  = slice.length();
      return result;
    }
    
    /**
     * \brief Dummy buffer view
     * 
     * Returns an \c R32_UINT view into the
     * dummy buffer, which will contain one
     * element with undefined value.
     * \returns Dummy buffer view
     */
    VkBufferView bufferViewDescriptor() const {
      return m_bufferView->handle();
    }
    
    /**
     * \brief Dummy sampler descriptor
     * 
     * Points to a sampler which was created with
     * reasonable default values. Client APIs may
     * still require different behaviour.
     * \returns Dummy sampler descriptor
     */
    VkDescriptorImageInfo samplerDescriptor() const {
      VkDescriptorImageInfo result;
      result.sampler     = m_sampler->handle();
      result.imageView   = VK_NULL_HANDLE;
      result.imageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
      return result;
    }
    
    /**
     * \brief Dummy image view descriptor
     * 
     * Points to an image view which, instead of
     * reading image data, will return zeroes for
     * all components unconditionally.
     * \returns Dummy image view descriptor
     */
    VkDescriptorImageInfo imageViewDescriptor(VkImageViewType type) const {
      auto view = getImageView(type);
      
      VkDescriptorImageInfo result;
      result.sampler     = VK_NULL_HANDLE;
      result.imageView   = view->handle();
      result.imageLayout = view->imageInfo().layout;
      return result;
    }
    
  private:
    
    Rc<DxvkSampler> m_sampler;
    
    Rc<DxvkBuffer>     m_buffer;
    Rc<DxvkBufferView> m_bufferView;
    
    Rc<DxvkImage> m_image1D;
    Rc<DxvkImage> m_image2D;
    Rc<DxvkImage> m_image3D;
    
    Rc<DxvkImageView> m_view1D;
    Rc<DxvkImageView> m_view1DArr;
    Rc<DxvkImageView> m_view2D;
    Rc<DxvkImageView> m_view2DArr;
    Rc<DxvkImageView> m_viewCube;
    Rc<DxvkImageView> m_viewCubeArr;
    Rc<DxvkImageView> m_view3D;
    
    Rc<DxvkSampler> createSampler(DxvkDevice* dev);
    
    Rc<DxvkBuffer> createBuffer(DxvkDevice* dev);
    
    Rc<DxvkBufferView> createBufferView(
            DxvkDevice*     dev,
      const Rc<DxvkBuffer>& buffer);
    
    Rc<DxvkImage> createImage(
            DxvkDevice*     dev,
            VkImageType     type,
            uint32_t        layers);
    
    Rc<DxvkImageView> createImageView(
            DxvkDevice*     dev,
      const Rc<DxvkImage>&  image,
            VkImageViewType type,
            uint32_t        layers);
    
    const DxvkImageView* getImageView(
            VkImageViewType type) const;
    
  };
  
}