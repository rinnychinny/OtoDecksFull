/*
  ==============================================================================

    PlaylistComponent.h
    Created: 3 Feb 2022 8:42:17pm
    Author:  richp

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include <vector>
#include <string>
#include "FileList.h"


//==============================================================================
/*
*/
class PlaylistComponent  : public juce::Component, public juce::TableListBoxModel, public Button::Listener, public Label::Listener
{
public:
    PlaylistComponent();
    ~PlaylistComponent() override;

    //std::map<std::string, std::string> getSettings();
    juce::var getSettings();
    void setSettings(juce::var settings);

    
    void paint (juce::Graphics&) override;
    void resized() override;

    //overrides from TableListBoxModel
    int getNumRows() override;

    void paintRowBackground(Graphics&,
        int 	rowNumber,
        int 	width,
        int 	height,
        bool 	rowIsSelected) override;

    void paintCell(Graphics&,
        int rowNumber,
        int columnId,
        int width, int height,
        bool rowIsSelected) override;

    Component* refreshComponentForCell(int 	rowNumber,
        int 	columnId,
        bool 	isRowSelected,
        Component* existingComponentToUpdate
    ) override;
    
    /** override from TableListBoxModel  */
    void cellClicked(int 	rowNumber,
        int 	columnId,
        const MouseEvent&
    )	 override;
    
    /** override from TableListBoxModel  */
    void backgroundClicked(
        const MouseEvent&
    )	 override;

        
    void buttonClicked(Button* button) override;
    void labelTextChanged(Label* labelThatHasChanged) override;

    void deleteKeyPressed(int currentSelectedRow) override;

    void sortOrderChanged(int newSortColumnId, bool isForwards); //TableListBoxModel

private:
    
    //used to initialise trackList before any settings get applied
    File MUSIC_LOCATION{ File::getSpecialLocation(File::SpecialLocationType::userMusicDirectory) };
    File APP_LOCATION{ File::getCurrentWorkingDirectory() };
    FileList trackList{ MUSIC_LOCATION, APP_LOCATION };


    
    /**Opens dialog box to select multiples files to add to playlist*/
    void AddFiles();
    /**Opens dialog box to load files from library file*/
    void LoadFiles();
    /**Saves library to current playlist file*/
    //void SaveFiles();
    /**Opens dialog box to save all files in library to a file list*/
    void SaveFilesAs();

    enum class COLS { ID = 1, FILENAME, PLAY, STATUS, LOADING, LENGTH, FORMAT, DECK};
    enum class MENU { PLAY = 1, REMOVE, INSERT };

    TableListBox tableComponent;
    
    
    /*used to update all visible tracks after amending tracksAll vector*/
    //void updateTracksVisible();

    
    //std::vector < juce::File > tracksAll;
    /**contains index for vector tracksAll of all tracks to show(used for filtering / searching)*/
    //std::vector <int> tracksVisible;

    TextButton buttonAdd{ "ADD" };
    TextButton buttonLoad{ "LOAD" };
    //TextButton buttonSave{ "SAVE" };
    TextButton buttonSaveAs{ "SAVE AS.." };

    Label labelSearch{ "FILTER", "Type search string here"};

    PopupMenu menu;

    void menuCallback(int opt);

    /** deletes selected rows from the table list*/
    void deleteSelectedRows();

    /** returns selected rows from the table list*/
    std::vector<int> getSelectedRows();

    /**shows the right click pop up menu*/
    void popUpMenu();

    void searchUpdated();






    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};
