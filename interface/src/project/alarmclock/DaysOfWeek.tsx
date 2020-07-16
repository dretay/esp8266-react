import React from 'react'
import {
    ToggleButton,
    ToggleButtonGroup
} from '@material-ui/lab';

import { makeStyles } from '@material-ui/core/styles';

import * as _ from 'underscore';

const useStyles = makeStyles((theme) => ({
    root: {
        minWidth: "2em",
        width: "2em",
    },
}));

type Props = {
    value: string,
    onChange?: (newValue: any) => void
    
};
function EditableDaysOfWeek({
    value,
    onChange
}: Props) {
    const classes = useStyles();
    let enabledDays:string[] = [];
    if(_.isString(value)){
        enabledDays = value.split(",");
        enabledDays = enabledDays.filter( e => (e!= 'null' && e!=null));
    }
    const [days, setDays] = React.useState(enabledDays);

    const handleChange = (event: any, newDays: any) => {
        setDays(newDays);
        onChange!(newDays.join(','));
    };

    return (
        <ToggleButtonGroup value={days} onChange={handleChange} >
            <ToggleButton className={classes.root} value="Sun">S</ToggleButton>
            <ToggleButton className={classes.root} value="Mon">M</ToggleButton>
            <ToggleButton className={classes.root} value="Tue">T</ToggleButton>
            <ToggleButton className={classes.root} value="Wed">W</ToggleButton>
            <ToggleButton className={classes.root} value="Thur">T</ToggleButton>
            <ToggleButton className={classes.root} value="Fri">F</ToggleButton>
            <ToggleButton className={classes.root} value="Sat">S</ToggleButton>
        </ToggleButtonGroup>
    );
}

export default function DaysOfWeek({
    value,
    onChange
}: Props) {
    if (typeof onChange == "function") {
        return <EditableDaysOfWeek {...arguments[0]} />
    }
    return <span>{value}</span>
}