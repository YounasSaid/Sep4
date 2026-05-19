import { useState } from 'react'

import './css/DateTimeInput.css'

export function DateTimeInput({defaultTime, sendStartDTToParent, sendSlutDTToParent})
  {
  const [startDateTime, setStartDateTime] = useState(defaultTime[0]);
  const [slutDateTime, setSlutDateTime] = useState(defaultTime[1]);

  // Check SlutTid Er Efter StartTid
  const CheckTid = () =>
    {
    const StartEpoch = new Date(startDateTime).getTime() ;
    const SlutEpoch = new Date(slutDateTime).getTime() ;
    alert(StartEpoch+", "+SlutEpoch+", "+(SlutEpoch >= StartEpoch))
    if (SlutEpoch >= StartEpoch)
      {
      alert("Start Skal Være Efter Slut I Tid !") ;
      return false ;
      }
    else
      return true ;
    }

  const handleStartDT = (e) => 
    {
    setStartDateTime(e.target.value) ;
    sendStartDTToParent(e.target.value) ;
    }

  const handleSlutDT = (e) =>
    {
    setSlutDateTime(e.target.value) ;
    sendSlutDTToParent(e.target.value);
    }

  return (
    <>
    <div className="DateTimeInput Font">Vælg dato og tid</div>
    <div className="DateTimeInput">
      <span className="DTFinput">
        <label htmlFor="startdaytime">Start :&nbsp;</label>
        <input 
          type="datetime-local"
          value={startDateTime}
          id="startdaytime" name="startdaytime" 
          onInput={handleStartDT}
          />
      </span>
      <span className="DTFinput">
        <label htmlFor="slutdaytime">Slut :&nbsp;</label>
        <input 
          type="datetime-local" 
          value={slutDateTime}
          id="slutdaytime" name="slutdaytime" 
          onChange={handleSlutDT}
          />
      </span>
    </div>
    </>
    ) ;
  }
