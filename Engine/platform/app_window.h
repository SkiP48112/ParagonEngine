#pragma once

#include "app_common_headers.h"

DEFINE_TYPED_ID(appWINDOW_ID);


class appWINDOW
{
public:
   constexpr explicit appWINDOW(appWINDOW_ID id)
      : id(id)
   {
   }

   constexpr appWINDOW()
      : id(ID_INVALID_ID)
   {
   }

   constexpr appWINDOW_ID GetID() const
   {
      return id;
   }

   constexpr bool IsValid() const
   {
      return idIsValid(id);
   }

   const mVECTOR4_U32 GetSize() const;
   const U32 GetWidth() const;
   const U32 GetHeight() const;

   bool IsFullscreen() const;
   bool IsClosed() const;

   void SetCaption(const char* caption) const;
   void SetFullscreen(bool isFullscreen) const;

   void Resize(U32 width, U32 height) const;
   void* Handle() const;
   
private:
   appWINDOW_ID id{ ID_INVALID_ID };
};