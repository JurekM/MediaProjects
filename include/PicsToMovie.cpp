#include <PicsToMovie.h>
#include <BitmapHolder.h>


class PicsToMovie::AutoFindClose
{
   HANDLE hFind{};
public:
   explicit AutoFindClose(HANDLE h)
      : hFind(h)
   {
   }

   ~AutoFindClose()
   {
      DWORD error = GetLastError();
      clear();
      SetLastError(error);
   }

   void clear()
   {
      if (hFind)
         FindClose(hFind);
      hFind = nullptr;
   }

   operator HANDLE() const
   {
      return hFind;
   }
};

class PicsToMovie::AutoMFShutdown
{
public:
   ~AutoMFShutdown()
   {
      MFShutdown();
   }
};

PicsToMovie::PicsToMovie()
{
}

PicsToMovie::~PicsToMovie()
{
}

bool PicsToMovie::initializeSinkWriter(IMFSinkWriter** ppWriter, DWORD* pStreamIndex,
                                       const wchar_t* path,
                                       unsigned width, unsigned height)
{
   *ppWriter = nullptr;
   *pStreamIndex = 0;

   //HRESULT hr{};

   CComPtr<IMFAttributes> mfattributes;
   if (SUCCEEDED(MFCreateAttributes(&mfattributes, 4)))
   {
      mfattributes->SetGUID(MF_TRANSCODE_CONTAINERTYPE, outputContainer);
   }

   CComPtr<IMFSinkWriter> pSinkWriter;
   if (!isok(MFCreateSinkWriterFromURL(path, nullptr, mfattributes, &pSinkWriter), L"MFCreateSinkWriterFromURL()"))
      return false;

   // Set the output media type.
   CComPtr<IMFMediaType> pMediaTypeOut;
   if (!isok(MFCreateMediaType(&pMediaTypeOut), L"out:MFCreateMediaType()"))
      return false;
   if (!isok(pMediaTypeOut->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Video), L"out:SetGUID(MF_MT_MAJOR_TYPE)"))
      return false;
   if (!isok(pMediaTypeOut->SetGUID(MF_MT_SUBTYPE, videoEncodingFormat), L"out:SetGUID(MF_MT_SUBTYPE)"))
      return false;
   if (videoEncodingProfile && !isok(pMediaTypeOut->SetUINT32(MF_MT_MPEG2_PROFILE, videoEncodingProfile),
                                     L"out:SetUINT32(MF_MT_MPEG2_PROFILE)"))
      return false;
   if (!isok(pMediaTypeOut->SetUINT32(MF_MT_AVG_BITRATE, kushRate(width, height, videoFps)),
             L"out:SetUINT32(MF_MT_AVG_BITRATE)"))
      return false;
   if (!isok(pMediaTypeOut->SetUINT32(MF_MT_INTERLACE_MODE, MFVideoInterlace_Progressive),
             L"out:SetUINT32(MF_MT_INTERLACE_MODE)"))
      return false;
   if (!isok(MFSetAttributeSize(pMediaTypeOut, MF_MT_FRAME_SIZE, width, height),
             L"out:MFSetAttributeSize(MF_MT_FRAME_SIZE)"))
      return false;
   if (!isok(MFSetAttributeRatio(pMediaTypeOut, MF_MT_FRAME_RATE, videoFps, 1),
             L"out:MFSetAttributeRatio(MF_MT_FRAME_RATE)"))
      return false;
   if (!isok(MFSetAttributeRatio(pMediaTypeOut, MF_MT_PIXEL_ASPECT_RATIO, 1, 1),
             L"out:MFSetAttributeRatio(MF_MT_PIXEL_ASPECT_RATIO)"))
      return false;

   DWORD streamIndex{};
   if (!isok(pSinkWriter->AddStream(pMediaTypeOut, &streamIndex), L"out:AddStream()"))
      return false;

   // Set the input media type.
   CComPtr<IMFMediaType> pMediaTypeIn;
   if (!isok(MFCreateMediaType(&pMediaTypeIn), L"in:MFCreateMediaType()"))
      return false;
   if (!isok(pMediaTypeIn->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Video), L"in:SetGUID(MF_MT_MAJOR_TYPE)"))
      return false;
   if (!isok(pMediaTypeIn->SetGUID(MF_MT_SUBTYPE, videoInputFormat), L"in:SetGUID(MF_MT_SUBTYPE)"))
      return false;
   if (!isok(pMediaTypeIn->SetUINT32(MF_MT_INTERLACE_MODE, MFVideoInterlace_Progressive),
             L"in:SetUINT32(MF_MT_INTERLACE_MODE)"))
      return false;
   if (!isok(MFSetAttributeSize(pMediaTypeIn, MF_MT_FRAME_SIZE, width, height),
             L"in:MFSetAttributeSize(MF_MT_FRAME_SIZE)"))
      return false;
   if (!isok(MFSetAttributeRatio(pMediaTypeIn, MF_MT_FRAME_RATE, videoFps, 1),
             L"in:MFSetAttributeRatio(MF_MT_FRAME_RATE)"))
      return false;
   if (!isok(MFSetAttributeRatio(pMediaTypeIn, MF_MT_PIXEL_ASPECT_RATIO, 1, 1),
             L"in:MFSetAttributeRatio(MF_MT_PIXEL_ASPECT_RATIO)"))
      return false;
   if (!isok(pSinkWriter->SetInputMediaType(streamIndex, pMediaTypeIn, nullptr), L"in:SetInputMediaType()"))
      return false;

   // Tell the sink writer to start accepting data.
   if (!isok(pSinkWriter->BeginWriting(), L"BeginWriting()"))
      return false;

   // Return the pointer to the caller.
   *ppWriter = pSinkWriter;
   (*ppWriter)->AddRef();
   *pStreamIndex = streamIndex;

   return true;
}

