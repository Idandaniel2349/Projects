package com.chromeExtensions;

import com.chromeExtensions.services.Util;
import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;
import org.springframework.mock.web.MockMultipartFile;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;

public class UtilTest {

    @Test
    public void hexStringToByteArrayWindowsExecutableTest_success() {
        String hexString = "4D5A";
        byte[] expected = new byte[]{0x4D, 0x5A};

        byte[] result = Util.hexStringToByteArray(hexString);

        Assertions.assertArrayEquals(expected, result);
    }

    @Test
    public void hexStringToByteArrayTest_success() {
        String hexString = "48656c6c6f20576f726c6421";
        byte[] expected = new byte[]{72, 101, 108, 108, 111, 32, 87, 111, 114, 108, 100, 33};

        byte[] result = Util.hexStringToByteArray(hexString);

        Assertions.assertArrayEquals(expected, result);
    }

    @Test
    public void readFileToByteArrayByLengthTest_success() throws IOException {
        Path path = Paths.get("src/test/resources/notepad.exe");
        byte[] fileContent = Files.readAllBytes(path);
        byte[] expected = new byte[]{(byte) 0x4D, (byte) 0x5A};

        MockMultipartFile multipartFile = new MockMultipartFile(
                "file",                  // form field name
                "test.exe",              // original file name
                "application/octet-stream",  // content type
                fileContent                  // file content as byte[]
        );

        byte[] result = Util.readFileToByteArrayByLength(multipartFile, 2);

        Assertions.assertArrayEquals(expected, result);
    }


}
