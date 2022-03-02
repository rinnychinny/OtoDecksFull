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

    struct FileListItem
    {
        int        fileID;
        juce::File file;
        bool       visible;
    };

    /** file extension used for saving a FileList to disk*/
    String fileListExt = ".flst";
    
    void addFilesFromChooser(int visibleInsertPos);
    int getNumFilesVisible();

    juce::File getFileVisible(int visibleFileIndex);
    FileListItem getFileListItemVisible(int visibleFileIndex);

    void filterFiles(const juce::String& searchText);

    /** expects visibleRows to be in ascending order for delte operation to work*/
    void deleteSelectedRows(std::vector<int> selectedRows);

    void saveFileListToDefault();
    void saveFileListAs();
    void saveFileListToDisk(File saveFile);

    void sortByName(bool isForwards);
    void sortByID(bool isForwards);

    void loadFileList();
    void loadFileListFromDisk(File loadFile);


    juce::String getFileLocation();
    juce::String getFileListLocation();
    void setFileLocation(File file);
    void setFileListLocation(File file);

private:
    
    File DEFAULT_FILE_LOCATION{};
    File DEFAULT_FILELIST_LOCATION{};

    static bool compFileNamesAsc(const FileListItem& f1, const FileListItem& f2);
    static bool compFileNamesDesc(const FileListItem& f1, const FileListItem& f2);
    static bool compFileIDAsc(const FileListItem& f1, const FileListItem& f2);
    static bool compFileIDDesc(const FileListItem& f1, const FileListItem& f2);




    std::vector <FileListItem> filesAll;
    /* contains indexing into filesAll for each visible row*/
    std::vector <int> filesVisible;
    void updateIndexes();
    
    void insertFiles(const juce::Array<juce::File>& newFiles, int insertIndex = 0);



};
