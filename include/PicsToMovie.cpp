// Quality versus Streaming
// ------------------------
// http://www.ezs3.com/public/What_bitrate_should_I_use_when_encoding_my_video_How_do_I_optimize_my_video_for_the_web.cfm
//
// If you are not sure what your target connection speed is and you are not shooting to stream your videos, but do not want an abundant
// amount of bandwidth cost from Amazon, one method that works really well in determining a good bitrate is the Kush Gauge. To find a decent
// bitrate simply multiply the target pixel count by the frame rate; then multiply the result by a factor of 1, 2 or 4, depending on the
// amount of motion in the video; and then multiply that result by 0.07 to get the bit rate in bps. Divide your result by 1,000 to get
// a kbps estimate or by 1,000,000 to get a Mbps estimate.
// 
// Kush gauge: pixel_count x motion_factor x 0.07 ÷ 1000 = bit rate in kbps
// Low motion 5fps, 1280 x 720: 1280 x 720 = 921,600 pixel count; 5fps; 1 = Low Motion
//   (921, 600 x 5) x 1 x 0.07 = 322, 560 bps / 1000 = 322 kbps bitrate
// Action shot 24fps, 1920 x 1080: 1920 x 1080 = 2,073,600 pixel count; 24fps; 4 = High Motion;
//   (2,073,600 x 24) x 4 x 0.07 = 13,934,592 bps / 1000 = 13,934 kbps bitrate
// 
// For streaming this example would be a bad idea, unless your target audience lives in South Korea. If you do decide to offer a super high
// quality file, be sure to also encode a lower quality streaming video for clients that may not have the fastest broadband available.
// When using this gauge with a variable bitrate (VBR), 75% of the resulting bitrate can be used for the minimum rate and 150% for the
// maximum rate. If you do not want to do the math this bitrate calculator will give you a similar result:
// http://www.adobe.com/jp/devnet/flash/apps/flv_bitrate_calculator.html 

// If you are completely lost right now and having trouble deciding what your video bitrate should be here are some starting points
// for you to experiment with for a streaming connection:
//   Resolution  Bitrate     Filesize
//    320x240     400 kbps    3MB / minute
//    480x270     700 kbps    5MB / minute
//   1024x576    1500 kbps   11MB / minute
//   1280x720    2500 kbps   19MB / minute
//   1920x1080   4000 kbps   30MB / minute
// In general though we recommend a bitrate of around 2 - 2.5 Mbps, which takes into account the average worldwide broadband connections.
// Once again we do however suggest you experiment with different settings and see how they work for you.
//
// References
// ----------
// https://msdn.microsoft.com/en-us/library/windows/desktop/aa371827(v=vs.85).aspx
// https://github.com/Microsoft/Windows-classic-samples/tree/master/Samples/Win7Samples/multimedia/mediafoundation
// https://msdn.microsoft.com/en-us/library/vs/alm/ms536290(v=vs.85).aspx
// https://msdn.microsoft.com/en-us/library/vs/alm/ms536295(v=vs.85).aspx
// https://msdn.microsoft.com/en-us/library/windows/desktop/dd183371(v=vs.85).aspx


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
   clear();
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
   if (videoEncodingProfile && !isok(pMediaTypeOut->SetUINT32(MF_MT_MPEG2_PROFILE, videoEncodingProfile), L"out:SetUINT32(MF_MT_MPEG2_PROFILE)"))
      return false;
   if (!isok(pMediaTypeOut->SetUINT32(MF_MT_AVG_BITRATE, kushRate(width, height, videoFps)), L"out:SetUINT32(MF_MT_AVG_BITRATE)"))
      return false;
   if (!isok(pMediaTypeOut->SetUINT32(MF_MT_INTERLACE_MODE, MFVideoInterlace_Progressive), L"out:SetUINT32(MF_MT_INTERLACE_MODE)"))
      return false;
   if (!isok(MFSetAttributeSize(pMediaTypeOut, MF_MT_FRAME_SIZE, width, height), L"out:MFSetAttributeSize(MF_MT_FRAME_SIZE)"))
      return false;
   if (!isok(MFSetAttributeRatio(pMediaTypeOut, MF_MT_FRAME_RATE, videoFps, 1), L"out:MFSetAttributeRatio(MF_MT_FRAME_RATE)"))
      return false;
   if (!isok(MFSetAttributeRatio(pMediaTypeOut, MF_MT_PIXEL_ASPECT_RATIO, 1, 1), L"out:MFSetAttributeRatio(MF_MT_PIXEL_ASPECT_RATIO)"))
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
   if (!isok(pMediaTypeIn->SetUINT32(MF_MT_INTERLACE_MODE, MFVideoInterlace_Progressive), L"in:SetUINT32(MF_MT_INTERLACE_MODE)"))
      return false;
   if (!isok(MFSetAttributeSize(pMediaTypeIn, MF_MT_FRAME_SIZE, width, height), L"in:MFSetAttributeSize(MF_MT_FRAME_SIZE)"))
      return false;
   if (!isok(MFSetAttributeRatio(pMediaTypeIn, MF_MT_FRAME_RATE, videoFps, 1), L"in:MFSetAttributeRatio(MF_MT_FRAME_RATE)"))
      return false;
   if (!isok(MFSetAttributeRatio(pMediaTypeIn, MF_MT_PIXEL_ASPECT_RATIO, 1, 1), L"in:MFSetAttributeRatio(MF_MT_PIXEL_ASPECT_RATIO)"))
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

