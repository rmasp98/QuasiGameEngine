/*------------------------------------------------------------------------------
   Copyright (C) 2018 Ross Maspero <rossmaspero@gmail.com>
   All rights reserved.

   This software is licensed as described in the file LICENSE.md, which
   you should have received as part of this distribution.

   Author: Ross Maspero <rossmaspero@gmail.com>
------------------------------------------------------------------------------*/

#include "resource/image_loader.h"
#include "resource/texture.h"

#include "utils/logging/log_capture.h"

#include <FreeImage.h>

namespace quasi_game_engine {

ImageLoader::ImageLoader() {
  // True means extensions disabled
  FreeImage_Initialise(true);
}


ImageLoader::~ImageLoader() { FreeImage_DeInitialise(); }


Resource* ImageLoader::Load(Asset asset) {
  LOG(TRACE, RESOURCE) 
      << "Loading image from '" << asset.file_location << "'";

  FREE_IMAGE_FORMAT file_type =
      FreeImage_GetFileType(asset.file_location, 0);
  if (file_type == FIF_UNKNOWN)
    file_type = FreeImage_GetFIFFromFilename(asset.file_location);

  if ((file_type != FIF_UNKNOWN) && (FreeImage_FIFSupportsReading(file_type))) {
    FIBITMAP* dib = FreeImage_Load(file_type, asset.file_location, 0);
    if (dib) {
      int bits_per_pixel = FreeImage_GetBPP(dib);

      // Convert to 32 bit pixelMap (TODO: figure out why?)
      FIBITMAP* dib32;
      if (bits_per_pixel == 32)
        dib32 = dib;
      else
        dib32 = FreeImage_ConvertTo32Bits(dib);

      unsigned char* pixel_map = FreeImage_GetBits(dib32);
      int width = FreeImage_GetWidth(dib32);
      int height = FreeImage_GetHeight(dib32);

      Resource* new_resource = new Texture(pixel_map, width, height, asset);
      return new_resource;
    }
  }

  LOG(ERROR, RESOURCE) 
      << "Failed to load " << asset.file_location
      << ". Either FreeImage does not support this filetype"
      << ", the file is corrupt or does not exist";

  return NULL;
}

}  // namespace quasi_game_engine
