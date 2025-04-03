#pragma once

#include "app_common_headers.h"
#include "..\platform\app_window.h"


DEFINE_TYPED_ID(grSURFACE_ID);


class grSURFACE
{
public:
   constexpr explicit grSURFACE() = default;

   constexpr explicit grSURFACE(grSURFACE_ID id)
      : id(id)
   {
   }

   constexpr grSURFACE_ID GetID() const
   {
      return id;
   }

   constexpr bool IsValid() const
   {
      return idIsValid(id);
   }

   void Resize(U32 width, U32 height) const;
   U32 GetWidth() const;
   U32 GetHeight() const;
   void Render() const;

private:
   grSURFACE_ID id{ ID_INVALID_ID };
};


struct grRENDER_SURFACE
{
   appWINDOW window{};
   grSURFACE surface{};
};


enum class grGRAPHICS_PLATFORM : U32
{
   GP_DIRECT3D_12 = 0,
};


bool grInitialize(grGRAPHICS_PLATFORM platform);
void grShutdown();

grSURFACE grCreateSurface(appWINDOW window);
void grRemoveSurface(grSURFACE_ID id);