void PicsToMovie::clear()
{
   _rtStart = 0;
   _frame.clear();

   if (_isStarted)
      MFShutdown();
   _isStarted = false;

   if (_pSinkWriter)
      _pSinkWriter.Release();
   _pSinkWriter = nullptr;
}

bool PicsToMovie::initialize(const wchar_t* outfile)
{
   clear();

   lastError = 0;
   frameCount = 0;
   frameWidth = 0;
   frameHeight = 0;

   _isStarted = isok(MFStartup(MF_VERSION), L"MFStartup(MF_VERSION)");
   if (!_isStarted)
      return false;

   CComPtr<IMFAttributes> mfattributes;
   if (!isok(MFCreateAttributes(&mfattributes, 4), L"MFCreateAttributes()"))
      return false;
   if (!isok(mfattributes->SetGUID(MF_TRANSCODE_CONTAINERTYPE, outputContainer), L"SetGUID(MF_TRANSCODE_CONTAINERTYPE)"))
      return false;;
   if (!isok(MFCreateSinkWriterFromURL(outfile, nullptr, mfattributes, &_pSinkWriter), L"MFCreateSinkWriterFromURL()"))
      return false;

   return true;
}

bool PicsToMovie::finalize()
{
   bool ok = _pSinkWriter ? isok(_pSinkWriter->Finalize(), L"Finalize()") : true;
   clear();
   return ok;
}


bool PicsToMovie::configure(unsigned width, unsigned height)
{
   if (!_pSinkWriter)
      return isok(HRESULT_FROM_WIN32(ERROR_INVALID_HANDLE_STATE), L"must call initialize() first");

   // Set the output media type.
   CComPtr<IMFMediaType> pMediaTypeOut;
   if (!isok(MFCreateMediaType(&pMediaTypeOut), L"out:MFCreateMediaType()"))
      return false;
   if (!isok(pMediaTypeOut->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Video), L"out:SetGUID(MF_MT_MAJOR_TYPE)"))
      return false;
   if (!isok(pMediaTypeOut->SetGUID(MF_MT_SUBTYPE, videoEncodingFormat), L"out:SetGUID(MF_MT_SUBTYPE)"))
      return false;
   if (videoEncodingProfile && !isok(pMediaTypeOut->SetUINT32(MF_MT_MPEG2_PROFILE, videoEncodingProfile), L"out:SetUINT32(MF_MT_MPEG2_PROFILE)"))
      return false;
   if (!isok(pMediaTypeOut->SetUINT32(MF_MT_AVG_BITRATE, kushRate(width, height, videoFps)), L"out:SetUINT32(MF_MT_AVG_BITRATE)"))
      return false;
   if (!isok(pMediaTypeOut->SetUINT32(MF_MT_INTERLACE_MODE, MFVideoInterlace_Progressive), L"out:SetUINT32(MF_MT_INTERLACE_MODE)"))
      return false;
   if (!isok(MFSetAttributeSize(pMediaTypeOut, MF_MT_FRAME_SIZE, width, height), L"out:MFSetAttributeSize(MF_MT_FRAME_SIZE)"))
      return false;
   if (!isok(MFSetAttributeRatio(pMediaTypeOut, MF_MT_FRAME_RATE, videoFps, 1), L"out:MFSetAttributeRatio(MF_MT_FRAME_RATE)"))
      return false;
   if (!isok(MFSetAttributeRatio(pMediaTypeOut, MF_MT_PIXEL_ASPECT_RATIO, 1, 1), L"out:MFSetAttributeRatio(MF_MT_PIXEL_ASPECT_RATIO)"))
      return false;

   if (!isok(_pSinkWriter->AddStream(pMediaTypeOut, &_streamIndex), L"out:AddStream()"))
      return false;

   // Set the input media type.
   CComPtr<IMFMediaType> pMediaTypeIn;
   if (!isok(MFCreateMediaType(&pMediaTypeIn), L"in:MFCreateMediaType()"))
      return false;
   if (!isok(pMediaTypeIn->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Video), L"in:SetGUID(MF_MT_MAJOR_TYPE)"))
      return false;
   if (!isok(pMediaTypeIn->SetGUID(MF_MT_SUBTYPE, videoInputFormat), L"in:SetGUID(MF_MT_SUBTYPE)"))
      return false;
   if (!isok(pMediaTypeIn->SetUINT32(MF_MT_INTERLACE_MODE, MFVideoInterlace_Progressive), L"in:SetUINT32(MF_MT_INTERLACE_MODE)"))
      return false;
   if (!isok(MFSetAttributeSize(pMediaTypeIn, MF_MT_FRAME_SIZE, width, height), L"in:MFSetAttributeSize(MF_MT_FRAME_SIZE)"))
      return false;
   if (!isok(MFSetAttributeRatio(pMediaTypeIn, MF_MT_FRAME_RATE, videoFps, 1), L"in:MFSetAttributeRatio(MF_MT_FRAME_RATE)"))
      return false;
   if (!isok(MFSetAttributeRatio(pMediaTypeIn, MF_MT_PIXEL_ASPECT_RATIO, 1, 1), L"in:MFSetAttributeRatio(MF_MT_PIXEL_ASPECT_RATIO)"))
      return false;
   if (!isok(_pSinkWriter->SetInputMediaType(_streamIndex, pMediaTypeIn, nullptr), L"in:SetInputMediaType()"))
      return false;

   // Tell the sink writer to start accepting data.
   if (!isok(_pSinkWriter->BeginWriting(), L"BeginWriting()"))
      return false;

   _frame.resize(width * height);

   frameWidth = width;
   frameHeight = height;

   return true;
}

