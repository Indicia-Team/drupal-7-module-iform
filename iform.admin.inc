<?php
/**
* @file
* Include file to define the admin interface for the iform module.
*/

/** 
 * A form definition for providing global configuration options for Indicia. Used both in the installation
 * profile as an install task, and provided on the admin menu.
 */
function iform_configuration_form($form, &$form_state, $url='', $submit_handler='') {
  iform_load_helpers(array('map_helper', 'data_entry_helper'));
  global $_iform_warehouses;
  iform_load_warehouse_array();
  foreach ($_iform_warehouses as $warehouse => $def) 
    $warehouses[$warehouse] = $def['title'];
  $warehouses['other'] = t('Other');
  $form['indicia_warehouse'] = array(
    '#type' => 'radios',
    '#title' => t('Indicia Warehouse'),
    '#options' => $warehouses,
    '#description' => t('Select the Indicia Warehouse to connect to, or select Other and enter the details in the Warehouse URL and GeoServer URL fields.'),
    '#required' => TRUE,
    '#default_value' => variable_get('indicia_warehouse', ''),
  );
  $form['other_warehouse'] = array(
    '#type' => 'fieldset',
    '#attributes' => array('id' => 'warehouse_details'),
    '#collapsible' => TRUE,
    '#collapsed' => variable_get('indicia_warehouse', '')!='other',
    '#title' => t('Other Warehouse Details')
  );
  $form['other_warehouse']['indicia_base_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Warehouse URL'),
    '#description' => t('If selecting Other for the Indicia Warehouse option, please enter the URL of the Indicia Warehouse you are connecting to, otherwise ignore this setting. ' .
        'This should include the full path and trailing slash but not the index.php part, e.g. "http://www.mysite.com/indicia/".'),
    '#maxlength' => 255,
    '#required' => FALSE,
    '#default_value' => variable_get('indicia_base_url', ''),
  );
  $form['other_warehouse']['indicia_geoserver_url'] = array(
    '#type' => 'textfield',
    '#title' => t('GeoServer URL'),
    '#description' => t('If selecting Other for the Indicia Warehouse option, please enter the URL of the GeoServer instance you are connecting to, otherwise ignore this setting. ' .
        'This is optional, if not specified then you will not be able to use some of the advanced mapping facilities provided by GeoServer.'),
    '#maxlength' => 255,
    '#required' => FALSE,
    '#default_value' => variable_get('indicia_geoserver_url', ''),
  );
  $form['indicia_private_warehouse'] = array(
    '#type' => 'checkbox',
    '#title' => t('Warehouse is private'),
    '#description' => t('If your warehouse is not publicly visible (e.g. behind a firewall) then as long as it accepts requests from the IP address of the Drupal website\'s server ' .
        'you can tick this box to send requests to the warehouse via a proxy on the Drupal server.'),
    '#required' => FALSE,
    '#default_value' => variable_get('indicia_private_warehouse', FALSE),
  );
  $form['indicia_allow_connection_override'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow website connection override'),
    '#description' => t('Tick this box to allow forms to override the specified website ID and password on an individual basis. This allows a single Drupal installation '.
        'to support forms which link to multiple Indicia website registrations. Leave unticked if you are not sure.'),
    '#required' => FALSE,
    '#default_value' => variable_get('indicia_allow_connection_override', false),
  );
  $form['indicia_website_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Indicia Website ID'),
    '#description' => t('Please enter the numeric ID given to your website record when your website was registered on the Indicia Warehouse.'),
    '#size' => 10,
    '#maxlength' => 10,
    '#required' => TRUE,
    '#default_value' => variable_get('indicia_website_id', ''),
  );
  // require the password only if not previously set.
  $pwd_required = (variable_get('indicia_password', '')=='');
  if ($pwd_required)
    $pwd_description = t('Please enter the password specified when your website was registered on the Indicia Warehouse.');
  else
    $pwd_description = t('If you need to change it, enter the password specified when your website was registered on the Indicia Warehouse. ' .
        'Otherwise leave the password blank to keep your previous settings.');
  $form['indicia_password'] = array(
    '#type' => 'password_confirm',
    '#description' => check_plain($pwd_description),
    '#required' => $pwd_required,
  );
  $form['api_keys'] = array(
    '#type' => 'fieldset',
    '#title' => t('API Keys')
  );
  $form['api_keys']['indicia_geoplanet_api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('GeoPlanet API Key'),
    '#description' => t('The Yahoo! GeoPlanet API is one option to lookup place names when you use the place search control. ' .
        'It references a global database of places and returns the list of possibilities with their spatial references ' .
        'to Indicia. To obtain your own API key for GeoPlanet, please visit <a target="_blank" href="http://developer.yahoo.com/geo/geoplanet/">' .
        'Yahoo! GeoPlanet</a> and follow the link to get an Application ID. '),
    '#required' => FALSE,
    '#default_value' => variable_get('indicia_geoplanet_api_key', helper_config::$geoplanet_api_key),
  );
  $form['api_keys']['indicia_google_api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Google API Key'),
    '#description' => t('The Google API provides the Places API text search, another option to lookup place names ' .
        'when you use the place search control. It references a global database of places and returns the list of ' .
        'possibilities with their spatial references to Indicia. To obtain your own API key for the Google API, ' .
        'please visit <a target="_blank" href="https://code.google.com/apis/console"> the Google APIs Console</a>.'),
    '#required' => FALSE,
    '#default_value' => variable_get('indicia_google_api_key',
        isset(helper_config::$google_api_key) ? helper_config::$google_api_key : ''),
  );
  $form['api_keys']['indicia_google_maps_api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Google Maps API Key'),
    '#description' => t('The Google Maps API key is required to allow use of Google Maps layers but can be left ' .
        'blank if you do not intend to use Google maps. To obtain your own API key for the Google Maps API, please ' .
        'visit <a target="_blank" href="https://developers.google.com/maps/documentation/javascript/get-api-key#key"> ' .
        'the Google APIs documentation.</a>.'),
    '#required' => FALSE,
    '#default_value' => variable_get('indicia_google_maps_api_key',
        isset(helper_config::$google_maps_api_key) ? helper_config::$google_maps_api_key : ''),
  );
  $form['api_keys']['indicia_bing_api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Bing API Key'),
    '#description' => t('The Bing API key is required to allow use of Bing map layers but can be left blank if you ' .
        'do not intend to use Bing maps. To obtain your own key, please visit the ' .
        '<a target="_blank" href="http://www.bingmapsportal.com/">Bing Maps Account Center</a>. Please ensure that ' .
        'you read and adhere to the <a href="http://www.microsoft.com/maps/product/terms.html">terms of use</a>.'),
    '#required' => FALSE,
    '#default_value' => variable_get('indicia_bing_api_key', ''),
  );
  $form['map'] = array(
    '#type' => 'fieldset',
    '#title' => t('Map Settings')
  );
  $form['map']['instruct'] = array(
    '#markup' => '<p>' . t('Pan and zoom this map to set the default map position for your survey input and mapping pages.') . '</p>'
  );
  $form['map']['panel'] = array(
    '#markup' => map_helper::map_panel(array(
      'width' => '100%',
      'height' => 500,
      'presetLayers' => array('osm'), 
      'editLayer' => FALSE,
      'layers' => array(),
      'initial_lat' => variable_get('indicia_map_centroid_lat', 55),
      'initial_long' => variable_get('indicia_map_centroid_long', -1),
      'initial_zoom' => variable_get('indicia_map_zoom', 5),
      'standardControls' => array('panZoomBar'),
      'scroll_wheel_zoom' => 'false'
    ))
  );
  $form['map']['indicia_map_centroid_lat'] = array(
    '#attributes'=>array('id'=>'edit-indicia-map-centroid-lat'),
    '#type' => 'hidden',
    '#default_value' => variable_get('indicia_map_centroid_lat', 55),
  );
  $form['map']['indicia_map_centroid_long'] = array(
    '#attributes'=>array('id'=>'edit-indicia-map-centroid-long'),
    '#type' => 'hidden',
    '#default_value' => variable_get('indicia_map_centroid_long', -1),
  );
  $form['map']['indicia_map_zoom'] = array(
    '#attributes'=>array('id'=>'edit-indicia-map-zoom'),
    '#type' => 'hidden',
    '#default_value' => variable_get('indicia_map_zoom', 6),
  );
  $form['map']['spatial_ref_systems'] = array(
    '#type' => 'fieldset',
    '#title' => t('List of spatial or grid reference systems'),
    '#description' => 'Please tick off each spatial or grid reference system you wish to enable for input when using this website.',
    '#tree' => true
  );
  $systems = array(
      'OSGB' => t('British National Grid'),
      'OSIE' => t('Irish National Grid'),
      '4326' => t('GPS Latitude and Longitude (WGS84)'),
      'guernsey' => t('Guernsey Grid'),
      'jersey' => t('Jersey Grid'),
      'utm30ed50' => t('UTM 30N (ED50)'),
      'utm30wgs84' => t('UTM 30N (WGS84)'),
      '2169' => t('LUREF Luxembourg')
  );
  $selected_systems = _indicia_form_values_from_sref_systems($systems);
  $form['map']['spatial_ref_systems']['spatial_ref_systems_list'] = array(
    '#type' => 'checkboxes',
    '#default_value' => $selected_systems['list'],
    '#options' => $systems
  );
  $form['map']['spatial_ref_systems']['spatial_ref_systems_other'] = array(
    '#type' => 'textfield',
    '#title' => t('Other'),
    '#default_value' => $selected_systems['other'],
    '#description' => t('For any system not in this list, you can enter a comma separated list of EPSG codes or other system names as long as they are ' .
        'recognised by the Indicia Warehouse you are using.')
  );
  // Add any module declared additional iform variables to the settings form. Extra variables only 
  // visible after install, as they may need a website connection
  if ((!defined('MAINTENANCE_MODE') || MAINTENANCE_MODE != 'install')  && variable_get('indicia_website_id', false)) {
    try {
      $readAuth = data_entry_helper::get_read_auth(variable_get('indicia_website_id', 0), variable_get('indicia_password', ''));
      // Add any indicia variables declared by hook_variable_info
      global $language;
      $vars = module_invoke_all('variable_info', array("language" => $language));
      $ctrls = array();
      foreach ($vars as $var => $config) {
        $matches = array();
        if (isset($config['addToIndiciaSettingsPage']) && $config['addToIndiciaSettingsPage']) {
          if (isset($config['subtype']) && preg_match('/^termlists_term:(?P<list>.+)/', $config['subtype'], $matches)) {
            $type = 'select';
          }
          elseif (isset($config['subtype']) && preg_match('/^(?P<table>[a-z_]+)\.(?P<caption>[a-z_]+)/', $config['subtype'], $matches)) {
            $type = 'select';
          }
          else {
            $type = ($config['type'] == 'radios' ? 'radios' : 'textfield');
          }
          $ctrls[$var] = array(
            '#type' => $type,
            '#title' => $config['title'],
            '#description' => $config['description'],
            '#default_value' => variable_get($var, isset($config['default']) ? $config['default'] : '')
          );
          if (isset($config['options'])) {
            $ctrls[$var]['#options'] = $config['options'];
          }
          try {
            // output buffer used to catch and discard errors as we'll handle them ourselves and don't want them on screen.
            ob_start();
            if (isset($matches['list']) || isset($matches['table'])) {
              if (isset($matches['list'])) {
                $rows = data_entry_helper::get_population_data(array(
                  'table' => 'termlists_term',
                  'extraParams' => $readAuth + array('termlist_external_key' => 'indicia:' . $matches['list'])
                ));
                $captionField = 'term';
              }
              else {
                $rows = data_entry_helper::get_population_data(array(
                  'table' => $matches['table'],
                  'extraParams' => $readAuth,
                  'columns' => 'id,' . $matches['caption']
                ));
                $captionField = $matches['caption'];
              }
              $options = array('' => t('<please select>'));
              foreach ($rows as $row) {
                if (!isset($row['id']))
                  drupal_set_message('Dodgy: ' . var_export($row, true));
                $options[$row['id']] = $row[$captionField];
              }
              $ctrls[$var]['#options'] = $options;
            }
            ob_end_clean();
          } catch (Exception $e) {
            ob_end_clean();
            watchdog('iform', 'Error loading iform settings for ' . $config['title'] . ': ' . $e->getMessage());
            drupal_set_message("Unable to load the control $config[title], probably because the warehouse connection is not correctly configured. ");
            unset($ctrls[$var]);
          }
        }
      }
      if (!empty($ctrls)) {
        $form['additional'] = array_merge(array(
          '#type' => 'fieldset',
          '#title' => t('Additional Settings')
        ), $ctrls);
      }
    } catch (Exception $e) {
      // Handle warehouse not found
      if ($e->getCode()===404) {
        drupal_set_message('The current warehouse URL is incorrect so some settings are not available.', 'warning');
      } else {
        throw $e;
      }
    }
  }
  $form['continue'] = array(
    '#type' => 'submit', 
    '#value' => t('Save'),
    '#validate' => array('indicia_configuration_form_validate'),
  );
  
  // Note that #action is set to the url passed through from
  // installer, ensuring that it points to the same page, and
  // #redirect is FALSE to avoid broken installer workflow.
  $form['errors'] = array();
  if (!empty($url)) {
    $form['#action'] = $url;
    $form['#redirect'] = FALSE;
  }
  if (empty($submit_handler))
    $form['#submit'][] = 'indicia_configuration_form_submit';
  else
    $form['#submit'][] = $submit_handler;
    
  // need some js to trap moving the map, and store the centre + zoom in hidden controls
  map_helper::$javascript .= "
