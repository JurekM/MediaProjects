# MediaProjects

Windows utilities for creating MP4 previews and HTML galleries from camera images.

These utilities were written to work with images from Foscam motion-detection cameras. `pic2video` creates MP4 previews from JPEG images, while `pic2gallery` generates HTML galleries using information parsed from Foscam-style filenames. You may find the utilities, or parts of their code, useful in your own projects.

The original workflow was to preview a folder of motion-detection images as a video, then inspect individual images in a gallery. Selecting and deleting images is a manual step; these tools only generate the previews.

## pic2video

A Windows C++ console tool that uses Media Foundation to encode JPEG images as an H.264 MP4 video. It does not require Foscam filenames.

```text
pic2video jpeg-wildcard output-path [videoFPS]
```

For example, from PowerShell with the executable in the current directory:

```powershell
.\pic2video.exe "D:\Camera\*.jpg" "D:\Camera\preview.mp4" 15
```

The optional frame rate defaults to 15 FPS. Each input image contributes one video frame; timestamps in filenames do not control playback timing.

Files are read using Windows file enumeration, without an explicit sort. Chronological playback order is therefore not guaranteed. Use a backslash-separated Windows path for the input wildcard, as in the example.

## pic2gallery

A .NET Framework console tool that generates an HTML page containing linked image thumbnails from a text file of image paths. The page references the original images; it does not copy or embed them.

```text
pic2gallery <inputfile> <outputfile> [.|root] [yyyymmdd]
```

Create a file list and generate a gallery from PowerShell:

```powershell
Get-ChildItem "D:\Camera\*.jpg" -File |
    Select-Object -ExpandProperty FullName |
    Set-Content -Encoding UTF8 .\images.txt
.\pic2gallery.exe .\images.txt .\gallery.html
```

Open `gallery.html` in a browser on the computer containing the images.

The filename parser expects the Foscam-style naming conventions used here, for example:

```text
000DC5D739DC(Sarenki)_1_20171008134036_233.jpg
```

In this example, `000DC5D739DC` identifies the device, `Sarenki` is its name, and `20171008134036` contains the date (`20171008`) and time (`134036`). The parser also accepts a form without the parenthesized camera name. This is not a claim of compatibility with every Foscam model or firmware version. Filenames that do not match the parser are silently omitted.

Optional arguments:

- `root`: a prefix added directly to every image path. Include any required trailing separator. Use `.` for no prefix when supplying a date filter.
- `yyyymmdd`: include only images with this date parsed from their filenames, rather than their filesystem dates.

For example:

```powershell
.\pic2gallery.exe .\images.txt .\gallery-20171008.html . 20171008
```

The output HTML file must not already exist. Relative image paths are resolved by the browser relative to the generated HTML file.

## Building

The solution has been built locally with Visual Studio 2026 using the `v145` platform toolset. Install:

- Visual Studio 2026 with the Desktop development with C++ workload and C++ ATL support for x86/x64.
- A Windows SDK.
- The .NET Framework 4.8 targeting pack for `pic2gallery`.

Open `MediaProjects.sln` in Visual Studio, or run this from a Visual Studio Developer PowerShell:

```powershell
msbuild MediaProjects.sln /m /p:Configuration=Release /p:Platform=x64
```

Use `x86` for a 32-bit native build. Debug and Release builds have been verified for both architectures; `pic2gallery` builds as Any CPU.

Build outputs are placed in `..\__binaries\MediaProjects` relative to the repository root. Intermediate files are placed in `..\__int\MediaProjects`.

## GitHub Actions

The [build workflow](.github/workflows/build.yml) builds Release for x64 and x86 on Windows. It runs for pull requests and pushes to the branches listed in the workflow, and can also be started manually.

Successful runs provide `binaries-x64-Release` and `binaries-x86-Release` artifacts containing both utilities. Open a completed run in the repository's [Actions tab](https://github.com/JurekM/MediaProjects/actions) to download them.

## Using the code

The shared video-writing code is in `include/PicsToMovie.h` and `include/PicsToMovie.cpp`. The gallery generator and filename parser are in `pic2gallery/HtmlGenerator.cs`. These may be useful starting points for adapting image previews or filename parsing to your own projects.

See [LICENSE](LICENSE) for the MIT license.
