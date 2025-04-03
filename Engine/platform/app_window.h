#pragma once

#include "app_common_headers.h"

DEFINE_TYPED_ID(appWINDOW_ID);


class appWINDOW
{
public:
   constexpr appWINDOW() = default;

   constexpr explicit appWINDOW(appWINDOW_ID id)
      : id(id)
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

   mVECTOR4_U32 GetSize() const;
   U32 GetWidth() const;
   U32 GetHeight() const;
   void* GetHandle() const;

   bool IsFullscreen() const;
   bool IsClosed() const;

   void SetCaption(const wchar_t* caption) const;
   void SetFullscreen(bool isFullscreen) const;

   void Resize(U32 width = -1, U32 height = -1) const;
   
private:
   appWINDOW_ID id{ ID_INVALID_ID };
};