mapInitialisationHooks.push(function(div) {
  div.map.events.on({
    'moveend' : function(evt) {
      var centre = div.map.center.clone();
      centre.transform(div.map.projection, new OpenLayers.Projection('EPSG:4326'));
      $('#edit-indicia-map-centroid-lat').val(centre.lat);
      $('#edit-indicia-map-centroid-long').val(centre.lon);
      $('#edit-indicia-map-zoom').val(div.map.zoom);
    }
  });
});
";
  $form['#attached'] = iform_get_form_resource_attachments();
  $form['#attached']['js'][] = array('data'=>drupal_get_path('module', 'iform').'/iform.admin.js');
  return $form;
}

/** 
 * Implement hook_validate.
 * Checks the warehouse configuration settings are valid.
 * @param array $form 		
 * @param array $form_state
 */
function indicia_configuration_form_validate($form, $form_state) {
  if ($form_state['values']['indicia_warehouse']==='' || ($form_state['values']['indicia_warehouse']=='other' && empty($form_state['values']['indicia_base_url'])))
    form_set_error('indicia_website_id', t('Please supply a warehouse URL for connection to Indicia, or select a pre-configured connection.'));
  else {
    $urls = indicia_get_warehouse_urls($form_state);
    if (variable_get('indicia_base_url', '') !== $urls['base_url'] || !empty($form_state['values']['indicia_password'])) {
      // test the connection to the warehouse
      data_entry_helper::$base_url = $urls['base_url'];
      // clear the cache if the linked warehouse changes
      data_entry_helper::clear_cache();
      try {
        $read_auth = data_entry_helper::get_read_auth($form_state['values']['indicia_website_id'], $form_state['values']['indicia_password']);
        $test = data_entry_helper::get_population_data(array(
          'table' => 'survey',
          'extraParams' => $read_auth + array('limit' => 0),
          'nocache' => TRUE
        ));
        if (isset($test['error'])) {
          form_set_error('indicia_website_id', t('The configuration for the connection to the warehouse is incorrect. This could be an incorrect ' .
            'or unavailable Indicia Warehouse, an incorrect Indicia Website ID or Password.'));
        }
      } catch (Exception $e) {
        // Handle warehouse not found
        if ($e->getCode() === 404) {
          form_set_error('indicia_base_url', 'The warehouse URL is incorrect or points to a warehouse that is not currently reachable.');
        }
        else {
          throw $e;
        }
      }
    }
  }
  $systems = _indicia_sref_systems_from_form($form_state['values']);
  if (empty($systems)) {
    form_set_error('spatial_ref_systems', t('Please enable at least one spatial or grid reference system.'));
  }
}

