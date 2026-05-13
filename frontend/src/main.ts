async function updateMetrics() {
  const response = await fetch("http://127.0.0.1:8080/metrics");
  const metrics = await response.json();

  const cpuElement = document.querySelector("#cpu-value");

  if (cpuElement) {
    cpuElement.textContent = `${metrics.cpu.usagePercent}%`;
  }
}

updateMetrics();
setInterval(updateMetrics, 1000);