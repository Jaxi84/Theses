# Info-TV

An information display system developed as an ambitious final project during the autumn of 2022 at OSAO (Vocational Qualification in Information and Communications Technology). The system was used at my workplace during an internship at the time.

The project was created before I had formal programming experience or access to AI tools. The implementation was built by studying documentation, Google search results and Stack Overflow examples.

## What It Does

Info-TV displays images on a screen and keeps the content up to date:

1. `dl_drive.py` authenticates to Google Drive with a service account and downloads image files from Drive.
2. The downloaded files are placed in a temporary directory and then swapped into the `media` directory.
3. `karuselli.php` scans the local media directory and displays the images in a Bootstrap carousel.
4. `menu.php` scrapes a restaurant's online lunch menu, renders it as a PNG image and uploads the image to Google Drive.

The carousel page also displays the current ISO week number and refreshes the browser page every 20 seconds.

## Technologies

- Python
- PHP, HTML and JavaScript
- Google Drive API
- Google service account authentication
- Bootstrap 5.2.2 and jQuery
- Simple HTML DOM Parser for extracting menu data
- PHP GD for rendering the menu image

## Project Structure

| File or directory | Purpose |
| --- | --- |
| `dl_drive.py` | Downloads image files from Google Drive to the local media directory. |
| `karuselli.php` | Shows the downloaded images in a Bootstrap carousel. |
| `menu.php` | Extracts, renders and uploads the restaurant menu. |
| `files/simplehtmldom.php` | Bundled HTML parsing library. |
| `files/arial.ttf` | Font used when rendering the menu image. |
| `lib/` | Locally bundled Bootstrap and jQuery assets. |

## Requirements

This repository is a source snapshot of the 2022 project, not a complete, ready-to-deploy package. A working deployment would require at least:

- Python and the Python packages used by `dl_drive.py`.
- PHP with the GD extension.
- Composer-installed Google API client libraries for `menu.php`.
- A Google Cloud service account with access to the shared Drive folder.
- A valid `files/credentials.json` file, kept private and excluded from version control.
- A web server capable of executing PHP.
- A `media` directory, or permission for the downloader to create it.

The original code also contains placeholders for the restaurant URL and Google Drive folder ID. These values must be configured before the application can work in a new environment.

## Important Limitations

The project was suitable for its original practical use, but it should not be considered production-ready by current standards without further work:

- The dependency definitions and installation instructions are incomplete. The Google PHP client and the Python dependencies are not included in this repository.
- The service-account credential file is expected at a hard-coded path. Credentials must never be committed to the repository or exposed through the web server.
- The Python Drive query does not explicitly restrict results to one folder and does not handle paginated API responses.
- Error handling is limited. Network failures, authentication errors, file-system failures and malformed menu pages are not handled robustly.
- The menu scraper depends on specific HTML IDs, classes and sibling elements. Changes to the restaurant's website can break it.
- The menu image has a fixed size and text layout, so long or unexpected menu content may overflow or become unreadable.
- The browser refresh and carousel timings are hard-coded, and there is no administration interface or content validation.
- The carousel images do not include alternative text, and accessibility has not been systematically addressed.
- There are no automated tests, CI configuration, logging strategy or monitoring setup.
- The code mixes older authentication and API-client patterns and would need dependency updates and security review before reuse.

## Project Status

This is an archived educational project from 2022. It demonstrates a real integration between a local display, a web application and Google Drive, but it is preserved primarily as a record of the original implementation and learning process. It should be treated as a starting point for modernization rather than deployed unchanged.

## License

No license is currently specified for this repository.