/**
 * Convert the values in the form array for spatial reference systems into the correct comma separated format for Indicia.
 */
function _indicia_sref_systems_from_form($values) {
  $arr = array();
  // convert the form value array into a simple array of enabled items
  foreach ($values['spatial_ref_systems']['spatial_ref_systems_list'] as $sys => $enabled) {
    if ($enabled) {
      $arr[] = $sys;
    }
  }
  $other = trim($values['spatial_ref_systems']['spatial_ref_systems_other']);
  if (!empty($other))
    $arr[] = $other;
  return implode(',', $arr);
}

/** 
 * Convert the stored value for indicia_spatial_systems into values to use as defaults for controls on the form.
 * @param array $systems The list of spatial systems to map to. Any others go into the array['other'] part of the response.
 * @return array Associative array containing entries called list (an array of available systems) and other
 * (an array of non-standard EPSG codes).
 */
function _indicia_form_values_from_sref_systems($systems) {
  $r = array(
    'list' => array(),
    'other' => array()
  );
  $var = explode(',', variable_get('indicia_spatial_systems', 'OSGB'));
  foreach ($var as $sys) {
    // check if this is one on the list, or should go in other
    if (isset($systems[$sys]))
      $r['list'][] = $sys;
    else
      $r['other'][] = $sys;
  }
  // implode the other systems into a comma separated list.
  $r['other'] = implode(',', $r['other']);
  return $r;
}


