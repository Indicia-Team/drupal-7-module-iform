<?php
/**
 * @file
 * Include file to define functions for signing up to recording groups.
 */

/**
 * Menu handler for the join/* path. Provide the name of the group in lowercase
 * with spaces converted to hyphens and all non-alphanumerics removed. Then the
 * group can be looked up. If the user is already signed in, then they will be
 * joined to the group. If the user is not already signed in, then they are
 * told to register and click on the link again.
 * Arguments passed to the function are the title of the group, lowercased with
 * non-alphanumerics stripped apart from spaces which are replaced by hyphens.
 * If a group has a parent, then this parent's title can be precede the group
 * title as a separate parameter (helping to disambiguate group names).
 * @return string
 */
function iform_join_group() {
  watchdog('groups', 'in iform_join_group');
  iform_load_helpers(array('data_entry_helper', 'report_helper'));
  $args = func_get_args();

  if (empty($args)) {
    drupal_set_message(t('The link you have visited is invalid as it does not specify which group to join.'));
    hostsite_goto_page('<front>');
  }
  $auth = data_entry_helper::get_read_write_auth(variable_get('indicia_website_id'), variable_get('indicia_password'));
  $title = array_pop($args);
  $indiciaUserId = hostsite_get_user_field('indicia_user_id', 0);
  $params = array(
    'title' => $title,
    'currentUser' => $indiciaUserId
  );
  if (count($args))
    $params['parent_title'] = array_pop($args);
  // look up the group.
  $groups = report_helper::get_report_data(array(
    'dataSource' => 'library/groups/find_group_by_url',
    'readAuth' => $auth['read'],
    'extraParams' => $params
  ));
  watchdog('groups', 'Groups' . var_export($groups, true));
  if (isset($groups['error'])) {
    drupal_set_message('An error occurred when trying to access the group');
    watchdog('groups', 'Groups page load error: ' . var_export($groups, true));
    hostsite_goto_page('<front>');
    return;
  }
  if (!count($groups)) {
    drupal_not_found();
    return;
  }
  if (count($groups) > 1) {
    drupal_set_message(t('The group you are trying to join has a duplicate name with another group so cannot be joined in this way.'));
    hostsite_goto_page('<front>');
  }
  $group = $groups[0];
  if ($group['member']==='t') {
    drupal_set_message(t("Welcome back to the @group.", array('@group' => iform_readable_group_title($group))));
    return iform_show_group_page($group, $auth['read']);
  }
  elseif ($group['joining_method_raw']==='I') {
    drupal_set_message(t('The group you are trying to join is private.'));
    hostsite_goto_page('<front>');
    return;
  }
  global $user;
  if ($user->uid) {
    // User is logged in
    if (!$indiciaUserId) {
      drupal_set_message(t("Before joining $group[title], please set your surname on your user account profile."));
      hostsite_goto_page('<front>');
      return;
    }
    elseif ($group['pending']==='t' && $group['joining_method'] !== 'P') {
      // Membership exists but is pending.
      drupal_set_message(t('Your application to join @group is still waiting for a group administrator to approve it.', array('@group' => iform_readable_group_title($group))));
    }
    elseif (!iform_join_public_group($group, $auth['write_tokens'], $indiciaUserId)) {
      hostsite_goto_page('<front>');
      return;
    }
    return iform_show_group_page($group, $auth['read']);
  }
  else {
    // User is not logged in, so redirect to login page with parameters so we know which group
    hostsite_goto_page('user', array('query' => array('group_id'=>$group['id'], 'destination' => $_GET['q'])));
  }
}

/**
 * Displays a HTML block that describes a group, including the logo, title, description
 * and available page links.
 * @param $group
 * @param $readAuth
 * @param string $group_home_path
 * @param string $groups_list_path
 * @return string
 */
function iform_show_group_page($group, $readAuth, $group_home_path='', $groups_list_path='') {
  $path = data_entry_helper::get_uploaded_image_folder();
  $img = empty($group['logo_path']) ? '' : "<img style=\"width: 20%; float: left; padding-right: 5%\" alt=\"Logo\" src=\"$path$group[logo_path]\"/>";
  $r = '<div class="clearfix">' . $img . '<div style="float: left; width: 70%;">' .
      "<h3>$group[title]</h3><p class=\"group-description\">$group[description]</p>";
  $nonMembers = '';
  $adminFlags = array('');
  if (empty($group['member']) || $group['member']==='f')
    $nonMembers = ' non-members of';
  elseif (!empty($group['member']) && $group['member']==='t')
    $adminFlags[] = 'f';
  if (!empty($group['administrator']) && $group['administrator']==='t')
    $adminFlags[] = 't';

  // load the available pages
  $pages = data_entry_helper::get_population_data(array(
    'table' => 'group_page',
    'extraParams' => $readAuth + array(
        'group_id' => $group['id'],
        'website_id' => variable_get('indicia_website_id'),
        'query' => json_encode(array('in'=>array('administrator'=>$adminFlags)))
      )
  ));
  if (count($pages)) {
    $pageList = array();
    foreach ($pages as $page)
      $pageList[] = "<li><a class=\"button\" href=\"" .
        hostsite_get_url($page['path'], array(
          'group_id' => $group['id'],
          'implicit' => $group['implicit_record_inclusion']
        )) .
        "\">$page[caption]</a></li>";
    $pageHtml = '<ul>' . implode('', $pageList) . '</ul>';
    $r .= "<fieldset><legend>Pages</legend><p>" .
        t("The following links are available for$nonMembers the @group:",
          array('@group' => iform_readable_group_title($group))) . "</p>$pageHtml</fieldset>";
  }
  return $r;
}

