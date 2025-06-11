package com.chromeExtensions.services.Interfaces;

import org.springframework.web.multipart.MultipartFile;

public interface IFileMonitorService {
    boolean shouldBlockFileDownload(MultipartFile file);
}
