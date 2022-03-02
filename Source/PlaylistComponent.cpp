/*
  ==============================================================================

    PlaylistComponent.cpp
    Created: 3 Feb 2022 8:42:17pm
    Author:  richp

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PlaylistComponent.h"

//==============================================================================
PlaylistComponent::PlaylistComponent()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    
    //Save this state and reload from previous?
    //DEFAULT_MUSIC_LOCATION = File::getSpecialLocation(File::SpecialLocationType::userMusicDirectory);

    tableComponent.getHeader().addColumn("ID", (int)COLS::ID, 50);
    tableComponent.getHeader().addColumn("Track Title", (int) COLS::FILENAME, 300);
    tableComponent.getHeader().addColumn("Play", (int) COLS::PLAY, 50);
    tableComponent.getHeader().addColumn("Loading", (int) COLS::LOADING, 50);
    tableComponent.getHeader().addColumn("Length", (int) COLS::LENGTH, 50);
    tableComponent.getHeader().addColumn("Format", (int) COLS::FORMAT, 100);
    tableComponent.getHeader().addColumn("Deck", (int) COLS::DECK, 50);
    tableComponent.getHeader().addColumn("Status", (int)COLS::STATUS, 200);

    tableComponent.setModel(this);
    tableComponent.setMultipleSelectionEnabled(true);
    addAndMakeVisible(tableComponent);

    addAndMakeVisible(buttonAdd);
    //addAndMakeVisible(buttonSave);
    addAndMakeVisible(buttonSaveAs);
    addAndMakeVisible(buttonLoad);
    buttonAdd.addListener(this);
    //buttonSave.addListener(this);
    buttonSaveAs.addListener(this);
    buttonLoad.addListener(this);

    addAndMakeVisible(labelSearch);
    labelSearch.addListener(this);
    labelSearch.setColour(Label::ColourIds::backgroundColourId, juce::Colours::grey);
    labelSearch.setColour(Label::ColourIds::textColourId, juce::Colours::white);
    labelSearch.setColour(Label::ColourIds::outlineColourId, juce::Colours::orange);
    labelSearch.setColour(Label::ColourIds::backgroundWhenEditingColourId, juce::Colours::darkorange);
    labelSearch.setColour(Label::ColourIds::textWhenEditingColourId, juce::Colours::black);
    labelSearch.setColour(Label::ColourIds::outlineWhenEditingColourId, juce::Colours::black);
    labelSearch.setEditable(true, false, false);
    //labelSearch.onTextChange = [this] {searchUpdated();};


    menu.addItem((int) MENU::INSERT, "Insert Tracks ...", true, false);
    menu.addItem((int) MENU::REMOVE, "Remove Selected Tracks ...", true, false);
    menu.addItem((int) MENU::PLAY, "Play Track", true, false);



}

PlaylistComponent::~PlaylistComponent()
{
}

void PlaylistComponent::sortOrderChanged(int newSortColumnId, bool isForwards)
{
    if (newSortColumnId == (int) COLS::FILENAME)
    {
        trackList.sortByName(isForwards);
    }
    if (newSortColumnId == (int)COLS::ID)
    {
        trackList.sortByID(isForwards);
    }
    tableComponent.updateContent();

}


juce::var PlaylistComponent::getSettings()
{
    var settings{ new juce::DynamicObject{} };
    settings.getDynamicObject()->setProperty("MusicLocation", trackList.getFileLocation());
    settings.getDynamicObject()->setProperty("PlaylistLocation", trackList.getFileListLocation());
    //settings.getDynamicObject()->setProperty("LastPlaylistName", trackList.getFileListLocation()  );
    return settings;
}

void PlaylistComponent::setSettings(var settings)
{
    if (!settings.isObject())
        return;

    if (settings.hasProperty("MusicLocation"))
    {
        String musicLoc = settings.getProperty("MusicLocation", "");
        if (musicLoc != "")
            //DEFAULT_MUSIC_LOCATION = File{musicLoc};
            trackList.setFileLocation(File{musicLoc});
    }
    if (settings.hasProperty("PlaylistLocation"))
    {
        String appLoc = settings.getProperty("PlaylistLocation", "");
        if (appLoc != "")
            //DEFAULT_APP_LOCATION = File{ appLoc};
            trackList.setFileListLocation(File{ appLoc });
    }

}



void PlaylistComponent::AddFiles()
{
    int insertPos = tableComponent.getSelectedRow(0);
    
    trackList.addFilesFromChooser(insertPos);

    tableComponent.updateContent();

}

void PlaylistComponent::LoadFiles()
{
    trackList.loadFileList();
    tableComponent.updateContent();

}

void PlaylistComponent::SaveFilesAs()
{
    trackList.saveFileListAs();
}

void PlaylistComponent::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("PlaylistComponent", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void PlaylistComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    int w = getWidth();
    int h = getHeight();
    
    labelSearch.setBounds   (w * 0.0, 0, w * 0.2, h * 0.1);
    buttonAdd.setBounds     (w * 0.3, 0, w * 0.1, h * 0.1);
    buttonLoad.setBounds    (w * 0.4, 0, w * 0.1, h * 0.1);
    //buttonSave.setBounds    (w * 0.5, 0, w * 0.1, h * 0.1);
    buttonSaveAs.setBounds  (w * 0.6, 0, w * 0.1, h * 0.1);
    tableComponent.setBounds(w * 0.0, h * 0.1, w, h * 0.9);

}

int PlaylistComponent::getNumRows()
{
    return trackList.getNumFilesVisible();

}

void PlaylistComponent::paintRowBackground(Graphics& g,
    int 	rowNumber,
    int 	width,
    int 	height,
    bool 	rowIsSelected)
{
    if (rowIsSelected)
    {
        g.fillAll(Colours::orange);
    }
    else
    {
        g.fillAll(Colours::darkgrey);
    }

}

void PlaylistComponent::paintCell(Graphics& g,
    int rowNumber,
    int columnId,
    int width, int height,
    bool rowIsSelected)
{
    
    //rowNumber may return a value greater than the number of rows in the list(vector) as per JUCE docs
    if (rowNumber < getNumRows())
    {
        if (columnId == (int)COLS::ID)
        {
            g.drawText((String) trackList.getFileListItemVisible(rowNumber).fileID,
                2, 0,
                width - 4, height,
                Justification::centredLeft,
                true);
        }
        if (columnId == (int) COLS::FILENAME)
        {
            g.drawText(trackList.getFileVisible(rowNumber).getFileName(),
                2, 0,
                width - 4, height,
                Justification::centredLeft,
                true);
        }
        if (columnId == (int) COLS::FORMAT)
        {
            g.drawText(trackList.getFileVisible(rowNumber).getFileExtension(),
                2, 0,
                width - 4, height,
                Justification::centredLeft,
                true);
        }
        if (columnId == (int) COLS::DECK)
        {
            g.drawText(String{ fmod(rowNumber,2) + 1  },
                2, 0,
                width - 4, height,
                Justification::centredLeft,
                true);
        }
    }
    
}

Component* PlaylistComponent::refreshComponentForCell(int 	rowNumber,
    int 	columnId,
    bool 	isRowSelected,
    Component* existingComponentToUpdate)

{
    if (columnId == (int) COLS::PLAY)
    {
        if (existingComponentToUpdate == nullptr)
        {
            TextButton* btn = new TextButton{ "play" };
            String id{ std::to_string(rowNumber) };
            btn->setComponentID(id);
            btn->addListener(this);
            existingComponentToUpdate = btn;
        }
    }
    
    return existingComponentToUpdate;

}

void PlaylistComponent::buttonClicked(Button* button)
{
    if (button == &buttonAdd)
    {
        AddFiles();
    }
    else if (button == &buttonLoad)
    {
        LoadFiles();
    }
    /*
    else if (button == &buttonSave)
    {
        SaveFiles();
    }
    */
    else if (button == &buttonSaveAs)
    {
        SaveFilesAs();
    }
    else
    {
        int id = std::stoi(button->getComponentID().toStdString());
    }
    
}

