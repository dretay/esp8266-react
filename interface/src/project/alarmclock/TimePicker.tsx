import React from 'react'

import {
    MuiPickersUtilsProvider,
    KeyboardTimePicker,
} from '@material-ui/pickers';

import { makeStyles } from '@material-ui/core/styles';

import DateFnsUtils from '@date-io/date-fns';
// import getUnixTime = require('date-fns/getUnixTime')

import * as _ from 'underscore';

const useStyles = makeStyles((theme) => ({
    root: {                        
        width: "9em",
    },
}));

type Props = {
    value: number,
    onChange: (newValue:any) => void
};

export default function TimePicker({
    value,
    onChange
}: Props) {
    const classes = useStyles();
    const dateFnsUtils = new DateFnsUtils();
    
    let parsedDate:string=(new Date()).toString();
    if(_.isNumber(value)){
        let d = new Date(0);
        d.setUTCSeconds(value);
        if(dateFnsUtils.getYear(d) > 2000){
            parsedDate = new Date(value*1000).toString();
        }        
    }

    const handleChange = (date:any)=>{
        let dateObj = new Date(date);
        let epoch = (dateObj.getTime()-dateObj.getMilliseconds())/1000;
        onChange(epoch);
    }

    return (
        <MuiPickersUtilsProvider utils={DateFnsUtils} >
            <KeyboardTimePicker
                className={classes.root}
                margin="dense"
                id="time-picker"
                value={parsedDate}
                onChange={handleChange}
                KeyboardButtonProps={{
                    'aria-label': 'change time',
                }}
            />
        </MuiPickersUtilsProvider>
    );
}