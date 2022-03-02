/*
  ==============================================================================

    FileList.cpp
    Created: 22 Feb 2022 1:54:06pm
    Author:  richp

  ==============================================================================
*/

#include "FileList.h"

/*
std::vector <juce::File> FileList::getFilesAll() const
{
    return filesAll;
}
*/

FileList::FileList()
{
    //DEFAULT_FILE_LOCATION = File::getSpecialLocation(File::SpecialLocationType::userMusicDirectory);
    //DEFAULT_FILELIST_LOCATION = getDefaultFileListPath();
}

FileList::FileList(const File& defaultFileLocation, const File& defaultFileListLocation )
{
    DEFAULT_FILE_LOCATION = defaultFileLocation;
    DEFAULT_FILELIST_LOCATION = defaultFileListLocation;
}

void FileList::addFilesFromChooser(int visibleInsertPos)
{

    FileChooser chooser{ "Select files ...", File(DEFAULT_FILE_LOCATION) };
    if (chooser.browseForMultipleFilesToOpen())
    {
        juce::Array < juce::File> res = chooser.getResults();
        setFileLocation(res[0]);
        insertFiles(res, visibleInsertPos);
    }
}

void FileList::sortByName(bool isForwards)
{
    if (isForwards)
    {
        std::sort(filesAll.begin(), filesAll.end(), FileList::compFileNamesAsc);
    }
    else
    {
        std::sort(filesAll.begin(), filesAll.end(), FileList::compFileNamesDesc);
    }
}

void FileList::sortByID(bool isForwards)
{
    if (isForwards)
    {
        std::sort(filesAll.begin(), filesAll.end(), FileList::compFileIDAsc);
    }
    else
    {
        std::sort(filesAll.begin(), filesAll.end(), FileList::compFileIDDesc);
    }
}

bool FileList::compFileIDAsc(const FileListItem& f1, const FileListItem& f2)
{
    return (f2.fileID > f1.fileID);
}
bool FileList::compFileIDDesc(const FileListItem& f1, const FileListItem& f2)
{
    return compFileIDAsc(f2, f1);
}


bool FileList::compFileNamesAsc(const FileListItem& f1, const FileListItem& f2)
{
    return (f2.file.getFileName() > f1.file.getFileName());
}

bool FileList::compFileNamesDesc(const FileListItem& f1, const FileListItem& f2)
{
    return compFileNamesAsc(f2, f1);
}

void FileList::setFileLocation(File file)
{
    DEFAULT_FILE_LOCATION = file;
}
void FileList::setFileListLocation(File file)
{
    DEFAULT_FILELIST_LOCATION = file;
}

juce::String FileList::getFileLocation()
{
    return DEFAULT_FILE_LOCATION.getFullPathName();
}
juce::String FileList::getFileListLocation()
{
    return DEFAULT_FILELIST_LOCATION.getFullPathName();
}



void FileList::insertFiles(const juce::Array<juce::File>& newTracks, int insertPos)
{
    
    //for when no row is selected, insert at end of list
    if (insertPos == -1) insertPos = filesAll.size();
    //bounds check before calling insert
    if ((insertPos >= 0) && (insertPos <= filesAll.size()))
    {
        for (const File& f : newTracks)
        {
            filesAll.insert(filesAll.begin() + insertPos, FileListItem{-1, f, true });
            ++insertPos;
        }
    }
    
    updateIndexes();
}

void FileList::updateIndexes()
{
    int count{ 0 };
    filesVisible.clear();
    for (int i =0; i<filesAll.size(); ++i)
    {
        filesAll[i].fileID = i;
        if (filesAll[i].visible)
        {
            filesVisible.push_back(i);
            ++count;
        }
    }
}

int FileList::getNumFilesVisible()
{
    int count{ 0 };
    
    for (const FileListItem& f : filesAll)
    {
        if (f.visible) ++count;
    }
    
    return count;
}


File FileList::getFileVisible(int visibleFileIndex)
{
    int count{ 0 };
    for (const FileListItem& f : filesAll)
    {
        if (f.visible)
        {
            if (count == visibleFileIndex) return f.file;
            ++count;
        }
    }

    //No visible file found with that index, return empty File
    DBG("Bad visible file index:" << visibleFileIndex);
    return File{};
}

