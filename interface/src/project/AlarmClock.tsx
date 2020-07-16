import React, { Component } from 'react';
import { Redirect, Switch, RouteComponentProps } from 'react-router-dom'

import { Tabs, Tab } from '@material-ui/core';

import { PROJECT_PATH } from '../api';
import { MenuAppBar } from '../components';
import { AuthenticatedRoute } from '../authentication';

import AlarmClockController from './AlarmClockController';

class AlarmClock extends Component<RouteComponentProps> {

  handleTabChange = (event: React.ChangeEvent<{}>, path: string) => {
    this.props.history.push(path);
  };

  render() {
    return (
      <MenuAppBar sectionTitle="Alarm Clock">
          <Tabs value={this.props.match.url} onChange={this.handleTabChange} variant="fullWidth">
            <Tab value={`/${PROJECT_PATH}/alarmclock/alarms`} label="Alarms" />
        </Tabs>
        <Switch>
          <AuthenticatedRoute exact path={`/${PROJECT_PATH}/alarmclock/alarms`} component={AlarmClockController} />          
          <Redirect to={`/${PROJECT_PATH}/alarmclock/alarms`} />
        </Switch>
      </MenuAppBar>
    )
  }

}

export default AlarmClock;
