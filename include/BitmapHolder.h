#pragma once

class BitmapHolder
{
public:
   BitmapHolder()
   {
   }

   ~BitmapHolder()
   {
      clear();
   }

   void clear()
   {
      if (h_bitmap)
         DeleteObject(h_bitmap);
      h_bitmap = nullptr;
      if (p_bitmap)
         delete p_bitmap;
      p_bitmap = nullptr;
   }

   bool loadImage(const wchar_t* path)
   {
      clear();
      p_bitmap = Bitmap::FromFile(path, true);
      if (!p_bitmap)
         return false;
      return Ok == p_bitmap->GetHBITMAP(Color(255, 0, 0, 0), &h_bitmap);
   }

   SIZE getSize() const
   {
      unsigned char buffer[256]{};
      if (!h_bitmap || 0 == GetObject(h_bitmap, sizeof(buffer), &buffer))
         return SIZE{};
      BITMAP& bm = *reinterpret_cast<BITMAP*>(&buffer);
      return SIZE{bm.bmWidth, bm.bmHeight};
   }

   bool copyPixels(size_t pixcount, COLORREF* frame) const
   {
      unsigned char buffer[256]{};
      if (!h_bitmap || 0 == GetObject(h_bitmap, sizeof(buffer), &buffer))
         return false;
      BITMAP& bm = *reinterpret_cast<BITMAP*>(&buffer);
      if (pixcount < unsigned(bm.bmWidth * bm.bmHeight))
      {
         SetLastError(ERROR_INSUFFICIENT_BUFFER);
         return false;
      }
      COLORREF* pixels = static_cast<COLORREF*>(bm.bmBits);
      for (LONG row = 0; row < bm.bmHeight; row++)
      {
         for (LONG col = 0; col < bm.bmWidth; col++)
         {
            frame[row * bm.bmWidth + col] = pixels[row * bm.bmWidth + col];
         }
      }
      return true;
   }

   Bitmap* p_bitmap{};
   HBITMAP h_bitmap{};
};
