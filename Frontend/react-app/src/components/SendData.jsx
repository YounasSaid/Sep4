import './SendData.css';

function SendData() {
  return (
    <div className="side">
      <div className="indhold">
        <h1 className="titel">Drivhusprojektet</h1>
        <div className="input-boks">
          <label>Indtast højde:</label>
          <input type="number" placeholder="cm" />
        </div>
        <button className="send-knap">Send</button>
      </div>
      <div className="sidebar">
        <a href="#">Mål Højde</a>
        <a href="#">Sensor Grafer</a>
      </div>
    </div>
  );
}

export default SendData;