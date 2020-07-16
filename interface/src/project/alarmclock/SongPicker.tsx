import React from 'react'
import songs from './songs.json'
import * as shortid from 'shortid';
import {
  Select,
  MenuItem
} from '@material-ui/core';
type Props = {
  value: number,
  onChange?: (newValue: any) => void,
};

function getSongTitle(songIndex:number):string{
  let song = songs.find(x => x.value === songIndex);
  if(song === undefined){
    return "Unknown";
  }
  return song.title;
}
export default function SongPicker({
  value=0,
  onChange
}: Props) {
  const [song, setSong] = React.useState(value);
  const handleChange = function(e:any){
    let value = parseInt(e.target.value);
    setSong(value);
    onChange!(value);
  };
  const items = []
  for (let value of songs) {
    items.push(
      <MenuItem
        key={shortid.generate()}
        value={value.value}>
        {value.title}
      </MenuItem>
    );
  }
  
  if (typeof onChange == "function") {
    return (
      <Select
        labelId="demo-simple-select-label"
        id="demo-simple-select"
        onChange={handleChange}
        value={song}
      >
        {items}
      </Select>
    );
  }
  else {

    return (
      <p>{getSongTitle(song)}</p>
    )
  }
}