import React from 'react'

import {
  Typography,
  Grid,
} from '@material-ui/core';

import AccessAlarmsIcon from '@material-ui/icons/AccessAlarms';
import AirlineSeatIndividualSuiteIcon from '@material-ui/icons/AirlineSeatIndividualSuite';

import AntSwitch from './AntSwitch'

function EditablePicker({
  value=true,
  onChange
}: Props) {

  const [state, setState] = React.useState(value);

  const handleChange = (event: any) => {    
    setState(event.target.checked);
    onChange!(event.target.checked);
  };
  return (
    <Typography component="div">
      <Grid component="label" container alignItems="center" spacing={1}>
        <Grid item>Off</Grid>
        <Grid item>
          <AntSwitch checked={state} onChange={handleChange} />
        </Grid>
        <Grid item>On</Grid>
      </Grid>
    </Typography>
  );
}

type Props = {
  value: boolean
  onChange?: (newValue: any) => void
};
export default function EnabledPicker({
  value,
  onChange
}: Props) {

  if (typeof onChange == "function") {
    return <EditablePicker {...arguments[0]}/>
  }
  else if (value) {
    return <AccessAlarmsIcon color="primary" />
  }
  return <AirlineSeatIndividualSuiteIcon color="disabled" />
}