/**
 * Having successfully joined a group, show the success and a list of ongoing options.
 * @param $group
 * @param $auth
 * @param bool $alreadyAMember
 * @param string $group_home_path
 * @param string $groups_list_path
 * @return string
 */
function iform_show_group_join_success($group, $auth, $alreadyAMember=false, $group_home_path='', $groups_list_path='') {
  $pageData = data_entry_helper::get_population_data(array(
    'table'=>'group_page',
    'extraParams' => $auth['read'] + array(
        'group_id' => $group['id'],
        'query' => json_encode(array('in'=>array('administrator'=>array('', 'f')))),
        'orderby' => 'caption'
      )
  ));
  $welcome = t("Welcome to the @group!", array('@group' => iform_readable_group_title($group)));
  if (count($pageData)===1) {
    // Only one page so go straight to it
    if (!$alreadyAMember)
      hostsite_show_message($welcome);
    hostsite_goto_page($pageData[0]['path']);
  } else {
    // Multiple pages, let the user choose
    $intro = '';
    $r = '';

    if (!$alreadyAMember) {
      hostsite_set_page_title($welcome);
      $intro = t("You've successfully joined the @group on @site.",
        array('@group' => iform_readable_group_title($group), '@site' => variable_get('site_name'))) . ' ';
    }
    $path = data_entry_helper::get_uploaded_image_folder();
    $r .= empty($group['logo_path']) ? '' : "<img style=\"width: 20%; float: left; padding: 0 5% 5%;\" alt=\"Logo\" src=\"$path$group[logo_path]\"/>";
    $r .= '<div style="float: left; width: 70%;">' .
        "<h3>$group[title]</h3><div class=\"group-description\">$group[description]</div><p>$intro" .
        t('You can access the following pages for the @group', array('@group' => iform_readable_group_title($group))) .
        ':</p>';
    $r .= '<ul>';
    if (!empty($group_home_path))
      $r .= '<li><a href="'.hostsite_get_url($group_home_path, array('group_id'=>$group['id'], 'implicit'=>$group['implicit_record_inclusion'])).'">'.lang::get("Visit the $group[title] home page").'<a></li>';
    foreach ($pageData as $page) {
      $class = preg_replace('/[^a-zA-Z0-9]/', '-', strtolower($page['path']));
      $r .= '<li class="' . $class . '"><a class="button" href="' . hostsite_get_url($page['path'], array(
          'group_id' => $group['id'],
          'implicit' => $group['implicit_record_inclusion']
        )) . '">' . lang::get($page['caption']) . '</a></li>';
    }
    if (!empty($groups_list_path))
      $r .= '<li><a href="'.hostsite_get_url($groups_list_path).'">'.lang::get("Return to your recording groups list").'<a></li>';
    $r .= '</ul></div>';
    return $r;
  }
}

/**
 * Take account of the different way group titles are written to make it easier to create readable
 * sentences about a group. Basically adds " group" to the end of the group title if not already
 * there.
 * @param array $group Group record loaded from database
 */
function iform_readable_group_title($group) {
  $r = $group['title'];
  if (!preg_match('/ ' . t('group') . '$/', $r))
    $r .= ' ' . t('group');
  return $r;
}

/**
 * Joins a given user to a recording group. After joining, shows a list of options related to the group
 * or redirects to the group's page if there is only one.
 * @param array $group Group data loaded from the database. Will be updated with new
 * membership status.
 * @param array $writeAuth Write authorisation tokens
 * @param $indiciaUserId User's warehouse user ID
 * remove the pending flag.
 * @return bool True if joining was successful.
 */
function iform_join_public_group(&$group, $writeAuth, $indiciaUserId) {
  global $user;
  $values = array(
    'website_id' => variable_get('indicia_website_id'),
    'groups_user:group_id' => $group['id'],
    'groups_user:user_id' => $indiciaUserId,
    'groups_user:username' => $user->name,
    // pending if group is by request
    'groups_user:pending' => $group['joining_method_raw'] === 'P' ? 'f' : 't'
  );
  if (!empty($group['groups_user_id'])) {
    // existing record to update?
    $values['groups_user:id'] = $group['groups_user_id'];
  }
  $s = submission_builder::build_submission($values, array('model'=>'groups_user'));
  $r = data_entry_helper::forward_post_to('save', $s, $writeAuth);
  if (isset($r['success'])) {
    $msg = $group['joining_method'] === 'R'
      ? 'Your request to join @group has been logged and is waiting for approval.'
      : 'Welcome, you are now a member of @group!';
    drupal_set_message(t($msg, array('@group' => iform_readable_group_title($group))));
    // update the new membership status in the group object.
    if ($group['joining_method'] === 'R')
      $group['pending'] = 't';
    else
      $group['member'] = 't';
    return true;
  }
  else {
    drupal_set_message(t("An error occurred whilst trying to join the @group.",
      array('@group'=>iform_readable_group_title($group))), 'warning');
    watchdog('groups', "An error occurred whilst trying to join $group[title] for {$user->name}.");
    watchdog('groups', var_export($r, true));
    return false;
  }
}