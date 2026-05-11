import { useState } from 'react'

import './css/DateTimeInput.css'

export function DateTimeInput({sendStartDTToParent, sendSlutDTToParent})
  {
  const handleStartDT = (e) => 
    {
    sendStartDTToParent(e.target.value) ;
    }

  const handleSlutDT = (e) =>
    {
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
          id="startdaytime" name="startdaytime" 
          onInput={handleStartDT}
          />
      </span>
      <span className="DTFinput">
        <label htmlFor="slutdaytime">Slut :&nbsp;</label>
        <input 
          type="datetime-local" 
          id="slutdaytime" name="slutdaytime" 
          onChange={handleSlutDT}
          />
      </span>
    </div>
    </>
    ) ;
  }