bool PicsToMovie::writeFrame(IMFSinkWriter* pWriter, DWORD streamIndex, const LONGLONG& rtStart, COLORREF* frame,
                             unsigned width, unsigned height)
{
   const LONG cbWidth = sizeof(COLORREF) * width;
   const DWORD cbBuffer = cbWidth * height;

   // Create a new memory buffer.
   CComPtr<IMFMediaBuffer> pBuffer{};
   if (!isok(MFCreateMemoryBuffer(cbBuffer, &pBuffer), L"MFCreateMemoryBuffer()"))
      return false;

   // Lock the buffer and copy the video frame to the buffer.
   BYTE* pData = nullptr;
   if (!isok(pBuffer->Lock(&pData, nullptr, nullptr), L"buffer::Lock()"))
      return false;

   if (!isok(MFCopyImage(
                pData, // Destination buffer.
                cbWidth, // Destination stride.
                reinterpret_cast<BYTE*>(frame), // First row in source image.
                cbWidth, // Source stride.
                cbWidth, // Image width in bytes.
                height // Image height in pixels.
             ), L"MFCopyImage()"))
      return false;

   pBuffer->Unlock();

   // Set the data length of the buffer.
   if (!isok(pBuffer->SetCurrentLength(cbBuffer), L"buffer::SetCurrentLength()"))
      return false;

   // Create a media sample and add the buffer to the sample.
   CComPtr<IMFSample> pSample;
   if (!isok(MFCreateSample(&pSample), L"MFCreateSample()"))
      return false;
   if (!isok(pSample->AddBuffer(pBuffer), L"AddBuffer()"))
      return false;

   // Set the time stamp and the duration.
   if (!isok(pSample->SetSampleTime(rtStart), L"SetSampleTime()"))
      return false;
   if (!isok(pSample->SetSampleDuration(videoFrameDuration()), L"SetSampleDuration()"))
      return false;

   // Send the sample to the Sink Writer.
   if (!isok(pWriter->WriteSample(streamIndex, pSample), L"WriteSample()"))
      return false;

   return true;
}

bool PicsToMovie::makeMovie(const wchar_t* wildcard, const wchar_t* outfile)
{
   frameCount = 0;

   WIN32_FIND_DATA data{};
   const AutoFindClose hFind(FindFirstFile(wildcard, &data));

   if (INVALID_HANDLE_VALUE == hFind)
      return fail(GetLastError(), (std::wstring(L"FindFirstFile(") + wildcard + L")").c_str());

   while (data.cFileName[0] == L'.' && (!data.cFileName[1] || data.cFileName[1] == L'.' && !data.cFileName[2]))
   {
      if (!FindNextFile(hFind, &data))
         return fail(GetLastError(), L"Skipping dot folders failed:");
   }

   const wchar_t* plast = wcsrchr(wildcard, L'\\');
   std::wstring folder(wildcard, plast ? plast - wildcard : 0);
   if (!folder.empty())
      folder.push_back(L'\\');

   BitmapHolder image;
   if (debug)
      log((std::wstring(L"Reading ") + folder + data.cFileName).c_str());
   if (!image.loadImage((std::wstring(folder) + data.cFileName).c_str()))
      return fail(GetLastError(), (std::wstring(L"loadImage(") + data.cFileName + L")").c_str());

   SIZE isize = image.getSize();
   frameWidth = unsigned(isize.cx);
   frameHeight = unsigned(isize.cy);

   std::vector<COLORREF> frame;
   frame.resize(frameWidth * frameHeight);

   if (!isok(MFStartup(MF_VERSION), L"MFStartup(MF_VERSION)"))
      return false;

   AutoMFShutdown amfs;

   CComPtr<IMFAttributes> mfattributes{};
   if (!isok(MFCreateAttributes(&mfattributes, 4), L""))
      return false;
   mfattributes->SetGUID(MF_TRANSCODE_CONTAINERTYPE, outputContainer);

   DWORD stream{};
   CComPtr<IMFSinkWriter> pSinkWriter{};
   if (!initializeSinkWriter(&pSinkWriter, &stream, outfile, frameWidth, frameHeight))
      return false;

   // Send frames to the sink writer.
   LONGLONG rtStart = 0;
   bool success{true};

   while (true)
   {
      if (!image.copyPixels(frame.size(), frame.data()))
      {
         success = fail(GetLastError(), (std::wstring(L"copyPixels(") + data.cFileName + L")").c_str());
         break;
      }

      success = writeFrame(pSinkWriter, stream, rtStart, frame.data(), frameWidth, frameHeight);
      if (!success)
         break;

      rtStart += videoFrameDuration();
      ++frameCount;

      if (!FindNextFile(hFind, &data))
      {
         DWORD error = GetLastError();
         if (ERROR_NO_MORE_FILES == error)
            fail(0, L"All files processed");
         else
            success = fail(error, L"FindNextFile():");
         break;
      }

      if (debug)
         log((std::wstring(L"Reading ") + folder + data.cFileName).c_str());
      if (!image.loadImage((folder + data.cFileName).c_str()))
      {
         success = fail(GetLastError(), (std::wstring(L"loadImage(") + data.cFileName + L")").c_str());
         break;
      }
   }

   pSinkWriter->Finalize();

   return success;
}
