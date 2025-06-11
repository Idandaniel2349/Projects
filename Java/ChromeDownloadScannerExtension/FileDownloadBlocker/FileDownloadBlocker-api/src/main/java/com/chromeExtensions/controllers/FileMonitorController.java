package com.chromeExtensions.controllers;

import com.chromeExtensions.services.Interfaces.IFileMonitorService;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;
import org.springframework.web.multipart.MultipartFile;

@RestController
@RequestMapping("/file-monitor")
@CrossOrigin(origins = "*")
public class FileMonitorController {

    private final IFileMonitorService fileMonitorService;

    public FileMonitorController(IFileMonitorService fileMonitorService) {
        this.fileMonitorService = fileMonitorService;
    }

    @PostMapping("/should-block")
    public ResponseEntity<Boolean> shouldBlockFileDownload(@RequestParam("file") MultipartFile file) {
        try {
            boolean shouldBlock = fileMonitorService.shouldBlockFileDownload(file);
            return ResponseEntity.ok(shouldBlock);
        } catch (Exception e) {
            // if an error occurs, block the file
            return ResponseEntity.ok(true);
        }
    }
}