void PlaylistComponent::cellClicked(int 	rowNumber,
    int 	columnId,
    const MouseEvent& e)
{
    if (e.mods.isRightButtonDown())
    {
        popUpMenu();
    }

}

void PlaylistComponent::backgroundClicked(const MouseEvent& e)
{
    tableComponent.deselectAllRows();
    if (e.mods.isRightButtonDown())
    {
        popUpMenu();
    }

}



void PlaylistComponent::popUpMenu()
{
    juce::PopupMenu::Options popUpOpt{};
    int result = menu.showMenu(popUpOpt);
    if (result == (int)MENU::PLAY)
    {


    }
    if (result == (int)MENU::REMOVE)
    {
        deleteSelectedRows();
    }
    if (result == (int)MENU::INSERT)
    {
        AddFiles();
    }

}

void PlaylistComponent::deleteKeyPressed(int currentSelectedRow)
{
    deleteSelectedRows();
}


void PlaylistComponent::deleteSelectedRows()
{
    
    std::vector<int> rows = getSelectedRows();
    
    //Need to change this to map onto the right part of the visible rows
    trackList.deleteSelectedRows(rows);

    tableComponent.updateContent();


}

std::vector<int> PlaylistComponent::getSelectedRows()
{
    std::vector<int> result;
    int numRows = tableComponent.getNumSelectedRows();
    for (int i = 0; i < numRows; ++i)
    {
        result.push_back(tableComponent.getSelectedRow(i));
    }
    return result;
}

void PlaylistComponent::labelTextChanged(Label* labelThatHasChanged)
{
    DBG("Label text changed:" << labelThatHasChanged->getText());
    trackList.filterFiles(labelThatHasChanged->getText());
    tableComponent.updateContent();
}

void PlaylistComponent::searchUpdated()
{
    DBG("Search text updated:" << labelSearch.getText());


}