FileList::FileListItem FileList::getFileListItemVisible(int visibleFileIndex)
{
    int count{ 0 };
    for (const FileListItem& f : filesAll)
    {
        if (f.visible)
        {
            if (count == visibleFileIndex) return f;
            ++count;
        }
    }

    //No visible file found with that index, return empty File
    DBG("Bad visible file index:" << visibleFileIndex);
    return FileListItem{};
}




void FileList::filterFiles(const juce::String& searchText)
{
    
    for (int i=0; i < filesAll.size(); ++i)
    {
        filesAll[i].visible  = filesAll[i].file.getFileName().containsIgnoreCase(searchText);
    }
    updateIndexes();
    
}

void FileList::deleteSelectedRows(std::vector<int> selectedRows)
{
    
    //First get vector of indices to delete from filesAll vector from selected rows
    std::vector<int> rowIndices{};
    for (int i = 0; i < selectedRows.size(); ++i)
    {
        int thisRow = selectedRows[i];
        //bounds check!!
        if ((thisRow >= 0) & (thisRow < filesVisible.size()))
        {
            rowIndices.push_back(filesVisible[thisRow]);
        }
    }

    //Need to sort rowIndices in order for erase operation
    std::sort(rowIndices.begin(), rowIndices.end());


    //Need to iterate from end to start so that prior indices are preserved when using erase on the vector    
    for (int i = rowIndices.size() - 1; i >= 0; --i)
    {
        int rowNum = rowIndices[i];
        DBG("Erase track:" << rowNum);
        //bounds check!!
        if ((rowNum >= 0) && (rowNum < filesAll.size()))
            filesAll.erase(filesAll.begin() + rowNum);
    }

    //Then need to regenerate the indexes following erase operations
    updateIndexes();

    
}


void FileList::saveFileListAs()
{

    FileChooser chooser{ "Save files as...", DEFAULT_FILELIST_LOCATION,  "*" + fileListExt};
    if (chooser.browseForFileToSave(true))
    {
        juce::File res = chooser.getResult();
        saveFileListToDisk(res);
    }
}

void FileList::saveFileListToDisk(File saveFile)
{
    FileOutputStream stream(saveFile);
    if (stream.openedOk())
    {
        setFileListLocation(saveFile);
        stream.setPosition(0);
        stream.truncate();
        for (const FileListItem& f : filesAll)
        {
            DBG("Saving to playlist file:" << f.file.getFullPathName());
            stream << f.file.getFullPathName();
            stream << "\n";
        }
    }
}

void FileList::saveFileListToDefault()
{
    File path = DEFAULT_FILELIST_LOCATION;
    saveFileListToDisk(path);
}

void FileList::loadFileList()
{
    FileChooser chooser{ "Load files ...", DEFAULT_FILELIST_LOCATION, "*" + fileListExt};
    if (chooser.browseForFileToOpen())
    {
        juce::File res = chooser.getResult();
        loadFileListFromDisk(res);
    }
}

/*
File FileList::getDefaultFileListPath()
{
    File cwd = File::getCurrentWorkingDirectory();
    String path{ cwd.getFullPathName() };
    path.append("\\defaultFileList", 20);
    path.append(fileListExt, 20);
    return File{ path };
}
*/

void FileList::loadFileListFromDisk(File loadFile)
{
    FileInputStream stream{ loadFile };
    if (stream.openedOk())
    {
        setFileListLocation(loadFile);
        filesAll.clear();
        while (!stream.isExhausted())
        {
            auto nextTrack = stream.readNextLine();
            DBG(nextTrack);
            filesAll.push_back(FileListItem{ -1, File{ nextTrack }, true });
        }
    }
    updateIndexes();
}


/*
void FileList::loadFileListFromDefault()
{
    
    File cwd = File::getCurrentWorkingDirectory();
    String path{ cwd.getFullPathName() };
    path.append("\\playlist" + fileListExt, 20);
    DBG("CWD is:" << path);
    File file = File{ path };
    loadFileListFromDisk(file);
    
}
*/



