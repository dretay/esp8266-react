import React, { Component } from 'react';
import { Link, withRouter, RouteComponentProps } from 'react-router-dom';

import {List, ListItem, ListItemIcon, ListItemText} from '@material-ui/core';
import SettingsRemoteIcon from '@material-ui/icons/SettingsRemote';

import { PROJECT_PATH } from '../api';

class ProjectMenu extends Component<RouteComponentProps> {

  render() {
    const path = this.props.match.url;
    return (
      <List>
        <ListItem to={`/${PROJECT_PATH}/demo/`} selected={path.startsWith(`/${PROJECT_PATH}/demo/`)} button component={Link}>
          <ListItemIcon>
            <SettingsRemoteIcon />
          </ListItemIcon>
          <ListItemText primary="Demo Project Poop" />
        </ListItem>
        <ListItem to={`/${PROJECT_PATH}/alarmclock/`} selected={path.startsWith(`/${PROJECT_PATH}/alarmclock/`)} button component={Link}>
          <ListItemIcon>
            <SettingsRemoteIcon />
          </ListItemIcon>
          <ListItemText primary="Alarm Clock" />
        </ListItem>
      </List>
    )
  }

}

export default withRouter(ProjectMenu);
