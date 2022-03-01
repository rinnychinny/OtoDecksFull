/*
  ==============================================================================

    FileList.h
    Created: 22 Feb 2022 1:54:06pm
    Author:  richp

  ==============================================================================
*/

#pragma once

#include <vector>
#include <JuceHeader.h>



class FileList
{
public:

    FileList();
    FileList(const File& defaultFileLocation, const File& defaultFileListLocation);

    
    /** file extension used for saving a FileList to disk*/
    String fileListExt = ".flst";
    
    void addFilesFromChooser(int visibleInsertPos);
    int getNumFilesVisible();
    juce::File getFileVisible(int visibleFileIndex);
    void filterFiles(const juce::String& searchText);

    /** expects visibleRows to be in ascending order for delte operation to work*/
    void deleteSelectedRows(std::vector<int> selectedRows);

    //std::vector <juce::File> getFilesAll() const;

    void saveFileListToDefault();
    void saveFileListAs();
    void saveFileListToDisk(File saveFile);

    //File getDefaultFileListPath();



    
    void loadFileList();
    //void loadFileListFromDefault();
    void loadFileListFromDisk(File loadFile);


    struct FileListItem
    {
        //int        fileID;
        juce::File file;
        bool       visible;
    };
    

private:
    
    
    File DEFAULT_FILE_LOCATION{};
    File DEFAULT_FILELIST_LOCATION{};

    void setFileLocation(File file);
    void setFileListLocation(File file);

    std::vector <FileListItem> filesAll;
    /* contains indexing into filesAll for each visible row*/
    std::vector <int> filesVisible;
    void updateIndexes();
    
    void insertFiles(const juce::Array<juce::File>& newFiles, int insertIndex = 0);



};
