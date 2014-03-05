<?php
/**
 * Indicia, the OPAL Online Recording Toolkit.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see http://www.gnu.org/licenses/gpl.html.
 *
 * @package	Client
 * @author	Indicia Team
 * @license	http://www.gnu.org/licenses/gpl.html GPL 3.0
 * @link 	http://code.google.com/p/indicia/
 */

// Translation strings for when in Amphibian mode, on top of reptile ones.
// this file should be copied to the file 'node.<nid>.en.php';
global $custom_term_overrides;
$custom_term_overrides[] = array(
	// 'Edit' is left unchanged
	'LANG_Shapefile_Download'=> 'These downloads provides zipped up shape files for the locations - due to the restrictions of the shape file format, there is one file for the points and one for the transects (lines). There can be one point or transect per site. Click to select:'
	,'LANG_LocModTool_Instructions2'=>"Either click on the map (ensuring that the select tool on the map is active) to select the site you wish to modify, or choose from the drop down list. You may then change its name, or modify the geometry (a Point and/or a Line) to define the site shape. You must choose the correct draw tool on the map for each of these. You may drag the highlighted vertices. To delete a point (purple circle) or shape vertex (red circle), place the mouse over the circle and press the 'Delete' button.<br />Selecting an existing site, re-clicking the 'Start a new site' (tick) button or clicking the 'Remove the selected new site' (red cross) button will remove any new site."
	,'LANG_LocModTool_CantCreate' => "You can't create a new site using this tool - that has to be done within the survey data entry itself."
	,'LANG_DE_Instructions2'=>"To choose a site, either click the relevant site on the map (ensuring that the select tool on the map is active) or pick it using the drop down lists.<br />You may add a new site: click the 'Start a new site' button on the map, and click on the map. You can change the position by dragging the circles. To delete a point (purple circle) or shape vertex (red circle), place the mouse over the circle and press the 'Delete' button.<br />Selecting an existing site, re-clicking the 'Start a new site' (tick) button or clicking the 'Remove the selected new site' (red cross) button will remove any new site.<br />It is not possible to change a site name or position on this form once it has been saved - this can be done by an Admin user using their special tool."
	,'LANG_Location_X_Label' => 'Site coordinates: X'
	,'LANG_PointTooltip'=>'Click on map to set site position'
	,'LANG_SpeciesInstructions'=>"Species records may be added using the control under the grid.<br />Click the red 'X' to delete the relevant row."
	,'Village' => 'Village/Locality'
	,'Site name' => 'Site number'
	,'LANG_LocModTool_IDLabel'=>'Old site number'
	,'LANG_CommonLocationNameLabel' => 'Site number'
	,'LANG_LocModTool_NameLabel'=>'New site number'
	,'LANG_CommonFilterNameLabel'=>'Existing site number'
	,'Dragonfly Survey Method'=>'Survey method'
	,'Dragonfly stage'=>'Stage'
	,'LANG_CommuneLookUpFailed' => 'Internal error: Lookup of the Commune for this location failed.'
	,'LANG_PositionInDifferentCommune' => 'The position you have chosen is outside the selected Commune. Do you wish to change the Commune field to match the point?'
	,'LANG_NumSites'=>'Number of sites in this square'
	,'LANG_Data_Download' => 'These reports provide details of the data entered in the survey.'
	,'LANG_Date_Explanation' => '(Indicate the date of the beginning of the survey)'
	,'LANG_FirstChooseParentFilter' => 'First choose a square'
	,'LANG_ZoomToParent'=>'Zoom to Commune'
	,'Add species to list'=>'Add species record to list'

	,'LANG_PositionOutsideCommune_1' => "The position you have chosen is outside the set of allowable Communes. You will not be able to save this location until you change it so it has a valid Commune."
	,'LANG_PositionOutsideCommune_2' => "The position you have chosen is outside the set of allowable Communes. Select 'OK' if you wish to keep the currently selected Commune value. (If you choose 'Cancel', the Commune will be cleared, and you will not be able to save this location until you change it so it has a valid Commune field)"
	,'LANG_PositionOutsideCommune_3' => "The position you have chosen is outside the set of allowable Communes, and is greater then {DISTANCE} metres from a Commune. You will not be able to save this location until you change it so it has a valid Commune, or is within {DISTANCE} metres of a Commune."
	,'LANG_PositionOutsideCommune_4' => "The position you have chosen is outside the set of allowable Communes, and is greater then {DISTANCE} metres from a Commune. Select 'OK' if you wish to keep the currently selected Commune value. (If you choose 'Cancel', the Commune will be cleared, and you will not be able to save this location until you change it so it has a valid Commune field)"
	,'LANG_PositionOutsideCommune_5' => "The position you have chosen is outside the set of allowable Communes, but the closest is SHAPE. Select 'OK' if you wish to use SHAPE. (If you choose 'Cancel' you will not be able to save this location until you change it so it has a valid Commune field)"
	,'LANG_PositionOutsideCommune_5A' => "The position you have chosen is outside the set of allowable Communes, but the closest is SHAPE. The Commune field will be set to SHAPE."
	,'LANG_PositionOutsideCommune_6' => "The position you have chosen is outside the set of allowable Communes, but the closest is SHAPE, which is also the currently selected value in the Commune field. Select 'OK' if you wish to keep SHAPE. (If you choose 'Cancel', the Commune will be cleared, and you will not be able to save this location until you change it so it has a valid Commune field)"
	,'LANG_PositionOutsideCommune_7' => "The position you have chosen is outside the set of allowable Communes, but the closest is SHAPE. This differs from the currently selected value in the Commune field of OLD. Select 'OK' if you wish to change the value to SHAPE. If you choose 'Cancel', the Commune will be left as its current value of OLD."
);
?>