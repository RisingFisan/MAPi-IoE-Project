import Chart from 'chart.js/auto';

type SensorTrendPayload = {
  labels: string[];
  values: Array<number | null>;
  accentColor?: string;
  metricLabel?: string;
};

type SensorTrendElement = HTMLElement & {
  dataset: DOMStringMap & {
    sensorTrendInitialized?: string;
  };
};

const initialized = new WeakSet<HTMLElement>();

const parsePayload = (element: HTMLScriptElement | null): SensorTrendPayload | null => {
  if (!element) return null;
  try {
    return JSON.parse(element.textContent || '{}');
  } catch (error) {
    console.error('SensorTrend: invalid payload', error);
    return null;
  }
};

const createGradient = (ctx: CanvasRenderingContext2D, color: string, height: number) => {
  const gradient = ctx.createLinearGradient(0, 0, 0, height || 240);
  gradient.addColorStop(0, `${color}33`);
  gradient.addColorStop(1, `${color}00`);
  return gradient;
};

const renderChart = (container: SensorTrendElement) => {
  if (initialized.has(container)) return;

  const payloadNode = container.querySelector<HTMLScriptElement>('script[data-sensor-payload]');
  const canvas = container.querySelector<HTMLCanvasElement>('canvas[data-sensor-canvas]');

  if (!payloadNode || !canvas) return;

  const payload = parsePayload(payloadNode);
  if (!payload || !Array.isArray(payload.labels)) return;

  const ctx = canvas.getContext('2d');
  if (!ctx) return;

  const accentColor = payload.accentColor ?? '#3b82f6';
  const gradient = createGradient(ctx, accentColor, canvas.offsetHeight);

  const chart = new Chart(ctx, {
    type: 'line',
    data: {
      labels: payload.labels,
      datasets: [
        {
          label: payload.metricLabel ?? 'Value',
          data: payload.values,
          borderColor: accentColor,
          backgroundColor: gradient,
          fill: true,
          borderWidth: 3,
          pointRadius: 0,
          pointHitRadius: 20,
          tension: 0.35,
        },
      ],
    },
    options: {
      responsive: true,
      maintainAspectRatio: false,
      interaction: {
        mode: 'index',
        intersect: false,
      },
      scales: {
        x: {
          ticks: {
            maxRotation: 0,
            color: '#94a3b8',
          },
          grid: {
            display: false,
          },
        },
        y: {
          ticks: {
            color: '#94a3b8',
          },
          grid: {
            color: 'rgba(148, 163, 184, 0.25)',
          },
        },
      },
      plugins: {
        legend: {
          display: false,
        },
        tooltip: {
          displayColors: false,
          callbacks: {
            label: (item) => `${item.formattedValue}`,
          },
        },
      },
    },
  });

  container.dataset.sensorTrendInitialized = 'true';
  initialized.add(container);
};

const initAll = () => {
  document
    .querySelectorAll<SensorTrendElement>('[data-component="sensor-trend"]')
    .forEach(renderChart);
};

if (document.readyState === 'loading') {
  document.addEventListener('DOMContentLoaded', initAll, { once: true });
} else {
  initAll();
}

const observer = new MutationObserver(() => {
  initAll();
});

observer.observe(document.documentElement, {
  childList: true,
  subtree: true,
});
