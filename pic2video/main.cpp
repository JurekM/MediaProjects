#include "stdafx.h"
#include "PicsToMovie.h"

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
   printf("\nFile size KB  = %u", unsigned(uli.QuadPart / 1024));
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
