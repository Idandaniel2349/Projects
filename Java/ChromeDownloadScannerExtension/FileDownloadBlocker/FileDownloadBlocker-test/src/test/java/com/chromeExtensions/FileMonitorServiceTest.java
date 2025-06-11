package com.chromeExtensions;

import com.chromeExtensions.services.Implementations.FileMonitorService;
import com.chromeExtensions.services.Util;
import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.extension.ExtendWith;
import org.mockito.InjectMocks;
import org.mockito.Mock;
import org.mockito.MockedStatic;
import org.mockito.Mockito;
import org.mockito.junit.jupiter.MockitoExtension;
import org.springframework.mock.web.MockMultipartFile;
import org.springframework.test.util.ReflectionTestUtils;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.List;

@ExtendWith(MockitoExtension.class)
public class FileMonitorServiceTest {

    @Mock
    private Util util;

    @InjectMocks
    private FileMonitorService fileMonitorService;

    @BeforeEach
    public void setup() {
        // set up the FileTypesToBlockMagicNumber property
        ReflectionTestUtils.setField(fileMonitorService, "FileTypesToBlockMagicNumber", List.of("4D5A"));
    }

    @Test
    public void shouldBlockFileDownloadTestTrue_success() throws IOException {
        byte[] expectedBytesFromFile = new byte[]{(byte) 0x4D, (byte) 0x5A};

        MockMultipartFile multipartFile = createFile("notepad.exe");

        try (MockedStatic<Util> utilMock = Mockito.mockStatic(Util.class)) {
            utilMock.when(() -> Util.hexStringToByteArray("4D5A")).thenReturn(expectedBytesFromFile);
            utilMock.when(() -> Util.readFileToByteArrayByLength(multipartFile, 2)).thenReturn(expectedBytesFromFile);
        }
        boolean result = fileMonitorService.shouldBlockFileDownload(multipartFile);

        Assertions.assertTrue(result);
    }

    @Test
    public void shouldBlockFileDownloadTestFalse_success() throws IOException {
        byte[] expectedBytesFromFile = new byte[]{(byte) 0x4D, (byte) 0x5A};
        byte[] fileBytes = new byte[]{(byte) 0x46, (byte) 0x59};


        MockMultipartFile multipartFile = createFile("file.json");


        try (MockedStatic<Util> utilMock = Mockito.mockStatic(Util.class)) {
            utilMock.when(() -> Util.hexStringToByteArray("4D5A")).thenReturn(expectedBytesFromFile);
            utilMock.when(() -> Util.readFileToByteArrayByLength(multipartFile, 2)).thenReturn(fileBytes);
        }

        boolean result = fileMonitorService.shouldBlockFileDownload(multipartFile);

        Assertions.assertFalse(result);
    }


    @Test
    public void shouldBlockFileDownloadTest_failure() throws IOException {
        byte[] expectedBytesFromFile = new byte[]{(byte) 0x4D, (byte) 0x5A};
        byte[] fileBytes = new byte[]{(byte) 0x46, (byte) 0x59};


        MockMultipartFile multipartFile = createFile("file.json");


        try (MockedStatic<Util> utilMock = Mockito.mockStatic(Util.class)) {
            utilMock.when(() -> Util.hexStringToByteArray("4D5A")).thenReturn(expectedBytesFromFile);
            utilMock.when(() -> Util.readFileToByteArrayByLength(multipartFile, 2)).thenThrow(new RuntimeException("Failed to read file bytes"));
        }
        try {
            boolean result = fileMonitorService.shouldBlockFileDownload(multipartFile);
        } catch (Exception e) {
            Assertions.assertEquals("Failed to read file bytes", e.getMessage());
        }
    }

    private MockMultipartFile createFile(String fileName) throws IOException {
        Path path = Paths.get("src/test/resources/" + fileName);
        byte[] fileContent = Files.readAllBytes(path);

        MockMultipartFile multipartFile = new MockMultipartFile(
                "file",                  // form field name
                fileName,              // original file name
                "application/octet-stream",  // content type
                fileContent                  // file content as byte[]
        );

        return multipartFile;
    }


}
