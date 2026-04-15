import React, { useState, useEffect, useRef } from 'react'
import {
  Chart as ChartJS,
  CategoryScale,
  LinearScale,
  PointElement,
  LineElement,
  Title,
  Tooltip,
  Legend,
  Filler
} from 'chart.js'
import { Line } from 'react-chartjs-2'

// Register Chart.js components (tree-shakable)
ChartJS.register(
  CategoryScale,
  LinearScale,
  PointElement,
  LineElement,
  Title,
  Tooltip,
  Legend,
  Filler
)
function makeInitialData(points = 20) {
  const labels = []
  const values = []
  const now = Date.now()
  for (let i = points - 1; i >= 0; i--) {
    labels.push(new Date(now - i * 1000).toLocaleTimeString('da-DK'))
    values.push(Math.round(50 + Math.random() * 50))
  }
  return { labels, values }
}

export default function LiveLineChart() {
  const WINDOW_SIZE = 20
  const [running, setRunning] = useState(true)
  const [intervalMs, setIntervalMs] = useState(1000)
  const [chartData, setChartData] = useState(() => {
    const init = makeInitialData(WINDOW_SIZE)
    return {
      labels: init.labels,
      datasets: [
        {
          label: 'Live value',
          data: init.values,
          borderColor: 'rgba(75,192,192,1)',
          backgroundColor: 'rgba(75,192,192,0.2)',
          fill: true,
          tension: 0.25,
          pointRadius: 3
        }
      ]
    }
  })

  const timerRef = useRef(null)

  // Append a new random point
  const pushPoint = () => {
    setChartData(prev => {
      const nextLabels = prev.labels.slice()
      const nextData = prev.datasets[0].data.slice()
      nextLabels.push(new Date().toLocaleTimeString('da-DK'))
      nextData.push(Math.round(50 + Math.random() * 50))
      // keep window size
      if (nextLabels.length > WINDOW_SIZE) {
        nextLabels.shift()
        nextData.shift()
      }
      return {
        ...prev,
        labels: nextLabels,
        datasets: [
          {
            ...prev.datasets[0],
            data: nextData
          }
        ]
      }
    })
  }

  // Start/stop interval
  useEffect(() => {
    if (running) {
      timerRef.current = setInterval(pushPoint, intervalMs)
    } else if (timerRef.current) {
      clearInterval(timerRef.current)
      timerRef.current = null
    }
    return () => {
      if (timerRef.current) {
        clearInterval(timerRef.current)
        timerRef.current = null
      }
    }
  }, [running, intervalMs])

  const options = {
    responsive: true,
    maintainAspectRatio: false,
    plugins: {
      title: { display: true, text: 'Live updating values' },
      legend: { display: false },
      tooltip: { mode: 'index', intersect: false }
    },
    interaction: { mode: 'nearest', axis: 'x', intersect: false },
    scales: {
      x: { display: true, title: { display: true, text: 'Time' } },
      y: {
        display: true,
        title: { display: true, text: 'Value' },
        beginAtZero: false
      }
    }
  }

  return (
    <div>
      <div style={{ height: 420 }}>
        <Line data={chartData} options={options} />
      </div>

      <div style={{
        display: 'flex', flexDirection: 'row', justifyContent: 'space-around', gap: 12,
        alignItems: 'center', marginBottom: 12, color: '#f0f0f0', backgroundColor: '#0f0f0f'
      }}>
        <button onClick={() => setRunning(r => !r)}>{running ? 'Stop' : 'Start'}</button>
        <label style={{ color: '#f0f0f0', backgroundColor: '#0f0f0f' }}>
          Interval (ms):
          <input
            type="number"
            value={intervalMs}
            onChange={e => setIntervalMs(Math.max(100, Number(e.target.value) || 100))}
            style={{ width: 100, marginLeft: 8 }}
          />
        </label>
        <button
          onClick={() =>
            setChartData(prev => {
              // reset to fresh random window
              const init = makeInitialData(WINDOW_SIZE)
              return {
                labels: init.labels,
                datasets: [{ ...prev.datasets[0], data: init.values }]
              }
            })
          }
        >
          Reset
        </button>
      </div>
    </div>
  )
}
