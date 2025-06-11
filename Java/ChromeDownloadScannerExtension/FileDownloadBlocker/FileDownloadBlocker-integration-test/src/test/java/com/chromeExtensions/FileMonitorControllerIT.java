package com.chromeExtensions;

import org.junit.jupiter.api.Test;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.autoconfigure.web.servlet.AutoConfigureMockMvc;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.mock.web.MockMultipartFile;
import org.springframework.test.web.servlet.MockMvc;
import org.springframework.test.web.servlet.result.MockMvcResultMatchers;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;

import static org.springframework.test.web.servlet.request.MockMvcRequestBuilders.multipart;

@SpringBootTest
@AutoConfigureMockMvc
public class FileMonitorControllerIT {

    @Autowired
    private MockMvc mockMvc;

    @Test
    public void shouldBlockFileDownloadTestTrue_success() throws Exception {
        MockMultipartFile multipartFile = createFile("notepad.exe");

        mockMvc.perform(multipart("/file-monitor/should-block")
                        .file(multipartFile))
                .andExpect(MockMvcResultMatchers.status().isOk())
                .andExpect(MockMvcResultMatchers.content().string("true"));
        ;
    }

    @Test
    public void shouldBlockFileDownloadTestFalse_success() throws Exception {
        MockMultipartFile multipartFile = createFile("file.json");

        mockMvc.perform(multipart("/file-monitor/should-block")
                        .file(multipartFile))
                .andExpect(MockMvcResultMatchers.status().isOk())
                .andExpect(MockMvcResultMatchers.content().string("false"));
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
