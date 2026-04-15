import { GettingCurrnentTemp } from './FetchFunctions';


const currenttemp = await GettingCurrnentTemp();
// indhenter nuværende temprature data fra serveren
export function CurrentTemp() {
    return (
        <div className='temp'>
            {currenttemp !== undefined ? `${currenttemp}°C` : "Loading..."}
        </div>
    );
}