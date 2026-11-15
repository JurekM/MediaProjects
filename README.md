# MediaProjects

**Fast triage tools for large motion-camera image archives.**

If you have hundreds or thousands of still images from a camera trap, driveway cam, or other motion-detection setup, this repository is built for the same workflow:

1. Turn a large image sequence into a short MP4 preview.
2. Spot interesting movement quickly.
3. Keep what matters and delete the rest.
4. Optionally review selected images in an HTML gallery.

This approach dramatically reduces review time: a few seconds of video can summarize hundreds of frames from a fixed camera.

## Tools

### `pic2video`

Windows C++ console tool that creates an MP4 from JPEG frames.

- **Best for:** first-pass scanning of large folders.
- **Why it helps:** motion stands out immediately when frames are played back.
- **Typical usage concept:** provide an input wildcard and an output `.mp4` path, optionally with FPS.

High-level syntax shown by the tool:

`pic2video jpeg-wildcard output-path [videoFPS]`

### `pic2gallery`

.NET console tool that generates an HTML image gallery from a file list.

- **Best for:** second-pass, image-by-image inspection after video triage.
- **Why it helps:** quick manual review of shortlisted frames in a browser.

High-level syntax shown by the tool:

`pic2gallery <inputfile> <outputfile> [.|root] [yyyymmdd]`

## Who this is for

These tools are useful if you search for:

- motion camera image review
- camera trap frame triage
- convert JPEG sequence to MP4 for inspection
- wildlife camera archive cleanup
- burst-image event detection workflow

## Developer notes (using this as a base)

If you want to adapt this repo for your own image-triage pipeline:

- **`pic2video`** contains a practical C++/Media Foundation path from image sequences to H.264 MP4.
- **`pic2gallery`** provides a simple gallery-generation flow for browser-based manual review.
- Keep the two-stage pattern (**quick video scan -> focused gallery review**) and tailor filename parsing, grouping, and output format to your camera naming scheme.

## CI build

GitHub Actions workflow: `.github/workflows/build.yml`

- Runs on pushes and pull requests.
- Builds `MediaProjects.sln` on Windows for `Release|x64` and `Release|x86`.
- Can also be started manually via **workflow_dispatch**.