function indicia_configuration_form_submit($form, &$form_state) {
  variable_set('indicia_warehouse', $form_state['values']['indicia_warehouse']);
  $urls = indicia_get_warehouse_urls($form_state);
  variable_set('indicia_base_url', $urls['base_url']);
  variable_set('indicia_geoserver_url', $urls['geoserver_url']);
  variable_set('indicia_private_warehouse', $form_state['values']['indicia_private_warehouse']);
  variable_set('indicia_allow_connection_override', $form_state['values']['indicia_allow_connection_override']);
  variable_set('indicia_website_id', $form_state['values']['indicia_website_id']);
  if (!empty($form_state['values']['indicia_password']))
    variable_set('indicia_password', $form_state['values']['indicia_password']);
  variable_set('indicia_geoplanet_api_key', $form_state['values']['indicia_geoplanet_api_key']);
  variable_set('indicia_google_api_key', $form_state['values']['indicia_google_api_key']);
  variable_set('indicia_google_maps_api_key', $form_state['values']['indicia_google_maps_api_key']);
  variable_set('indicia_bing_api_key', $form_state['values']['indicia_bing_api_key']);
  variable_set('indicia_map_centroid_lat', $form_state['values']['indicia_map_centroid_lat']);
  variable_set('indicia_map_centroid_long', $form_state['values']['indicia_map_centroid_long']);
  variable_set('indicia_map_zoom', $form_state['values']['indicia_map_zoom']);
  // Save any indicia variables declared by hook_variable_info
  global $language;
  $vars = module_invoke_all('variable_info', array("language" => $language));  
  foreach ($vars as $var=>$config) {
    if (!empty($config['addToIndiciaSettingsPage']) && $config['addToIndiciaSettingsPage'] && isset($form_state['values'][$var]))
      variable_set($var, $form_state['values'][$var]);
  }
  $systems = _indicia_sref_systems_from_form($form_state['values']);
  variable_set('indicia_spatial_systems', $systems);
  drupal_set_message(t('Indicia settings saved.'));
}

/**
 * Returns the base url and geoserver url defined in the form state array.
 */
function indicia_get_warehouse_urls($form_state) {
  if (strcasecmp($form_state['values']['indicia_warehouse'], t('Other'))===0) {
    return array(
      'base_url' => $form_state['values']['indicia_base_url'],
      'geoserver_url' => $form_state['values']['indicia_geoserver_url']
    );
  } 
  else {
    global $_iform_warehouses;
    iform_load_warehouse_array();
    foreach ($_iform_warehouses as $warehouse => $def) {
      if ($warehouse==$form_state['values']['indicia_warehouse']) {
        return array(
          'base_url' => $def['base_url'],
          'geoserver_url' => $def['geoserver_url']
        );
      }
    }
    // if not found, something went wrong
    throw new exception('Could not find configuration for selected warehouse.');
  }
}

/**
 * Utility function to populate the list of warehouses in the global $_iform_warehouses. Each warehouse is loaded from an .inc
 * file in the warehouses sub-folder.
 */
function iform_load_warehouse_array() {
  global $_iform_warehouses;
  $_iform_warehouses = array();
  foreach (glob(drupal_get_path('module', 'iform') . '/warehouses/*.inc') as $warehouse_file) {
    require($warehouse_file);
  }
}
