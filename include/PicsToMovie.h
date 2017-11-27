#pragma once

class PicsToMovie
{
public:
   PicsToMovie();
   ~PicsToMovie();

   struct IPicsToMovieLogger
   {
      virtual ~IPicsToMovieLogger() = default;
      virtual void log(const wchar_t* message) = 0;
   };

   //Kush gauge: pixel_count x motion_factor x 0.07 ÷ 1000 = bit rate in kbps
   // Low motion 5fps, 1280 x 720: 1280 x 720 = 921,600 pixel count; 5fps; 1 = Low Motion
   //   (921, 600 x 5) x 1 x 0.07 = 322, 560 bps / 1000 = 322 kbps bitrate
   // Action shot 24fps, 1920 x 1080: 1920 x 1080 = 2,073,600 pixel count; 24fps; 4 = High Motion;
   //   (2,073,600 x 24) x 4 x 0.07 = 13,934,592 bps / 1000 = 13,934 kbps bitrate

   enum class KushMotionFactor
   {
      Low = 1,
      Medium = 2,
      High = 4,
      Ultra = 32,
   };

   static unsigned kushRate(unsigned width, unsigned height, unsigned fps,
                            KushMotionFactor speed = KushMotionFactor::Medium)
   {
      return max(500000, width * height * (10 < fps ? fps : 10) * unsigned(speed) * 7 / 100);
   }

   void setLogger(IPicsToMovieLogger* logger)
   {
      _logger = logger;
   }

   UINT64 videoFrameDuration() const { return 10 * 1000 * 1000 / videoFps; }

   bool initializeSinkWriter(IMFSinkWriter** ppWriter, DWORD* pStreamIndex, const wchar_t* path,
                             unsigned width, unsigned height);

   bool writeFrame(IMFSinkWriter* pWriter, DWORD streamIndex, const LONGLONG& rtStart, COLORREF* frame,
                   unsigned width, unsigned height);

   bool makeMovie(const wchar_t* wildcard, const wchar_t* outfile);

   void log(const wchar_t* message) const
   {
      IPicsToMovieLogger* logger{_logger};
      if (logger)
         logger->log(message);
   }

   bool isok(HRESULT hr, const wchar_t* what)
   {
      if (FAILED(hr))
         return fail(hr, what);
      lastError = S_OK;
      return true;;
   }

   bool fail(HRESULT hr, const wchar_t* message)
   {
      if (hr < 0xFFFF && hr != 0)
         hr = HRESULT_FROM_WIN32(hr);
      lastError = hr;
      if (_logger)
      {
         wchar_t hex[16]{};
         _itow_s(hr, hex, 16);
         log((std::wstring(message ? message : L"FAILED:") + L" 0x" + hex).c_str());
      }
      return false;
   }

   bool debug{};

   HRESULT lastError{};
   unsigned frameCount{};
   unsigned frameWidth{};
   unsigned frameHeight{};

   GUID videoEncodingFormat{MFVideoFormat_H264};
   UINT32 videoEncodingProfile{eAVEncH264VProfile_High};
   GUID outputContainer{MFTranscodeContainerType_MPEG4};
   unsigned videoFps{15};
   GUID videoInputFormat = MFVideoFormat_RGB32;

protected:
   class AutoFindClose;
   class AutoMFShutdown;
   IPicsToMovieLogger* _logger{};
};