bool PicsToMovie::addFrame(const wchar_t* path)
{
   if (!_pSinkWriter)
      return isok(HRESULT_FROM_WIN32(ERROR_INVALID_HANDLE_STATE), L"must call initialize() first");

   BitmapHolder image;

   if (debug)
      log((std::wstring(L"Reading ") + path).c_str());

   if (!image.loadImage(path))
      return fail(GetLastError(), (std::wstring(L"loadImage(") + path + L")").c_str());

   if (_frame.empty())
   {
      SIZE isize = image.getSize();
      if (!configure(unsigned(isize.cx), unsigned(isize.cy)))
         return false;
   }

   if (!image.copyPixels(_frame.size(), _frame.data()))
      return fail(GetLastError(), (std::wstring(L"copyPixels(") + path + L")").c_str());

   if (!writeFrame(_pSinkWriter, _streamIndex, _rtStart, _frame.data(), frameWidth, frameHeight))
      return false;

   _rtStart += videoFrameDuration();
   ++frameCount;

   return true;
}

bool PicsToMovie::addFrameWildcard(const wchar_t* wildcard)
{
   WIN32_FIND_DATA data{};
   const AutoFindClose hFind(FindFirstFile(wildcard, &data));
   if (INVALID_HANDLE_VALUE == hFind)
      return fail(GetLastError(), (std::wstring(L"FindFirstFile(") + wildcard + L")").c_str());

   while (0==wcscmp(data.cFileName, L".") || 0==wcscmp(data.cFileName, L".."))
   {
      if (!FindNextFile(hFind, &data))
         return fail(GetLastError(), (std::wstring(L"FindNextFile(") + wildcard + L")").c_str());
   }

   const wchar_t* plast = wcsrchr(wildcard, L'\\');
   std::wstring folder(wildcard, plast ? plast - wildcard : 0);
   if (!folder.empty())
      folder.push_back(L'\\');

   bool ok{};
   do
   {
      ok = false;
      if (!addFrame((folder + data.cFileName).c_str()))
         break;
      ok = true;
   } while (FindNextFile(hFind, &data));

   DWORD error = GetLastError();
   if (ok && ERROR_NO_MORE_FILES != error)
      return fail(error, (std::wstring(L"FindNextFile(") + wildcard + L")").c_str());

   return true;
}

bool PicsToMovie::makeMovie(const wchar_t* wildcard, const wchar_t* outfile)
{
   if (!initialize(outfile))
      return false;

   bool ok = addFrameWildcard(wildcard);
   HRESULT error = lastError;
   bool of = finalize();
   if (!ok)
      lastError = error;

   return ok && of;
}
