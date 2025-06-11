package com.chromeExtensions.services.Implementations;

import com.chromeExtensions.services.Interfaces.IFileMonitorService;
import com.chromeExtensions.services.Util;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Service;
import org.springframework.web.multipart.MultipartFile;

import java.util.List;

@Service
public class FileMonitorService implements IFileMonitorService {

    @Value("${fileTypesToBlock.magicNumbers}")
    private List<String> FileTypesToBlockMagicNumber;

    @Override
    public boolean shouldBlockFileDownload(MultipartFile file) {
        try{
            for (String s : FileTypesToBlockMagicNumber) {
                byte[] magicNumber = Util.hexStringToByteArray(s);
                // if file size is smaller than magic number size(in bytes) then skip
                if (magicNumber.length > file.getSize()) {
                    continue;
                }
                // else check if file starts with magic number
                if (checkIfFileStartsWithMagicNumber(file, magicNumber)) {
                    return true;
                }
            }

            return false;
        }catch(Exception e){
            return true;
        }
    }

    private boolean checkIfFileStartsWithMagicNumber(MultipartFile file, byte[] magicNumber) {
        byte[] fileBytes = Util.readFileToByteArrayByLength(file, magicNumber.length);

        for (int i = 0; i < magicNumber.length; i++) {
            if (fileBytes[i] != magicNumber[i]) {
                return false;
            }
        }
        return true;
    }
}
