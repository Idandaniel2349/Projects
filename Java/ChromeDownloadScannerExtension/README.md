Chrome Download Scanner Extension
A Chrome extension that intercepts file downloads, sends a small portion of the file to a backend API for scanning, and decides whether to allow or block the download based on the response.

🧩 Features

    - Intercepts all file downloads in Chrome.

    - Cancels the original download and sends the first 16 bytes to a backend API.

    - If the API determines the file is safe, re-triggers the download.

    - Built-in retry mechanism to prevent infinite loops.

    - Designed to demonstrate browser extension development with external API integration.

🛠️ Technologies Used

    Frontend: JavaScript (Chrome Extension APIs)

    Backend: Java / Spring Boot

🧪 How It Works

1. The extension listens for new downloads using the chrome.downloads.onCreated event.

2. When a download starts:

   - It cancels the download.

   - Fetches the first 16 bytes of the file.

   - Sends them (as a form-data file) to a backend API.

3. The API replies with true (block) or false (allow).

4. If allowed, the extension re-triggers the download.

Setup

1. Backend FileDownloadBlocker ms: 

    - Run ChromeDownloadScannerExtension\FileDownloadBlocker\FileDownloadBlocker-api\target\FileDownloadBlocker-api-0.0.1-SNAPSHOT.

    - It will run locally on port 8081.

2. Frontend(Extension):  

    - Load the Chrome Extension.

    - Open Chrome and go to chrome://extensions.
   
     - Enable "Developer mode" (top-right).
     
     - Click "Load unpacked".

     - Select the extension/ folder.