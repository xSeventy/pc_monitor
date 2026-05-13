async function updateMetrics() {
  const response = await fetch("http://127.0.0.1:8080/metrics");
  const metrics = await response.json();

  const cpuElement = document.querySelector("#cpu-value");
  const memoryElement = document.querySelector("#memory-value");
  const loadElement = document.querySelector("#load-value");

  if (cpuElement) {
    cpuElement.textContent = `${metrics.cpu.usagePercent}%`;
  }

  if (memoryElement) {
    memoryElement.textContent = `${metrics.memory.usedPercent}%`;
  }

  if (loadElement) {
    loadElement.textContent =
      `${metrics.loadAverage.one} ${metrics.loadAverage.five} ${metrics.loadAverage.fifteen}`;
  }
}

updateMetrics();
setInterval(updateMetrics, 1000);