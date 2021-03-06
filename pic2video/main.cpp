#include "stdafx.h"
#include "PicsToMovie.h"

// https://msdn.microsoft.com/en-us/library/windows/desktop/aa371827(v=vs.85).aspx
// https://github.com/Microsoft/Windows-classic-samples/tree/master/Samples/Win7Samples/multimedia/mediafoundation
// https://msdn.microsoft.com/en-us/library/vs/alm/ms536290(v=vs.85).aspx
// https://msdn.microsoft.com/en-us/library/vs/alm/ms536295(v=vs.85).aspx
// https://msdn.microsoft.com/en-us/library/windows/desktop/dd183371(v=vs.85).aspx

// http://www.ezs3.com/public/What_bitrate_should_I_use_when_encoding_my_video_How_do_I_optimize_my_video_for_the_web.cfm
// Quality versus Streaming
// ------------------------
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

class Logger : public PicsToMovie::IPicsToMovieLogger
{
public:
   virtual ~Logger() = default;

   void log(const wchar_t* message) override
   {
      ++counter;
      wprintf(L"\n%s", message);
   }

   unsigned counter{};
};


int makeMovie(const wchar_t* wildcard, const wchar_t* output, unsigned fps, bool debug)
{
   PicsToMovie p2m;
   p2m.debug = debug;
   Logger logger;
   p2m.setLogger(&logger);
   if (fps)
      p2m.videoFps = fps;

   //printf("\nOutput: %ws", output);
   //printf("\nStart:  %ws", wildcard);
   printf("\n");

   bool ok = p2m.makeMovie(wildcard, output);

   WIN32_FILE_ATTRIBUTE_DATA info{};
   GetFileAttributesEx(output, GET_FILEEX_INFO_LEVELS::GetFileExInfoStandard, &info);
   ULARGE_INTEGER uli{0};
   uli.LowPart = info.nFileSizeLow;
   uli.HighPart = info.nFileSizeHigh;

   if (logger.counter)
      printf("\n");
   printf("\nFinal status  = 0x%08X %ls", p2m.lastError, ok ? L"Success" : L"FAILED");
   printf("\nVideo size    = %ux%u", p2m.frameWidth, p2m.frameHeight);
   printf("\nVideo FPS     = %u", p2m.videoFps);
   printf("\nFrame time    = %I64u", p2m.videoFrameDuration());
   printf("\nFrame count   = %u", p2m.frameCount);
   printf("\nDuration(m:s) = %u:%02u", p2m.frameCount / p2m.videoFps / 60, (p2m.frameCount / p2m.videoFps) % 60);
   printf("\nFile size KB  = %u", unsigned(uli.QuadPart/1024));
   printf("\nKush bit rate = %u", p2m.kushRate(p2m.frameWidth, p2m.frameHeight, p2m.videoFps));

   printf("\nFinished.\n");
   return p2m.lastError;
}


int wmain(int argc, const wchar_t* argv[])
{
   printf("\nSyntax: pic2video jpeg-wildcard output-path [videoFPS]\n");

   wchar_t wildcard[MAX_PATH + 2]{};
   wchar_t outfile[MAX_PATH + 2]{};
   unsigned fps = 0;
   wchar_t* tail{L""};

   if (1 < argc)
      ExpandEnvironmentStrings(argv[1], wildcard, _countof(wildcard));
   if (2 < argc)
      ExpandEnvironmentStrings(argv[2], outfile, _countof(outfile));
   if (3 < argc)
      fps = wcstoul(argv[3], &tail, 10);

   printf("\nWildcard: %ls", wildcard);
   printf("\nOutfile:  %ls", outfile);
   printf("\nVideoFPS: %u", fps);

   if (!wildcard[0] || !outfile[0] || *tail)
   {
      printf("\n\nMissing or invalid parameters.\n");
      return ERROR_INVALID_PARAMETER;
   }

   GdiplusStartupInput gdiplusStartupInput;
   ULONG_PTR gdiplusToken;
   CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
   GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
   int rc = makeMovie(wildcard, outfile, fps, false);
   GdiplusShutdown(gdiplusToken);
   CoUninitialize();
   return rc;
}
