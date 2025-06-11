const apiBaseUrl = "http://localhost:8081";
const apiShouldBlockUrl = "/file-monitor/should-block";

const restartedDownloads = new Set();

//Main call
chrome.downloads.onCreated.addListener(handleDownload);

// Main logic
async function handleDownload(downloadItem) {
    try {
        console.log("Download started:", downloadItem.id, "URL:", downloadItem.url);

        // if its a file restart(meaning we already checked it and redownloaded it in code(for same download from the web)) then no need to do anything (just delete from list)
        if (restartedDownloads.has(downloadItem.id)) {
            console.log("Ignoring restarted download:", downloadItem.id);
            restartedDownloads.delete(downloadItem.id);
            return;
        }

        if (!downloadItem.url) {
            console.warn("No URL available for download:", downloadItem.id);
            await chrome.downloads.cancel(downloadItem.id);
            return;
        }

        // get the file name
        const results = await chrome.downloads.search({ id: downloadItem.id });
        const fullDownload = results.length > 0 ? results[0] : undefined;
        const fileName = extractFileName(fullDownload, downloadItem);

        console.log("Using filename:", fileName);

        // block download for now, will redownload later if necessary
        await chrome.downloads.cancel(downloadItem.id);

        //get file data
        const firstBytesBlob = await getFirstBytesOfFile(downloadItem);

        const formData = new FormData();
        formData.append("file", firstBytesBlob, fileName);

        // check if should block download, if not allow it
        const shouldBlock = await shouldBlockDownload(formData);

        if (!shouldBlock) {
            console.log("Allowing download of file:", fileName);
            await downloadFile(downloadItem, fileName);
        }

    } catch (error) {
        console.error("Error handling download:", error);
        try {
            await chrome.downloads.cancel(downloadItem.id);
        } catch (cancelError) {
            console.error("Error handling download:", cancelError);
            // Already canceled or failed, do nothing
        }
        return;
    }

}

//Helper functions


function extractFilenameFromUrl(url) {
    try {
        const urlObj = new URL(url);
        const pathname = urlObj.pathname;
        const parts = pathname.split("/");
        const fileName = parts.pop();
        return fileName || "unknown";
    } catch {
        return "unknown";
    }
}

// extract from filename if exists, else try to extract from url
function extractFileName(fullDownload, downloadItem) {
    if (fullDownload && fullDownload.fileName) {
        return fullDownload.fileName.split(/[\\/]/).pop();
    } else {
        return extractFilenameFromUrl(downloadItem.url);
    }
}


async function getFirstBytesOfFile(downloadItem) {
    console.log("Fetching URL:", downloadItem.url);
    const fileResponse = await fetch(downloadItem.url, {
        method: 'GET',
        headers: {
            'Range': 'bytes=0-15'
        },
        mode: 'cors',
        credentials: 'include'
    });

    if (!fileResponse.ok) {
        console.error("Failed to fetch file from URL:", downloadItem.url);
        throw new Error("get file data API call failed: " + fileResponse.status);
    }

    const fileBlob = await fileResponse.blob();
    return fileBlob;
}


async function shouldBlockDownload(formData) {
    // Call should-block API
    const apiResponse = await fetch(apiBaseUrl + apiShouldBlockUrl, {
        method: "POST",
        body: formData
    });

    if (!apiResponse.ok) {
        console.error("API error:", apiResponse.status);
        throw new Error("should-block API call failed: " + apiResponse.status);
    }
    const text = (await apiResponse.text()).trim();
    const shouldBlock = text === "true";

    console.log("Should block:", shouldBlock);
    return shouldBlock;
}


async function downloadFile(downloadItem, fileName) {
    chrome.downloads.download({
        url: downloadItem.url,
        filename: fileName,
        conflictAction: "uniquify",
        saveAs: false,
    }, (newDownloadId) => {
        if (chrome.runtime.lastError) {
            console.error("Download restart failed:", chrome.runtime.lastError.message);
            return;
        }
        restartedDownloads.add(newDownloadId);
        console.log("Restarted download with ID:", newDownloadId);
    });

}