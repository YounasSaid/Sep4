import React from 'react'

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

// Register only the pieces we need (tree-shakable)
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

const labels = [
  '2024-01-01',
  '2024-02-01',
  '2024-03-01',
  '2024-04-01',
  '2024-05-01',
  '2024-06-01'
]

const data = {
  labels,
  datasets: [
    {
      label: 'Visitors',
      data: [120, 190, 170, 220, 260, 300],
      borderColor: 'rgba(75,192,192,1)',
      backgroundColor: 'rgba(75,192,192,0.2)',
      fill: true,
      tension: 0.3,
      pointRadius: 4,
      pointBackgroundColor: 'rgba(75,192,192,1)'
    }
  ]
}

const options = {
  responsive: true,
  maintainAspectRatio: false,
  plugins: {
    title: {
      display: true,
      text: 'Monthly Visitors'
    },
    legend: {
      position: 'top'
    },
    tooltip: {
      mode: 'index',
      intersect: false
    }
  },
  interaction: {
    mode: 'nearest',
    axis: 'x',
    intersect: false
  },
  scales: {
    x: {
      display: true,
      title: {
        display: true,
        text: 'Month'
      },
      grid: {
        display: false
      }
    },
    y: {
      display: true,
      title: {
        display: true,
        text: 'Visitors'
      },
      beginAtZero: true,
      grid: {
        color: 'rgba(0,0,0,0.05)'
      }
    }
  }
}

export default function LineChart() 
{
  return (
    <div style={{ height: 420 }}>
      <Line data={data} options={options} />
    </div>
  )
}
