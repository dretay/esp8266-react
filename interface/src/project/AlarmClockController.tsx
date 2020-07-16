import React, { Component, useEffect } from 'react';

import MaterialTable from 'material-table';

import { ENDPOINT_ROOT } from '../api';
import { restController, RestControllerProps, RestFormLoader, RestFormProps, SectionContent } from '../components';

import { AlarmSettings } from './types';

import DateFnsAdapter from '@date-io/date-fns';

import DaysOfWeek from './alarmclock/DaysOfWeek'
import TimePicker from './alarmclock/TimePicker'
import SongPicker from './alarmclock/SongPicker'
import EnabledPicker from './alarmclock/EnabledPicker'
import * as _ from 'underscore';

import { redirectingAuthorizedFetch } from '../authentication';

const dateFns = new DateFnsAdapter();
export const ALARM_SETTINGS_ENDPOINT = ENDPOINT_ROOT + "alarmState";


type AlarmStateRestControllerProps = RestControllerProps<AlarmSettings>;


class AlarmStateRestController extends Component<AlarmStateRestControllerProps> {

  componentDidMount() {
    this.props.loadData();
  }

  render() {
    return (
      <SectionContent title='My REST Controller' titleGutter>
        <RestFormLoader
          {...this.props}
          render={props => (
            <AlarmStateRestControllerForm {...props} />
          )}
        />
      </SectionContent>
    )
  }
}

export default restController(ALARM_SETTINGS_ENDPOINT, AlarmStateRestController);

type AlarmStateRestControllerFormProps = RestFormProps<AlarmSettings>;

function AlarmStateRestControllerForm(props: AlarmStateRestControllerFormProps) {
  const { data, setData, saveData, loadData, handleValueChange } = props;

  const { useState } = React;

  useEffect(() => {
    console.log("FINISHING UPDATE!!!!!");
  });

  let formatDate = (date: number): string => {
    let parsedDate = new Date();
    let formattedDate = dateFns.format(parsedDate, "hh:mm aa");
    
    if(_.isNumber(date)){
      parsedDate = new Date(date*1000);
      formattedDate = dateFns.format(parsedDate, "hh:mm aa");
    }    
    return formattedDate;
  }
  const handleRowUpdate = (newData:any, oldData:any, resolve:any) => {
    console.log("STARTING UPDATE!!!!!");
    setData({"entries":[newData]}, () => {
      saveData();
    });
  }

  return (
    <MaterialTable
      title="Editable Preview"
      columns={[
        {
          title: 'Song', field: 'song',
          editComponent: props => (
            <SongPicker onChange={props.onChange} value={props.value} />
          ),
          render: rowData => (
            <SongPicker value={rowData.song} />
          )
        },
        {
          title: 'Time', field: 'time',
          editComponent: props => (
            <TimePicker onChange={props.onChange} value={props.value} />
          ),
          render: rowData => (
            formatDate(rowData.time)
          )
        },
        {
          title: 'Days of Week',
          field: 'daysOfWeek',
          editComponent: props => (
            <DaysOfWeek onChange={props.onChange} value={props.value} />
          ),
          render: rowData => (
            <DaysOfWeek value={rowData.daysOfWeek} />
          )
        },
        {
          title: 'Enabled',
          field: 'enabled',
          editComponent: props => (
            <EnabledPicker onChange={props.onChange} value={props.value} />
          ),
          render: rowData => (
            <EnabledPicker value={rowData.enabled} />
          )
        },
      ]}
      data={data.entries}
      editable={{
        onRowAdd: newTableData =>
          new Promise((resolve, reject) => {
            _.defaults(newTableData, {
              song: 0,
              time: JSON.stringify(new Date()),
              daysOfWeek: "",
              enabled: true
            });
            // setTableData([...data.entries, newTableData]);
            resolve();
          }),
        onRowUpdate: (newTableData, oldTableData) =>
          new Promise((resolve, reject) => {
            handleRowUpdate(newTableData, oldTableData, resolve);            
          }),
        onRowDelete: oldTableData =>
          new Promise((resolve, reject) => {
            // const dataDelete = [...tableData];
            // const index = (oldTableData as any).tableData.id;
            // dataDelete.splice(index, 1);
            // setTableData([...dataDelete]);
            resolve();
          }),
      }}
    />
  );
}
